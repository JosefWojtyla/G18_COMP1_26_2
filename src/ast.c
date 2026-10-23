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
AstNode *criar_no_op_binaria(char *op, AstNode *esq, AstNode *dir, int linha){
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

// construtor do para
AstNode *criar_no_para(AstNode *inicializa, AstNode *condicao, AstNode *entao, AstNode *passo, int linha){
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
    no->dados.para.passo= passo;
    return no; 
}

// construtor para atribuição
AstNode *criar_no_atribuicao(char *nome, AstNode *expressao, int linha){
    AstNode *no = (AstNode *)malloc(sizeof(AstNode));

    // validar falha na alocação
    if(!no){
        perror("Erro de alocação de memória para AstNode");
        exit(EXIT_FAILURE);
    }

    no->tipo = NO_ATRIBUICAO;
    no->linha = linha;
    no->dados.atribuicao.nome = nome;
    no->dados.atribuicao.expressao = expressao;
    return no; 
}

// construtor para o retorne
AstNode *criar_no_retorne(AstNode *expressao, int linha){
    AstNode *no = (AstNode *)malloc(sizeof(AstNode));

    // validar falha na alocação
    if(!no){
        perror("Erro de alocação de memória para AstNode");
        exit(EXIT_FAILURE);
    }

    no->tipo = NO_RETORNE;
    no->linha = linha;
    no->dados.retorne.expressao = expressao;
    return no; 
}

// construtor para função
AstNode *criar_no_funcao(char *nome, char *tipo_retorno,AstNode *parametros, AstNode *corpo, int linha){
    AstNode *no = (AstNode *)malloc(sizeof(AstNode));

    // validar falha na alocação
    if(!no){
        perror("Erro de alocação de memória para AstNode");
        exit(EXIT_FAILURE);
    }

    no->tipo = NO_FUNCAO;
    no->linha = linha;
    no->dados.funcao.nome = nome;
    no->dados.funcao.tipo_retorno = tipo_retorno;
    no->dados.funcao.parametros = parametros;
    no->dados.funcao.corpo = corpo;
    return no; 
}

AstNode *criar_no_bloco(AstNode *comando, AstNode *proximo, int linha){
    AstNode *no = (AstNode *)malloc(sizeof(AstNode));

    // validar falha na alocação
    if(!no){
        perror("Erro de alocação de memória para AstNode");
        exit(EXIT_FAILURE);
    }

    no->tipo = NO_BLOCO;
    no->linha = linha;
    no->dados.bloco.comando = comando;
    no->dados.bloco.proximo = proximo;
    return no; 
}
AstNode *criar_no_variavel(char *nome, int linha){
    AstNode *no = (AstNode *)malloc(sizeof(AstNode));

    // validar falha na alocação
    if(!no){
        perror("Erro de alocação de memória para AstNode");
        exit(EXIT_FAILURE);
    }

    no->tipo = NO_VARIAVEL;
    no->linha = linha;
    no->dados.variavel.nome = nome;
    return no; 
}
AstNode *criar_no_escreva(AstNode *expressao, int linha){
    AstNode *no = (AstNode *)malloc(sizeof(AstNode));

    // validar falha na alocação
    if(!no){
        perror("Erro de alocação de memória para AstNode");
        exit(EXIT_FAILURE);
    }

    no->tipo = NO_ESCREVA;
    no->linha = linha;
    no->dados.escreva.expressao = expressao;
    return no; 
}
AstNode *criar_no_leia(char *nome, int linha){
    AstNode *no = (AstNode *)malloc(sizeof(AstNode));

    // validar falha na alocação
    if(!no){
        perror("Erro de alocação de memória para AstNode");
        exit(EXIT_FAILURE);
    }

    no->tipo = NO_LEIA;
    no->linha = linha;
    no->dados.leia.nome = nome;
    return no; 
}
AstNode *criar_no_chamada(char *nome, AstNode *argumentos, int linha){
    AstNode *no = (AstNode *)malloc(sizeof(AstNode));

    // validar falha na alocação
    if(!no){
        perror("Erro de alocação de memória para AstNode");
        exit(EXIT_FAILURE);
    }

    no->tipo = NO_CHAMADA;
    no->linha = linha;
    no->dados.chamada.nome = nome;
    no->dados.chamada.argumentos = argumentos;
    return no; 
}

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
            // FILHOS: inicializa, condição, entao, passo
            liberar_ast(no->dados.para.inicializa);
            liberar_ast(no->dados.para.condicao);
            liberar_ast(no->dados.para.entao);
            liberar_ast(no->dados.para.passo);
            break;
        
        case NO_ATRIBUICAO:
            // FILHO: expressao
            liberar_ast(no->dados.atribuicao.expressao);
            break;

        case NO_RETORNE:
            // FILHO: expressao
            liberar_ast(no->dados.retorne.expressao);
            break;

        case NO_FUNCAO:
            // FILHOS: parametros, corpo
            liberar_ast(no->dados.funcao.parametros);
            liberar_ast(no->dados.funcao.corpo);
            break;
        
        case NO_BLOCO:
            // FILHOS: comando, proximo
            liberar_ast(no->dados.bloco.comando);
            liberar_ast(no->dados.bloco.proximo);
            break;

        case NO_VARIAVEL:
            // NAO POSSUI FILHOS
            break;

        case NO_ESCREVA:
            // FILHO: expressao 
            liberar_ast(no->dados.escreva.expressao);
            break;

        case NO_LEIA:
            // NAO POSSUI FILHOS
            break;
        
        case NO_CHAMADA:
            // FILHO: argumentos
            liberar_ast(no->dados.chamada.argumentos);
            break;
    }

    free(no);
}