// Cada instrução é uma quádrupla: (op, arg1, arg2, resultado)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

#include "codigo_intermediario.h"

static Instrucao *primeira = NULL, *ultima = NULL;

Instrucao *obtem_tac(void) {
    return primeira;
}

static int contador_temp = 0, contador_rotulo = 0;

// contrutores

static char *copia(const char *str) {
    char *copia = malloc(strlen(str) + 1);
    if (!copia) {
        perror("Erro ao alocar memória para cópia de string"); exit(EXIT_FAILURE);
    }
    strcpy(copia, str);
    return copia;
}

static Operando opd_nenhum() {
    Operando opd;
    opd.tipo = OPD_NENHUM;
    return opd;
}

static Operando opd_nome(TipoOPD tipo, const char *nome) {
    Operando opd;
    opd.tipo = tipo;
    opd.valor.nome = copia(nome);
    return opd;
}

static Operando opd_inteiro(long valor) {
    Operando opd;
    opd.tipo = OPD_CONST_INT;
    opd.valor.inteiro = valor;
    return opd;
}

static Operando opd_real(double valor) {
    Operando opd;
    opd.tipo = OPD_CONST_REAL;
    opd.valor.real = valor;
    return opd;
}

static Operando opd_novo_temp() {
    Operando opd;
    opd.tipo = OPD_TEMP;
    opd.valor.nome = malloc(20);
    if (!opd.valor.nome) {
        perror("Erro ao alocar memória para nome de temporário"); exit(EXIT_FAILURE);
    }
    sprintf(opd.valor.nome, "t%d", contador_temp++);
    return opd;
}

static Operando opd_novo_rotulo() {
    Operando opd;
    opd.tipo = OPD_ROTULO;
    opd.valor.nome = malloc(20);
    if (!opd.valor.nome) {
        perror("Erro ao alocar memória para nome de rótulo"); exit(EXIT_FAILURE);
    }
    sprintf(opd.valor.nome, "L%d", contador_rotulo++);
    return opd;
}


// Lista de instruções

static void emite(OPCode op, Operando a1, Operando a2, Operando r) {
    Instrucao *i = calloc(1, sizeof *i);
    if (!i) { perror("calloc"); exit(EXIT_FAILURE); }
    i->op = op;
    i->arg1 = a1;
    i->arg2 = a2;
    i->resultado = r;
    if (ultima) ultima->proximo = i;
    else        primeira = i;
    ultima = i;
}

static void emite_rotulo(Operando l) {
    emite(OP_LABEL, opd_nenhum(), opd_nenhum(), l);
}


// Tradução da AST para código intermediário

static OPCode op_de_string(const char *op) {
    if (strcmp(op, "+") == 0) return OP_ADD;
    if (strcmp(op, "-") == 0) return OP_SUB;
    if (strcmp(op, "*") == 0) return OP_MUL;
    if (strcmp(op, "/") == 0) return OP_DIV;
    if (strcmp(op, "%") == 0) return OP_MOD;
    if (strcmp(op, "==") == 0) return OP_EQ;
    if (strcmp(op, "!=") == 0) return OP_NE;
    if (strcmp(op, "<") == 0) return OP_LT;
    if (strcmp(op, ">") == 0) return OP_GT;
    if (strcmp(op, "<=") == 0) return OP_LE;
    if (strcmp(op, ">=") == 0) return OP_GE;
    if (strcmp(op, "e") == 0) return OP_AND;
    if (strcmp(op, "ou") == 0) return OP_OR;
    if (strcmp(op, "nao") == 0) return OP_NOT;
    fprintf(stderr, "Operador desconhecido: %s\n", op);
    exit(EXIT_FAILURE);
}

// lista de AST (listas da AST são NO_BLOCO encadeados: comando + proximo)

static int conta_lista(AstNode *lista) {
    int count = 0;
    while (lista) {
        count++;
        lista = lista->dados.bloco.proximo;
    }
    return count;
}

//expressões

static Operando gera_expr(AstNode *n);

