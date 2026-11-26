// Código final: traduz o código de três endereços (TAC) para Python.
//
// Cada quádrupla (op, arg1, arg2, resultado) vira uma linha de Python:
//   (+, a, b, t0)    ->  t0 = a + b
//   (=, t0, -, x)    ->  x = t0
//   (write, x, -, -) ->  print(x)
//   (read, -, -, x)  ->  x = int(input()) / float(input()) / input(), pelo tipo de x
//
// Estrutura do arquivo gerado:
//   1. funções auxiliares fixas (_ler e _div)
//   2. uma 'def' para cada função do TAC (begin ... end), inclusive 'inicio'
//   3. instruções soltas do TAC (fora de begin/end) = inicialização das globais
//   4. chamada inicio()
//
// Python não tem 'goto'. Uma função cujo TAC tem rótulos e saltos é traduzida
// como uma máquina de estados: o contador _pc diz qual bloco (trecho entre dois
// rótulos) executa; pular para um rótulo é "_pc = k; continue".

#include <stdio.h>
#include <string.h>
#include "ast.h"
#include "simbolos.h"
#include "codigo_intermediario.h"
#include "codigo_final.h"

extern TabelaSimbolos *tabela;      // tabela de símbolos (definida no parser)

#define MAX_NOMES 64

// ---------- nomes e operandos ----------

static const char *palavras_python[] = {
    "False", "None", "True", "and", "as", "assert", "async", "await", "break",
    "class", "continue", "def", "del", "elif", "else", "except", "finally",
    "for", "from", "global", "if", "import", "in", "is", "lambda", "nonlocal",
    "not", "or", "pass", "raise", "return", "try", "while", "with", "yield", NULL
};

static int eh_palavra_python(const char *nome) {
    for (int i = 0; palavras_python[i] != NULL; i++)
        if (strcmp(nome, palavras_python[i]) == 0) return 1;
    return 0;
}

// Nome igual a palavra reservada do Python ganha um '_' no fim
static void escreve_nome(FILE *out, const char *nome) {
    fputs(nome, out);
    if (eh_palavra_python(nome)) fputc('_', out);
}

static void escreve_opd(FILE *out, Operando o) {
    switch (o.tipo) {
    case OPD_CONST_INT:
        fprintf(out, "%ld", o.valor.inteiro);
        break;
    case OPD_CONST_REAL: {
        char buf[32];
        snprintf(buf, sizeof buf, "%g", o.valor.real);
        fputs(buf, out);
        // %g escreve 2.0 como "2", o que viraria inteiro no Python
        if (strpbrk(buf, ".en") == NULL) fputs(".0", out);
        break;
    }
    case OPD_CONST_STRING:
        fputs(o.valor.nome, out);      // o lexer já guarda as aspas
        break;
    case OPD_VAR:
    case OPD_TEMP:
    case OPD_FUNC:
        escreve_nome(out, o.valor.nome);
        break;
    default:
        fputs("None", out);
        break;
    }
}

static void indenta(FILE *out, int nivel) {
    for (int k = 0; k < nivel; k++) fputs("    ", out);
}

// ---------- tipos declarados ----------
//
// O TAC não guarda tipos, então consultamos a tabela de símbolos. Só nos importa
// o 'real': no Python, "real r = 7" guardaria o inteiro 7 e "r / 2" daria 3 em vez
// de 3.5. Por isso valores que vão para variável, parâmetro ou retorno 'real'
// passam por float(...).

// Tipo declarado de uma variável; NULL se não existe ou se o mesmo nome tem tipos
// diferentes em escopos diferentes (ambíguo: não convertemos nada).
static const char *tipo_variavel(const char *nome) {
    if (tabela == NULL) return NULL;
    const char *tipo = NULL;
    for (int k = 0; k < tabela->count; k++) {
        const Simbolo *s = tabela->simbolos[k];
        if (strcmp(s->nome, nome) != 0) continue;
        if (tipo == NULL) tipo = s->tipo;
        else if (strcmp(tipo, s->tipo) != 0) return NULL;
    }
    return tipo;
}

