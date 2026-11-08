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
    OP_ASSIGN,
    OP_JUMP,
    OP_JUMP_IF_ZERO,
    OP_JUMP_IF_NOT_ZERO,
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