static Operando gera_chamada(AstNode *n) {
    int qtd = conta_lista(n->dados.chamada.argumentos);

    Operando *vals = malloc((qtd ? qtd : 1) * sizeof *vals);
    if (!vals) { perror("malloc"); exit(EXIT_FAILURE); }
    int i = 0;
    for (AstNode *c = n->dados.chamada.argumentos; c != NULL; c = c->dados.bloco.proximo)
        vals[i++] = gera_expr(c->dados.bloco.comando);


    for (i = 0; i < qtd; i++)
        emite(OP_PARAM, vals[i], opd_nenhum(), opd_nenhum());
    free(vals);


    Operando r = opd_novo_temp();
    emite(OP_CALL, opd_nome(OPD_FUNC, n->dados.chamada.nome), opd_inteiro(qtd), r);
    return r;
}

static Operando gera_expr(AstNode *n) {
    if (n == NULL) {
        /* Vetores ainda não têm nó na AST (decisão: fora de escopo por ora).
           Em vez de abortar o programa, avisa e segue com operando vazio. */
        fprintf(stderr, "aviso: acesso a vetor ainda nao suportado no codigo intermediario (ignorado)\n");
        return opd_nenhum();
    }

    switch (n->tipo) {
    case NO_INTEIRO:
        return opd_inteiro(n->dados.inteiro.valor);
    case NO_REAL:
        return opd_real(n->dados.real.valor);
    case NO_LOGICO:
        return opd_inteiro(n->dados.logico.valor);
    case NO_CADEIA:
        return opd_nome(OPD_CONST_STRING, n->dados.cadeia.valor);
    case NO_VARIAVEL:
        return opd_nome(OPD_VAR, n->dados.variavel.nome);

    case NO_OP_BINARIA: {
        const char *op = n->dados.op_binaria.op;

        if (strcmp(op, "nao") == 0) {               
            Operando a = gera_expr(n->dados.op_binaria.dir);
            Operando t = opd_novo_temp();
            emite(OP_NOT, a, opd_nenhum(), t);
            return t;
        }

        Operando a = gera_expr(n->dados.op_binaria.esq); 
        Operando b = gera_expr(n->dados.op_binaria.dir);
        Operando t = opd_novo_temp();
        emite(op_de_string(op), a, b, t);
        return t;
    }

    case NO_CHAMADA:
        return gera_chamada(n);

    default:
        fprintf(stderr, "gera_expr: tipo de nó %d não é expressão\n", n->tipo);
        exit(EXIT_FAILURE);
    }
}

// comandos


static void gera_comando(AstNode *n);


static void gera_bloco(AstNode *lista) {
    for (AstNode *c = lista; c != NULL; c = c->dados.bloco.proximo)
        gera_comando(c->dados.bloco.comando);
}

static void gera_atribuicao(AstNode *n) {
    Operando v = gera_expr(n->dados.atribuicao.expressao);
    emite(OP_ASSIGN, v, opd_nenhum(), opd_nome(OPD_VAR, n->dados.atribuicao.nome));
}

static void gera_se(AstNode *n) {
    Operando l_senao = opd_novo_rotulo();
    Operando l_fim   = opd_novo_rotulo();

    Operando c = gera_expr(n->dados.se.condicao);
    emite(OP_JUMP_IF_ZERO, c, opd_nenhum(), l_senao);

    gera_bloco(n->dados.se.entao);

    if (n->dados.se.senao != NULL) {
        emite(OP_JUMP, opd_nenhum(), opd_nenhum(), l_fim);
        emite_rotulo(l_senao);
        gera_bloco(n->dados.se.senao);
        emite_rotulo(l_fim);
    } else {
        emite_rotulo(l_senao);
    }
}

static void gera_enquanto(AstNode *n) {
    Operando l_ini = opd_novo_rotulo();
    Operando l_fim = opd_novo_rotulo();

    emite_rotulo(l_ini);                             
    Operando c = gera_expr(n->dados.enquanto.condicao);
    emite(OP_JUMP_IF_ZERO, c, opd_nenhum(), l_fim);

    gera_bloco(n->dados.enquanto.entao);
    emite(OP_JUMP, opd_nenhum(), opd_nenhum(), l_ini);
    emite_rotulo(l_fim);
}