static int eh_real(Operando o) {
    if (o.tipo != OPD_VAR) return 0;
    const char *t = tipo_variavel(o.valor.nome);
    return t != NULL && strcmp(t, "real") == 0;
}

// Expressão Python que lê uma linha e converte pelo tipo declarado do destino.
// Tipo desconhecido ou ambíguo: _ler() deduz o tipo pelo texto digitado.
static const char *leitura_por_tipo(Operando destino) {
    const char *t = (destino.tipo == OPD_VAR) ? tipo_variavel(destino.valor.nome) : NULL;
    if (t == NULL)                    return "_ler()";
    if (strcmp(t, "inteiro") == 0)    return "int(input())";
    if (strcmp(t, "real") == 0)       return "float(input())";
    if (strcmp(t, "cadeia") == 0 ||
        strcmp(t, "caractere") == 0)  return "input()";
    if (strcmp(t, "logico") == 0)     return "int(input().strip() == \"verdadeiro\")";
    return "_ler()";
}

// Símbolo da função (escopo 0): guarda o tipo de retorno e a lista de parâmetros
static const Simbolo *simbolo_funcao(const char *nome) {
    if (tabela == NULL) return NULL;
    for (int k = 0; k < tabela->count; k++) {
        const Simbolo *s = tabela->simbolos[k];
        if (s->escopo == 0 && strcmp(s->nome, nome) == 0) return s;
    }
    return NULL;
}

// Tipo de retorno da função que está sendo escrita (NULL para 'inicio')
static const char *retorno_atual = NULL;

// ---------- tradução de uma instrução ----------

// Operador Python de uma operação binária; NULL se a operação não for binária
static const char *op_python(OPCode op) {
    switch (op) {
    case OP_ADD: return "+";   case OP_SUB: return "-";
    case OP_MUL: return "*";   case OP_MOD: return "%";
    case OP_EQ:  return "==";  case OP_NE:  return "!=";
    case OP_LT:  return "<";   case OP_GT:  return ">";
    case OP_LE:  return "<=";  case OP_GE:  return ">=";
    case OP_AND: return "and"; case OP_OR:  return "or";
    default:     return NULL;
    }
}

// Argumentos já emitidos por 'param' e ainda não consumidos por um 'call'.
// O gerador de TAC emite os 'param' de uma chamada logo antes do seu 'call'.
static Operando pendentes[MAX_NOMES];
static int npendentes = 0;

// Número do bloco de um rótulo dentro da região [ini, fim): o 1º rótulo é o bloco 1,
// o 2º é o bloco 2... (o bloco 0 é o trecho antes do primeiro rótulo).
static int indice_rotulo(const Instrucao *ini, const Instrucao *fim, const char *nome) {
    int k = 0;
    for (const Instrucao *i = ini; i != fim; i = i->proximo) {
        if (i->op != OP_LABEL) continue;
        k++;
        if (strcmp(i->resultado.valor.nome, nome) == 0) return k;
    }
    return k + 1;       // rótulo inexistente: nenhum bloco casa e a função termina
}

