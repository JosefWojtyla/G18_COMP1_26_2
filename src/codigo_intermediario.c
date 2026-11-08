// Cada instrução é uma quádrupla: (op, arg1, arg2, resultado)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

typedef enum { 
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_MOD,
    OP_EQ, OP_NE, OP_LT, OP_GT, OP_LE, OP_GE,
    OP_AND, OP_OR, OP_NOT,
    OP_ASSIGN,
    OP_JUMP,
    OP_JUMP_IF_ZERO,
    OP_JUMP_IF_NOT_ZERO,
    OP_LABEL,
    OP_BEGIN, OP_END, OP_PARAM, OP_CALL, OP_RETURN,
    OP_WRITE, OP_READ
} OPCode;

typedef enum {
    OPD_NENHUM,
    OPD_CONST_INT,
    OPD_CONST_REAL,
    OPD_CONST_STRING,
    OPD_VAR,
    OPD_TEMP,
    OPD_ROTULO,
    OPD_FUNC,
} TipoOPD;

typedef struct {
    TipoOPD tipo;
    union {
        char *nome;
        long inteiro;
        double real;
    } valor;
} Operando;

typedef struct Instrucao{
    OPCode op;
    Operando arg1,arg2,resultado;
    struct Instrucao *proximo;
} Instrucao; //lista encadeada de instruções

static Instrucao *lista_instrucoes = NULL;
static int contador_temp = 0, contador_rotulo = 0;

// contrutores

static char *copia(const char *str) {
    char *copia = malloc(strlen(str) + 1);
    if (!copia) {
        perro("Erro ao alocar memória para cópia de string"); exit(EXIT_FAILURE);
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
        perro("Erro ao alocar memória para nome de temporário"); exit(EXIT_FAILURE);
    }
    sprintf(opd.valor.nome, "t%d", contador_temp++);
    return opd;
}

static Operando opd_novo_rotulo() {
    Operando opd;
    opd.tipo = OPD_ROTULO;
    opd.valor.nome = malloc(20);
    if (!opd.valor.nome) {
        perro("Erro ao alocar memória para nome de rótulo"); exit(EXIT_FAILURE);
    }
    sprintf(opd.valor.nome, "L%d", contador_rotulo++);
    return opd;
}