static void gera_para(AstNode *n) {
    gera_comando(n->dados.para.inicializa);         

    Operando l_ini   = opd_novo_rotulo();
    Operando l_passo = opd_novo_rotulo();
    Operando l_fim   = opd_novo_rotulo();

    emite_rotulo(l_ini);
    Operando c = gera_expr(n->dados.para.condicao);
    emite(OP_JUMP_IF_ZERO, c, opd_nenhum(), l_fim);

    gera_bloco(n->dados.para.entao);

    emite_rotulo(l_passo);
    gera_comando(n->dados.para.passo);         
    emite(OP_JUMP, opd_nenhum(), opd_nenhum(), l_ini);
    emite_rotulo(l_fim);
}

static void gera_funcao(AstNode *n) {
    int qtd = conta_lista(n->dados.funcao.parametros);
    emite(OP_BEGIN, opd_nome(OPD_FUNC, n->dados.funcao.nome), opd_inteiro(qtd), opd_nenhum());

    // um (formal, nome) por parâmetro, na ordem da declaração: o código final
    // usa essas linhas para montar o cabeçalho da função
    for (AstNode *p = n->dados.funcao.parametros; p != NULL; p = p->dados.bloco.proximo)
        emite(OP_FORMAL, opd_nome(OPD_VAR, p->dados.bloco.comando->dados.parametro.nome),
              opd_nenhum(), opd_nenhum());

    gera_bloco(n->dados.funcao.corpo);
    emite(OP_END, opd_nome(OPD_FUNC, n->dados.funcao.nome), opd_nenhum(), opd_nenhum());
}

static void gera_comando(AstNode *n) {
    if (n == NULL) return;  

    switch (n->tipo) {
    case NO_BLOCO:
        // O único bloco aninhado na AST do programa é o corpo de 'inicio': o parser
        // o encaixa como um item da lista do programa. Marcamos com begin/end, como
        // uma função qualquer, para o código final separar inicio das variáveis globais.
        emite(OP_BEGIN, opd_nome(OPD_FUNC, "inicio"), opd_inteiro(0), opd_nenhum());
        gera_bloco(n);
        emite(OP_END, opd_nome(OPD_FUNC, "inicio"), opd_nenhum(), opd_nenhum());
        break;

    case NO_ATRIBUICAO:
        gera_atribuicao(n);
        break;

    case NO_ESCREVA: {
        Operando v = gera_expr(n->dados.escreva.expressao);
        emite(OP_WRITE, v, opd_nenhum(), opd_nenhum());
        break;
    }

    case NO_LEIA:
        emite(OP_READ, opd_nenhum(), opd_nenhum(), opd_nome(OPD_VAR, n->dados.leia.nome));
        break;

    case NO_RETORNE: {
        Operando v = gera_expr(n->dados.retorne.expressao);
        emite(OP_RETURN, v, opd_nenhum(), opd_nenhum());
        break;
    }

    case NO_CHAMADA:        
        gera_chamada(n);
        break;

    case NO_SE:       gera_se(n);       break;
    case NO_ENQUANTO: gera_enquanto(n); break;
    case NO_PARA:     gera_para(n);     break;
    case NO_FUNCAO:   gera_funcao(n);   break;

    default:
        fprintf(stderr, "gera_comando: tipo de nó %d não suportado\n", n->tipo);
        exit(EXIT_FAILURE);
    }
}

// Ponto de entrada
void gera_codigo(AstNode *raiz) {
    gera_bloco(raiz);
}

// imprime cada quádrupla no formato (op, arg1, arg2, resultado)