// Escreve a(s) linha(s) Python da instrução com o nível de indentação dado.
// [ini, fim) é a região da função, usada para achar o destino dos saltos.
// Devolve 1 se escreveu algum comando, 0 se não escreveu nada (ex.: param).
static int emite_instrucao(FILE *out, const Instrucao *i, int nivel,
                           const Instrucao *ini, const Instrucao *fim) {
    const char *op = op_python(i->op);

    if (op != NULL) {                                   // r = a op b
        indenta(out, nivel);
        escreve_opd(out, i->resultado); fputs(" = ", out);
        escreve_opd(out, i->arg1);
        fprintf(out, " %s ", op);
        escreve_opd(out, i->arg2);
        fputc('\n', out);
        return 1;
    }

    switch (i->op) {
    case OP_DIV:                                        // r = _div(a, b)
        indenta(out, nivel);
        escreve_opd(out, i->resultado); fputs(" = _div(", out);
        escreve_opd(out, i->arg1); fputs(", ", out);
        escreve_opd(out, i->arg2); fputs(")\n", out);
        return 1;
    case OP_NOT:                                        // r = not a
        indenta(out, nivel);
        escreve_opd(out, i->resultado); fputs(" = not ", out);
        escreve_opd(out, i->arg1); fputc('\n', out);
        return 1;
    case OP_ASSIGN:                                     // x = a   (x real: x = float(a))
        indenta(out, nivel);
        escreve_opd(out, i->resultado); fputs(" = ", out);
        if (eh_real(i->resultado)) {
            fputs("float(", out); escreve_opd(out, i->arg1); fputs(")\n", out);
        } else {
            escreve_opd(out, i->arg1); fputc('\n', out);
        }
        return 1;
    case OP_WRITE:                                      // print(a)
        indenta(out, nivel); fputs("print(", out);
        escreve_opd(out, i->arg1); fputs(")\n", out);
        return 1;
    case OP_READ:                                       // x = int(input()), float(input()), ...
        indenta(out, nivel);
        escreve_opd(out, i->resultado); fputs(" = ", out);
        fputs(leitura_por_tipo(i->resultado), out);
        fputc('\n', out);
        return 1;

    case OP_PARAM:                                      // guarda o argumento até o call
        if (npendentes < MAX_NOMES) pendentes[npendentes++] = i->arg1;
        return 0;
    case OP_CALL: {                                     // r = f(a1, a2, ...)
        int qtd = (int)i->arg2.valor.inteiro;
        if (qtd > npendentes) qtd = npendentes;
        indenta(out, nivel);
        escreve_opd(out, i->resultado); fputs(" = ", out);
        escreve_opd(out, i->arg1); fputc('(', out);
        for (int k = 0; k < qtd; k++) {
            if (k > 0) fputs(", ", out);
            escreve_opd(out, pendentes[npendentes - qtd + k]);
        }
        fputs(")\n", out);
        npendentes -= qtd;
        return 1;
    }
    case OP_RETURN:                                     // return v
        indenta(out, nivel); fputs("return", out);
        if (i->arg1.tipo != OPD_NENHUM) {
            fputc(' ', out);
            if (retorno_atual != NULL && strcmp(retorno_atual, "real") == 0) {
                fputs("float(", out); escreve_opd(out, i->arg1); fputc(')', out);
            } else {
                escreve_opd(out, i->arg1);
            }
        }
        fputc('\n', out);
        return 1;

    case OP_JUMP:                                       // goto L
        indenta(out, nivel);
        fprintf(out, "_pc = %d\n", indice_rotulo(ini, fim, i->resultado.valor.nome));
        indenta(out, nivel); fputs("continue\n", out);
        return 1;
    case OP_JUMP_IF_ZERO:                               // ifFalse c goto L
    case OP_JUMP_IF_NOT_ZERO:                           // ifTrue  c goto L
        indenta(out, nivel);
        fputs(i->op == OP_JUMP_IF_ZERO ? "if not " : "if ", out);
        escreve_opd(out, i->arg1); fputs(":\n", out);
        indenta(out, nivel + 1);
        fprintf(out, "_pc = %d\n", indice_rotulo(ini, fim, i->resultado.valor.nome));
        indenta(out, nivel + 1); fputs("continue\n", out);
        return 1;

    default:
        // label, begin, end, formal: tratados por quem monta a função
        return 0;
    }
}

