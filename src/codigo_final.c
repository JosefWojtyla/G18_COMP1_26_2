// Código final: traduz o código de três endereços (TAC) para Python.
//
// Cada quádrupla (op, arg1, arg2, resultado) vira uma linha de Python:
//   (+, a, b, t0)   ->  t0 = a + b
//   (=, t0, -, x)   ->  x = t0
//   (write, x, -, -) -> print(x)
//   (read, -, -, x)  -> x = _ler()
//
// Estrutura do arquivo gerado:
//   1. funções auxiliares fixas (_ler e _div)
//   2. funções do programa (begin ... end)          [instruções de função: parte 3]
//   3. def inicio(): corpo solto do TAC (globais + inicio)
//   4. chamada inicio()

#include <stdio.h>
#include <string.h>
#include "codigo_intermediario.h"
#include "codigo_final.h"

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

// Variável com nome igual a palavra reservada do Python ganha um '_' no fim
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
        escreve_nome(out, o.valor.nome);
        break;
    default:
        fputs("None", out);
        break;
    }
}

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

// Escreve a(s) linha(s) Python da instrução, com 4 espaços de indentação.
// Devolve 1 se traduziu, 0 se a instrução ainda não é suportada (parte 3).
static int emite_instrucao(FILE *out, const Instrucao *i) {
    const char *op = op_python(i->op);

    if (op != NULL) {                                   // r = a op b
        fputs("    ", out);
        escreve_opd(out, i->resultado); fputs(" = ", out);
        escreve_opd(out, i->arg1);
        fprintf(out, " %s ", op);
        escreve_opd(out, i->arg2);
        fputc('\n', out);
        return 1;
    }

    switch (i->op) {
    case OP_DIV:                                        // r = _div(a, b)
        fputs("    ", out);
        escreve_opd(out, i->resultado); fputs(" = _div(", out);
        escreve_opd(out, i->arg1); fputs(", ", out);
        escreve_opd(out, i->arg2); fputs(")\n", out);
        return 1;
    case OP_NOT:                                        // r = not a
        fputs("    ", out);
        escreve_opd(out, i->resultado); fputs(" = not ", out);
        escreve_opd(out, i->arg1); fputc('\n', out);
        return 1;
    case OP_ASSIGN:                                     // x = a
        fputs("    ", out);
        escreve_opd(out, i->resultado); fputs(" = ", out);
        escreve_opd(out, i->arg1); fputc('\n', out);
        return 1;
    case OP_WRITE:                                      // print(a)
        fputs("    print(", out);
        escreve_opd(out, i->arg1); fputs(")\n", out);
        return 1;
    case OP_READ:                                       // x = _ler()
        fputs("    ", out);
        escreve_opd(out, i->resultado); fputs(" = _ler()\n", out);
        return 1;
    default:
        // goto, ifFalse, label, param, call, return, formal...: parte 3
        fputs("    # instrucao ainda nao traduzida (controle de fluxo / funcoes)\n", out);
        return 0;
    }
}

// ---------- arquivo completo ----------

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
    int nao_traduzidas = 0;

    escreve_auxiliares(saida);

    // Funções do programa (begin ... end): tradução na parte 3
    for (Instrucao *i = lista; i != NULL; i = i->proximo) {
        if (i->op == OP_BEGIN)
            fprintf(saida, "# funcao %s: traducao pendente (controle de fluxo / funcoes)\n",
                    i->arg1.valor.nome);
    }

    // Instruções fora de begin/end: globais e corpo do inicio
    fputs("def inicio():\n", saida);
    int traduzidas = 0, dentro_de_funcao = 0;
    for (Instrucao *i = lista; i != NULL; i = i->proximo) {
        if (i->op == OP_BEGIN) { dentro_de_funcao = 1; continue; }
        if (i->op == OP_END)   { dentro_de_funcao = 0; continue; }
        if (dentro_de_funcao) continue;

        if (emite_instrucao(saida, i)) traduzidas++;
        else nao_traduzidas++;
    }
    if (traduzidas == 0) fputs("    pass\n", saida);   // corpo Python não pode ficar vazio

    fputs("\ninicio()\n", saida);

    if (nao_traduzidas > 0)
        fprintf(stderr, "aviso: %d instrucao(oes) do TAC ainda nao traduzida(s) para Python\n",
                nao_traduzidas);
}
