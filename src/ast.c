#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    no->dados.cadeia.valor = strdup(valor);
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
    no->dados.op_binaria.op = strdup(op);
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
    no->dados.atribuicao.nome = strdup(nome);
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
    no->dados.funcao.nome = strdup(nome);
    no->dados.funcao.tipo_retorno = strdup(tipo_retorno);
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
    no->dados.variavel.nome = strdup(nome);
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
    no->dados.leia.nome = strdup(nome);
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
    no->dados.chamada.nome = strdup(nome);
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
            free(no->dados.cadeia.valor);
            break;

        case NO_LOGICO:
            // não possui nós filhos
            break;

        case NO_OP_BINARIA:
            free(no->dados.op_binaria.op);
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
            free(no->dados.atribuicao.nome);
            liberar_ast(no->dados.atribuicao.expressao);
            break;

        case NO_RETORNE:
            // FILHO: expressao
            liberar_ast(no->dados.retorne.expressao);
            break;

        case NO_FUNCAO:
            free(no->dados.funcao.nome);
            free(no->dados.funcao.tipo_retorno);
            liberar_ast(no->dados.funcao.parametros);
            liberar_ast(no->dados.funcao.corpo);
            break;
        
        case NO_BLOCO:
            // FILHOS: comando, proximo
            liberar_ast(no->dados.bloco.comando);
            liberar_ast(no->dados.bloco.proximo);
            break;

        case NO_VARIAVEL:
            free(no->dados.variavel.nome);
            break;

        case NO_ESCREVA:
            // FILHO: expressao 
            liberar_ast(no->dados.escreva.expressao);
            break;

        case NO_LEIA:
            free(no->dados.leia.nome);
            break;
        
        case NO_CHAMADA:
            free(no->dados.chamada.nome);
            liberar_ast(no->dados.chamada.argumentos);
            break;
    }

    free(no);
}

// imprimir 2 espaços para cada nivel de profundidade da arvore
static void imprimir_recuo(int nivel){
    for (int i = 0; i < nivel; i++){
        printf("  ");
    }
}

void ast_imprimir(AstNode *no, int nivel){
    if(no == NULL) return;

    if(no->tipo == NO_BLOCO){
        ast_imprimir(no->dados.bloco.comando, nivel);
        ast_imprimir(no->dados.bloco.proximo, nivel);
        return;
    }

    imprimir_recuo(nivel);

    switch(no->tipo){

    

        // GRUPO A: não possuem filhos, apenas imprimem seu valor
        case NO_INTEIRO:
            printf("[INTEIRO: %d] (linha %d)\n", no->dados.inteiro.valor, no->linha);
            break;
        
        case NO_REAL:
            printf("[REAL: %f] (linha %d)\n", no->dados.real.valor, no->linha);
            break;

        case NO_CADEIA:
            printf("[CADEIA: %s] (linha %d)\n", no->dados.cadeia.valor, no->linha);
            break;

        case NO_LOGICO:
            printf("[LOGICO: %s] (linha %d)\n", no->dados.logico.valor ? "verdadeiro" : "falso", no->linha);
            break;

        case NO_VARIAVEL:
            printf("[VARIAVEL: %s] (linha %d)\n", no->dados.variavel.nome, no->linha);
            break;

        case NO_LEIA:
            printf("[LEIA: %s] (linha %d)\n", no->dados.leia.nome, no->linha);
            break;

        // GRUPO B: nós de expressão e comandos simples
        case NO_OP_BINARIA:
            printf("[OP_BINARIA: %s] (linha %d)\n", no->dados.op_binaria.op, no->linha);
            ast_imprimir(no->dados.op_binaria.esq, nivel + 1);
            ast_imprimir(no->dados.op_binaria.dir, nivel + 1);
            break;

        case NO_ATRIBUICAO:
            printf("[ATRIBUICAO: %s] (linha %d)\n", no->dados.atribuicao.nome, no->linha);
            ast_imprimir(no->dados.atribuicao.expressao, nivel + 1);
            break;

        case NO_ESCREVA: 
            printf("[ESCREVA] (linha %d)\n", no->linha);
            ast_imprimir(no->dados.escreva.expressao, nivel + 1);
            break;
        
        case NO_RETORNE:
            printf("[RETORNE] (linha %d)\n", no->linha);
            ast_imprimir(no->dados.retorne.expressao, nivel + 1);
            break;

        case NO_CHAMADA:
            printf("[CHAMADA: %s] (linha %d)\n", no->dados.chamada.nome, no->linha);
            ast_imprimir(no->dados.chamada.argumentos, nivel + 1);
            break;

        // GRUPO C: estruturas de controle e funcoes (se, enquanto, para, funcao)
        case NO_SE:
            printf("[SE] (linha %d)\n", no->linha);
            ast_imprimir(no->dados.se.condicao, nivel + 1);
            ast_imprimir(no->dados.se.entao, nivel + 1);
            // Regra de negócio: o bloco 'senao' é opcional (pode ser NULL)
            if (no->dados.se.senao != NULL) {
                imprimir_recuo(nivel);
                printf("[SENAO] (linha %d)\n", no->linha);
                ast_imprimir(no->dados.se.senao, nivel + 1);
            }
            break;

        case NO_ENQUANTO:
            printf("[ENQUANTO] (linha %d)\n", no->linha);
            ast_imprimir(no->dados.enquanto.condicao, nivel + 1);
            ast_imprimir(no->dados.enquanto.entao, nivel + 1);
            break;

        case NO_PARA:
            // Siga o mesmo padrão do NO_ENQUANTO:
            printf("[PARA] (linha %d)\n", no->linha);
            ast_imprimir(no->dados.para.inicializa, nivel + 1);
            ast_imprimir(no->dados.para.condicao, nivel + 1);
            ast_imprimir(no->dados.para.passo, nivel + 1);
            ast_imprimir(no->dados.para.entao, nivel + 1);
            break;

        case NO_FUNCAO:
            printf("[FUNCAO: %s -> %s] (linha %d)\n", no->dados.funcao.nome, no->dados.funcao.tipo_retorno, no->linha);
            ast_imprimir(no->dados.funcao.parametros, nivel + 1);
            ast_imprimir(no->dados.funcao.corpo, nivel + 1);
            break;

        default:
            break;
    }

}

void ast_liberar(AstNode *raiz) {
    liberar_ast(raiz);
}