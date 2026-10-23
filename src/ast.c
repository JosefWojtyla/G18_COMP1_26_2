#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

// construtor para inteiro
AstNode *criar_no_inteiro(int valor, int linha){
    AstNode *no = (AstNode *)malloc(sizeof(AstNode));

    // validar falha na alocação
    if(!no){
        perror("Erro de alocação de memória para AstNode");
        exit(EXIT_FAILURE);
    }

    no->tipo = NO_INTEIRO;
    no->linha = linha;
    no->dados.inteiro.valor = valor;
    return no; 
}

// construtor para real
AstNode *criar_no_real(float valor, int linha){
    AstNode *no = (AstNode *)malloc(sizeof(AstNode));

    // validar falha na alocação
    if(!no){
        perror("Erro de alocação de memória para AstNode");
        exit(EXIT_FAILURE);
    }

    no->tipo = NO_REAL;
    no->linha = linha;
    no->dados.real.valor = valor;
    return no; 
}

// construtor para cadeia
AstNode *criar_no_cadeia(char *valor, int linha){
    AstNode *no = (AstNode *)malloc(sizeof(AstNode));

    // validar falha na alocação
    if(!no){
        perror("Erro de alocação de memória para AstNode");
        exit(EXIT_FAILURE);
    }

    no->tipo = NO_CADEIA;
    no->linha = linha;
    no->dados.cadeia.valor = valor;
    return no; 
}

// construtor para logico
AstNode *criar_no_logico(int valor, int linha){
    AstNode *no = (AstNode *)malloc(sizeof(AstNode));

    // validar falha na alocação
    if(!no){
        perror("Erro de alocação de memória para AstNode");
        exit(EXIT_FAILURE);
    }

    no->tipo = NO_LOGICO;
    no->linha = linha;
    no->dados.logico.valor = valor;
    return no; 
}

// construtor para operação binaria (+, -, *, /)
AstNode *criar_no_op_binaria(char op, AstNode *esq, AstNode *dir, int linha){
    AstNode *no = (AstNode *)malloc(sizeof(AstNode));

    // validar falha na alocação
    if(!no){
        perror("Erro de alocação de memória para AstNode");
        exit(EXIT_FAILURE);
    }

    no->tipo = NO_OP_BINARIA;
    no->linha = linha;
    no->dados.op_binaria.op = op;
    no->dados.op_binaria.esq = esq;
    no->dados.op_binaria.dir = dir;
    return no; 
}

// construtor para se - senao
AstNode *criar_no_se(AstNode *condicao, AstNode *entao, AstNode *senao, int linha){
    AstNode *no = (AstNode *)malloc(sizeof(AstNode));

    // validar falha na alocação
    if(!no){
        perror("Erro de alocação de memória para AstNode");
        exit(EXIT_FAILURE);
    }

    no->tipo = NO_SE;
    no->linha = linha;
    no->dados.se.condicao = condicao;
    no->dados.se.entao = entao;
    no->dados.se.senao= senao;
    return no; 
}

// construtor do enquanto
AstNode *criar_no_enquanto(AstNode *condicao, AstNode *entao, int linha){
    AstNode *no = (AstNode *)malloc(sizeof(AstNode));

    // validar falha na alocação
    if(!no){
        perror("Erro de alocação de memória para AstNode");
        exit(EXIT_FAILURE);
    }

    no->tipo = NO_ENQUANTO;
    no->linha = linha;
    no->dados.enquanto.condicao = condicao;
    no->dados.enquanto.entao = entao;
    return no; 
}

// construtor para para
AstNode *criar_no_para(AstNode *inicializa, AstNode *condicao, AstNode *entao, AstNode *op_inicializa, int linha){
    AstNode *no = (AstNode *)malloc(sizeof(AstNode));

    // validar falha na alocação
    if(!no){
        perror("Erro de alocação de memória para AstNode");
        exit(EXIT_FAILURE);
    }

    no->tipo = NO_PARA;
    no->linha = linha;
    no->dados.para.inicializa= inicializa;
    no->dados.para.condicao = condicao;
    no->dados.para.entao = entao;
    no->dados.para.op_inicializa= op_inicializa;
    return no; 
}

// outros ???

// destrutor
void liberar_ast(AstNode *no){
    if (no == NULL) return;

    switch(no->tipo){
        case NO_INTEIRO:
            // não possui nós filhos
            break;
        
        case NO_REAL:
            // não possui nós filhos
            break;

        case NO_CADEIA:
            // não possui nós filhos
            break;

        case NO_LOGICO:
            // não possui nós filhos
            break;

        case NO_OP_BINARIA:
            liberar_ast(no->dados.op_binaria.esq);
            liberar_ast(no->dados.op_binaria.dir);
            break;

        case NO_SE:
            // FILHOS: condição, entao, senao
            liberar_ast(no->dados.se.condicao);
            liberar_ast(no->dados.se.entao);
            liberar_ast(no->dados.se.senao);
            break;

        case NO_ENQUANTO:
            // FILHOS: condição, entao
            liberar_ast(no->dados.enquanto.condicao);
            liberar_ast(no->dados.enquanto.entao);
            break;

        case NO_PARA:
            // FILHOS: inicializa, condição, entao, op_inicializa
            liberar_ast(no->dados.para.inicializa);
            liberar_ast(no->dados.para.condicao);
            liberar_ast(no->dados.para.entao);
            liberar_ast(no->dados.para.op_inicializa);
            break;
    }

    free(no);
}