static const char *nome_opcode(OPCode op) {
    switch (op) {
    case OP_ADD: return "+";      case OP_SUB: return "-";
    case OP_MUL: return "*";      case OP_DIV: return "/";
    case OP_MOD: return "%";
    case OP_EQ:  return "==";     case OP_NE:  return "!=";
    case OP_LT:  return "<";      case OP_GT:  return ">";
    case OP_LE:  return "<=";     case OP_GE:  return ">=";
    case OP_AND: return "and";    case OP_OR:  return "or";
    case OP_NOT: return "not";
    case OP_ASSIGN:           return "=";
    case OP_JUMP:             return "goto";
    case OP_JUMP_IF_ZERO:     return "ifFalse";
    case OP_JUMP_IF_NOT_ZERO: return "ifTrue";
    case OP_LABEL:            return "label";
    case OP_BEGIN:  return "begin";  case OP_END:    return "end";
    case OP_PARAM:  return "param";  case OP_CALL:   return "call";
    case OP_RETURN: return "return";
    case OP_WRITE:  return "write";  case OP_READ:   return "read";
    case OP_FORMAL: return "formal";
    }
    return "?";
}
 
static void imprime_opd(Operando o) {
    switch (o.tipo) {
    case OPD_NENHUM:        fprintf(stderr, "-"); break;
    case OPD_CONST_INT:     fprintf(stderr, "%ld", o.valor.inteiro); break;
    case OPD_CONST_REAL:    fprintf(stderr, "%g", o.valor.real); break;
    case OPD_CONST_STRING:  fprintf(stderr, "%s", o.valor.nome); break;   /* o lexer já guarda as aspas */
    default:                fprintf(stderr, "%s", o.valor.nome); break;
    }
}



void imprime_tac(void) {
    fprintf(stderr, "\n= = = Código de três endereços = = =\n");
    for (Instrucao *i = primeira; i != NULL; i = i->proximo) {
        fprintf(stderr, "(%s, ", nome_opcode(i->op));
        imprime_opd(i->arg1);     fprintf(stderr, ", ");
        imprime_opd(i->arg2);     fprintf(stderr, ", ");
        imprime_opd(i->resultado); fprintf(stderr, ")\n");
    }
    fprintf(stderr, "\n");   // linha em branco marca o fim do TAC (o test-tac do Makefile depende dela)
}

static Instrucao *remove_instrucao(Instrucao *ant, Instrucao *atual) {
    Instrucao *prox = atual->proximo;
    if(ant) ant->proximo = prox;   
    else primeira = prox;  

    if (atual == ultima) ultima = ant;
    free(atual);
    return prox;
}

static int remove_inalcancavel(void) {
    int removidas = 0, morto = 0;
    Instrucao *ant = NULL, *atual = primeira;
    while (atual) {
        
        if (atual->op == OP_LABEL || atual->op == OP_BEGIN || atual->op == OP_END)
            morto = 0;
        if (morto) {
            atual = remove_instrucao(ant, atual);
            removidas++;
            continue;
        }

        if (atual->op == OP_RETURN || atual->op == OP_JUMP)
            morto = 1;
        ant = atual;
        atual = atual->proximo;
    }
    return removidas;
}

static int usa_temp(Operando o, const char *nome) {
    return o.tipo == OPD_TEMP && strcmp(o.valor.nome, nome) == 0;
}

static int temp_eh_usado(const char *nome) {
    for (Instrucao *i = primeira; i; i = i->proximo)
        if (usa_temp(i->arg1, nome) || usa_temp(i->arg2, nome))
            return 1;
    return 0;
}

// Só aritméticas/lógicas (OP_ADD..OP_NOT no enum) 
static int eh_operacao_pura(OPCode op) {
    return op >= OP_ADD && op <= OP_NOT;
}
static int remove_temps_mortos(void) {
    int removidas = 0;
    Instrucao *ant = NULL, *atual = primeira;
    while (atual) {
        if (atual->resultado.tipo == OPD_TEMP
            && eh_operacao_pura(atual->op)
            && !temp_eh_usado(atual->resultado.valor.nome)) {
            atual = remove_instrucao(ant, atual);
            removidas++;
            continue;
        }
        ant = atual;
        atual = atual->proximo;
    }
    return removidas;
}

void otimiza_tac(void) {
    int mudou;
    do {
        mudou = remove_inalcancavel() + remove_temps_mortos();
    } while (mudou);
}