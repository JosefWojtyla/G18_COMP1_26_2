#ifndef CODIGO_INTERMEDIARIO_H
#define CODIGO_INTERMEDIARIO_H

// Representação do código de três endereços (TAC).
// Cada instrução é uma quádrupla: (op, arg1, arg2, resultado).
// Fica num header para que o código final (codigo_final.c) consiga ler a lista.

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
    OP_WRITE, OP_READ,
    OP_FORMAL   // nome de um parâmetro formal, emitido logo após o begin da função
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

// Devolve a primeira instrução da lista (NULL se estiver vazia).
// Chamar depois de gera_codigo() e otimiza_tac().
Instrucao *obtem_tac(void);

#endif