// Corpo de uma função (instruções de [ini, fim)) no nível de indentação dado.
static void emite_corpo(FILE *out, const Instrucao *ini, const Instrucao *fim, int nivel) {
    int tem_fluxo = 0;
    for (const Instrucao *i = ini; i != fim; i = i->proximo)
        if (i->op == OP_LABEL || i->op == OP_JUMP ||
            i->op == OP_JUMP_IF_ZERO || i->op == OP_JUMP_IF_NOT_ZERO)
            tem_fluxo = 1;

    // Sem saltos: as instruções saem em sequência
    if (!tem_fluxo) {
        int n = 0;
        for (const Instrucao *i = ini; i != fim; i = i->proximo)
            n += emite_instrucao(out, i, nivel, ini, fim);
        if (n == 0) { indenta(out, nivel); fputs("pass\n", out); }
        return;
    }

    // Com saltos: máquina de estados. Cada 'if _pc == k' é um bloco. O fim de um
    // bloco passa para o seguinte com "_pc = k+1" (os 'if' são seguidos, não 'elif',
    // então o próximo bloco já executa na mesma volta do laço).
    indenta(out, nivel);     fputs("_pc = 0\n", out);
    indenta(out, nivel);     fputs("while True:\n", out);
    indenta(out, nivel + 1); fputs("if _pc == 0:\n", out);

    int bloco = 0, linhas = 0, termina = 0;
    for (const Instrucao *i = ini; i != fim; i = i->proximo) {
        if (i->op == OP_LABEL) {
            if (!termina) {                       // cai no bloco seguinte
                indenta(out, nivel + 2);
                fprintf(out, "_pc = %d\n", bloco + 1);
            }
            bloco++; linhas = 0; termina = 0;
            indenta(out, nivel + 1);
            fprintf(out, "if _pc == %d:\n", bloco);
            continue;
        }
        linhas += emite_instrucao(out, i, nivel + 2, ini, fim);
        termina = (i->op == OP_RETURN || i->op == OP_JUMP);
    }
    if (linhas == 0 && !termina) { indenta(out, nivel + 2); fputs("pass\n", out); }
    indenta(out, nivel + 1); fputs("break\n", out);   // acabou o último bloco: sai da função
}

// ---------- variáveis globais ----------

static int eh_funcao(const Instrucao *lista, const char *nome) {
    for (const Instrucao *i = lista; i != NULL; i = i->proximo)
        if (i->op == OP_BEGIN && strcmp(i->arg1.valor.nome, nome) == 0) return 1;
    return 0;
}

// Uma variável é global se está na tabela de símbolos no escopo 0 (e não é função).
// Se o mesmo nome também é declarado em escopo local, é ambíguo (o TAC não diz
// qual é qual): nesse caso tratamos como local para não sobrescrever a global.
static int eh_global(const Instrucao *lista, const char *nome) {
    if (tabela == NULL) return 0;
    int global = 0;
    for (int k = 0; k < tabela->count; k++) {
        const Simbolo *s = tabela->simbolos[k];
        if (strcmp(s->nome, nome) != 0) continue;
        if (s->escopo == 0) global = 1;
        else return 0;
    }
    return global && !eh_funcao(lista, nome);
}

static int ja_listado(const char *lista[], int n, const char *nome) {
    for (int k = 0; k < n; k++)
        if (strcmp(lista[k], nome) == 0) return 1;
    return 0;
}

// ---------- funções e arquivo completo ----------

// def nome(parametros): + corpo. 'begin' e 'fim' (o 'end') delimitam a função no TAC.
static void emite_funcao(FILE *out, const Instrucao *lista,
                         const Instrucao *begin, const Instrucao *fim) {
    const char *formais[MAX_NOMES];
    int nformais = 0;

    // (formal, nome) logo após o begin: nomes dos parâmetros, na ordem
    const Instrucao *corpo = begin->proximo;
    while (corpo != fim && corpo->op == OP_FORMAL) {
        if (nformais < MAX_NOMES) formais[nformais++] = corpo->arg1.valor.nome;
        corpo = corpo->proximo;
    }

    fputs("def ", out);
    escreve_nome(out, begin->arg1.valor.nome);
    fputc('(', out);
    for (int k = 0; k < nformais; k++) {
        if (k > 0) fputs(", ", out);
        escreve_nome(out, formais[k]);
    }
    fputs("):\n", out);

    // 'global x' para as globais que a função atribui (sem isso o Python criaria
    // uma variável local nova com o mesmo nome)
    const char *globais[MAX_NOMES];
    int nglobais = 0;
    for (const Instrucao *i = corpo; i != fim; i = i->proximo) {
        if ((i->op != OP_ASSIGN && i->op != OP_READ) || i->resultado.tipo != OPD_VAR) continue;
        const char *nome = i->resultado.valor.nome;
        if (nglobais >= MAX_NOMES || ja_listado(globais, nglobais, nome)) continue;
        if (ja_listado(formais, nformais, nome) || !eh_global(lista, nome)) continue;
        globais[nglobais++] = nome;
    }
    if (nglobais > 0) {
        indenta(out, 1); fputs("global ", out);
        for (int k = 0; k < nglobais; k++) {
            if (k > 0) fputs(", ", out);
            escreve_nome(out, globais[k]);
        }
        fputc('\n', out);
    }

    // Parâmetros 'real' recebidos como inteiro (ex.: soma(1, 2)) viram float na entrada
    const Simbolo *fn = simbolo_funcao(begin->arg1.valor.nome);
    retorno_atual = (fn != NULL) ? fn->tipo : NULL;
    if (fn != NULL) {
        const AstNode *param = fn->parametros;
        for (int k = 0; k < nformais && param != NULL; k++, param = param->dados.bloco.proximo) {
            const char *tipo = param->dados.bloco.comando->dados.parametro.tipo_dado;
            if (tipo != NULL && strcmp(tipo, "real") == 0) {
                indenta(out, 1);
                escreve_nome(out, formais[k]); fputs(" = float(", out);
                escreve_nome(out, formais[k]); fputs(")\n", out);
            }
        }
    }

    npendentes = 0;
    emite_corpo(out, corpo, fim, 1);
    fputc('\n', out);
    retorno_atual = NULL;
}

static void escreve_auxiliares(FILE *out) {
    fputs(
        "# Codigo gerado pelo compilador Portugol (Grupo 18)\n"
        "\n"
        "def _ler():\n"
        "    # leia(): le uma linha e converte para inteiro, depois real, senao texto\n"
        "    s = input()\n"
        "    try:\n"
        "        return int(s)\n"
        "    except ValueError:\n"
        "        try:\n"
        "            return float(s)\n"
        "        except ValueError:\n"
        "            return s\n"
        "\n"
        "def _div(a, b):\n"
        "    # divisao inteira se os dois lados forem inteiros, real caso contrario\n"
        "    if isinstance(a, int) and isinstance(b, int):\n"
        "        return a // b\n"
        "    return a / b\n"
        "\n", out);
}

void gerar_codigo_final(Instrucao *lista, FILE *saida) {
    escreve_auxiliares(saida);

    // 1) Uma def para cada função (begin ... end), inclusive 'inicio'
    int tem_inicio = 0;
    for (const Instrucao *i = lista; i != NULL; i = i->proximo) {
        if (i->op != OP_BEGIN) continue;
        const Instrucao *fim = i->proximo;
        while (fim != NULL && fim->op != OP_END) fim = fim->proximo;
        if (fim == NULL) {
            fprintf(stderr, "aviso: funcao %s sem 'end' no TAC\n", i->arg1.valor.nome);
            break;
        }
        if (strcmp(i->arg1.valor.nome, "inicio") == 0) tem_inicio = 1;
        emite_funcao(saida, lista, i, fim);
        i = fim;
    }

    // 2) Instruções fora de qualquer função: inicialização das variáveis globais
    int dentro = 0;
    npendentes = 0;
    for (const Instrucao *i = lista; i != NULL; i = i->proximo) {
        if (i->op == OP_BEGIN) { dentro = 1; continue; }
        if (i->op == OP_END)   { dentro = 0; continue; }
        if (!dentro) emite_instrucao(saida, i, 0, lista, NULL);
    }

    // 3) Começa a execução
    if (tem_inicio) fputs("inicio()\n", saida);
}
