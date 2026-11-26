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

// construtor do bloco
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

// construtor variavel
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

// construtor para escreva
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

// construtor para leia
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

// construtor para chamada
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
    no->dados.chamada.argumentos = argumentos; //pq aqui não precisa ser passado com strdup()?
    return no; 
}

AstNode *criar_no_parametro(char *nome, char *tipo_dado, int linha){
    AstNode *no = (AstNode *)malloc(sizeof(AstNode));

    // validar falha na alocação
    if(!no){
        perror("Erro de alocação de memória para AstNode");
        exit(EXIT_FAILURE);
    }

    no->tipo = NO_PARAMETRO;
    no->linha = linha;
    no->dados.parametro.nome = strdup(nome);
    no->dados.parametro.tipo_dado = strdup(tipo_dado);
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

        case NO_PARAMETRO:
            free(no->dados.parametro.nome);
            free(no->dados.parametro.tipo_dado);
            break;
    }

    free(no);
}

// imprimir 2 espaços para cada nivel de profundidade da arvore
static void imprimir_recuo(int nivel){
    for (int i = 0; i < nivel; i++){
        fprintf(stderr, "  ");
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
            fprintf(stderr, "[INTEIRO: %d] (linha %d)\n", no->dados.inteiro.valor, no->linha);
            break;
        
        case NO_REAL:
            fprintf(stderr, "[REAL: %f] (linha %d)\n", no->dados.real.valor, no->linha);
            break;

        case NO_CADEIA:
            fprintf(stderr, "[CADEIA: %s] (linha %d)\n", no->dados.cadeia.valor, no->linha);
            break;

        case NO_LOGICO:
            fprintf(stderr, "[LOGICO: %s] (linha %d)\n", no->dados.logico.valor ? "verdadeiro" : "falso", no->linha);
            break;

        case NO_VARIAVEL:
            fprintf(stderr, "[VARIAVEL: %s] (linha %d)\n", no->dados.variavel.nome, no->linha);
            break;

        case NO_LEIA:
            fprintf(stderr, "[LEIA: %s] (linha %d)\n", no->dados.leia.nome, no->linha);
            break;

        case NO_PARAMETRO:
            fprintf(stderr, "[PARAMETRO: %s %s] (linha %d)\n", no->dados.parametro.tipo_dado, no->dados.parametro.nome, no->linha);
            break;

        // GRUPO B: nós de expressão e comandos simples
        case NO_OP_BINARIA:
            fprintf(stderr, "[OP_BINARIA: %s] (linha %d)\n", no->dados.op_binaria.op, no->linha);
            ast_imprimir(no->dados.op_binaria.esq, nivel + 1);
            ast_imprimir(no->dados.op_binaria.dir, nivel + 1);
            break;

        case NO_ATRIBUICAO:
            fprintf(stderr, "[ATRIBUICAO: %s] (linha %d)\n", no->dados.atribuicao.nome, no->linha);
            ast_imprimir(no->dados.atribuicao.expressao, nivel + 1);
            break;

        case NO_ESCREVA: 
            fprintf(stderr, "[ESCREVA] (linha %d)\n", no->linha);
            ast_imprimir(no->dados.escreva.expressao, nivel + 1);
            break;
        
        case NO_RETORNE:
            fprintf(stderr, "[RETORNE] (linha %d)\n", no->linha);
            ast_imprimir(no->dados.retorne.expressao, nivel + 1);
            break;

        case NO_CHAMADA:
            fprintf(stderr, "[CHAMADA: %s] (linha %d)\n", no->dados.chamada.nome, no->linha);
            ast_imprimir(no->dados.chamada.argumentos, nivel + 1);
            break;

        // GRUPO C: estruturas de controle e funcoes (se, enquanto, para, funcao)
        case NO_SE:
            fprintf(stderr, "[SE] (linha %d)\n", no->linha);
            ast_imprimir(no->dados.se.condicao, nivel + 1);
            ast_imprimir(no->dados.se.entao, nivel + 1);
            // Regra de negócio: o bloco 'senao' é opcional (pode ser NULL)
            if (no->dados.se.senao != NULL) {
                imprimir_recuo(nivel);
                fprintf(stderr, "[SENAO] (linha %d)\n", no->linha);
                ast_imprimir(no->dados.se.senao, nivel + 1);
            }
            break;

        case NO_ENQUANTO:
            fprintf(stderr, "[ENQUANTO] (linha %d)\n", no->linha);
            ast_imprimir(no->dados.enquanto.condicao, nivel + 1);
            ast_imprimir(no->dados.enquanto.entao, nivel + 1);
            break;

        case NO_PARA:
            // Siga o mesmo padrão do NO_ENQUANTO:
            fprintf(stderr, "[PARA] (linha %d)\n", no->linha);
            ast_imprimir(no->dados.para.inicializa, nivel + 1);
            ast_imprimir(no->dados.para.condicao, nivel + 1);
            ast_imprimir(no->dados.para.passo, nivel + 1);
            ast_imprimir(no->dados.para.entao, nivel + 1);
            break;

        case NO_FUNCAO:
            fprintf(stderr, "[FUNCAO: %s -> %s] (linha %d)\n", no->dados.funcao.nome, no->dados.funcao.tipo_retorno, no->linha);
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
static int eh_zero(AstNode *n) {
    if (n == NULL) return 0;
    return (n->tipo == NO_INTEIRO && n->dados.inteiro.valor == 0) ||
           (n->tipo == NO_REAL && n->dados.real.valor == 0.0f);
}

static int eh_um(AstNode *n) {
    if (n == NULL) return 0;
    return (n->tipo == NO_INTEIRO && n->dados.inteiro.valor == 1) ||
           (n->tipo == NO_REAL && n->dados.real.valor == 1.0f);
}

static int tem_chamada(AstNode *n) {
    if (n == NULL) return 0;
    if (n->tipo == NO_CHAMADA) return 1;
    if (n->tipo == NO_OP_BINARIA)
        return tem_chamada(n->dados.op_binaria.esq) || tem_chamada(n->dados.op_binaria.dir);
    return 0;
}

static AstNode *substituir_por_filho(AstNode *no, AstNode *manter, AstNode *descartar) {
    liberar_ast(descartar);
    free(no->dados.op_binaria.op);
    free(no);
    return manter;
}

AstNode *otimizar_ast(AstNode *no) {
    if (no == NULL) return NULL;

    switch (no->tipo) {
        case NO_BLOCO:
            no->dados.bloco.comando = otimizar_ast(no->dados.bloco.comando);
            no->dados.bloco.proximo = otimizar_ast(no->dados.bloco.proximo);
            return no;

        case NO_ATRIBUICAO:
            no->dados.atribuicao.expressao = otimizar_ast(no->dados.atribuicao.expressao);
            return no;

        case NO_ESCREVA:
            no->dados.escreva.expressao = otimizar_ast(no->dados.escreva.expressao);
            return no;

        case NO_RETORNE:
            no->dados.retorne.expressao = otimizar_ast(no->dados.retorne.expressao);
            return no;

        case NO_SE:
            no->dados.se.condicao = otimizar_ast(no->dados.se.condicao);
            no->dados.se.entao = otimizar_ast(no->dados.se.entao);
            no->dados.se.senao = otimizar_ast(no->dados.se.senao);
            return no;

        case NO_ENQUANTO:
            no->dados.enquanto.condicao = otimizar_ast(no->dados.enquanto.condicao);
            no->dados.enquanto.entao = otimizar_ast(no->dados.enquanto.entao);
            return no;

        case NO_PARA:
            no->dados.para.inicializa = otimizar_ast(no->dados.para.inicializa);
            no->dados.para.condicao = otimizar_ast(no->dados.para.condicao);
            no->dados.para.passo = otimizar_ast(no->dados.para.passo);
            no->dados.para.entao = otimizar_ast(no->dados.para.entao);
            return no;

        case NO_FUNCAO:
            no->dados.funcao.corpo = otimizar_ast(no->dados.funcao.corpo);
            return no;

        case NO_CHAMADA:
            no->dados.chamada.argumentos = otimizar_ast(no->dados.chamada.argumentos);
            return no;

        case NO_OP_BINARIA: {
            no->dados.op_binaria.esq = otimizar_ast(no->dados.op_binaria.esq);
            no->dados.op_binaria.dir = otimizar_ast(no->dados.op_binaria.dir);

            AstNode *esq = no->dados.op_binaria.esq;
            AstNode *dir = no->dados.op_binaria.dir;
            const char *op = no->dados.op_binaria.op;

            // Operador unário 'nao' (esq == NULL)
            if (esq == NULL) {
                if (strcmp(op, "nao") == 0 && dir && dir->tipo == NO_LOGICO) {
                    AstNode *novo = criar_no_logico(!dir->dados.logico.valor, no->linha);
                    liberar_ast(no);
                    return novo;
                }
                return no;
            }

            // 1. Dobragem de constantes: inteiro com inteiro
            if (esq->tipo == NO_INTEIRO && dir->tipo == NO_INTEIRO) {
                int a = esq->dados.inteiro.valor;
                int b = dir->dados.inteiro.valor;
                AstNode *novo = NULL;

                if (strcmp(op, "+") == 0) novo = criar_no_inteiro(a + b, no->linha);
                else if (strcmp(op, "-") == 0) novo = criar_no_inteiro(a - b, no->linha);
                else if (strcmp(op, "*") == 0) novo = criar_no_inteiro(a * b, no->linha);
                else if (strcmp(op, "/") == 0 && b != 0) novo = criar_no_inteiro(a / b, no->linha);
                else if (strcmp(op, "%") == 0 && b != 0) novo = criar_no_inteiro(a % b, no->linha);
                else if (strcmp(op, "==") == 0) novo = criar_no_logico(a == b, no->linha);
                else if (strcmp(op, "!=") == 0) novo = criar_no_logico(a != b, no->linha);
                else if (strcmp(op, "<") == 0)  novo = criar_no_logico(a < b, no->linha);
                else if (strcmp(op, ">") == 0)  novo = criar_no_logico(a > b, no->linha);
                else if (strcmp(op, "<=") == 0) novo = criar_no_logico(a <= b, no->linha);
                else if (strcmp(op, ">=") == 0) novo = criar_no_logico(a >= b, no->linha);

                if (novo != NULL) {
                    liberar_ast(no);
                    return novo;
                }
            }

            if ((esq->tipo == NO_INTEIRO || esq->tipo == NO_REAL) &&
                (dir->tipo == NO_INTEIRO || dir->tipo == NO_REAL)) {
                float a = (esq->tipo == NO_REAL) ? esq->dados.real.valor : (float)esq->dados.inteiro.valor;
                float b = (dir->tipo == NO_REAL) ? dir->dados.real.valor : (float)dir->dados.inteiro.valor;
                AstNode *novo = NULL;

                if (strcmp(op, "+") == 0) novo = criar_no_real(a + b, no->linha);
                else if (strcmp(op, "-") == 0) novo = criar_no_real(a - b, no->linha);
                else if (strcmp(op, "*") == 0) novo = criar_no_real(a * b, no->linha);
                else if (strcmp(op, "/") == 0 && b != 0.0f) novo = criar_no_real(a / b, no->linha);

                if (novo != NULL) {
                    liberar_ast(no);
                    return novo;
                }
            }

            if (esq->tipo == NO_LOGICO && dir->tipo == NO_LOGICO) {
                int a = esq->dados.logico.valor;
                int b = dir->dados.logico.valor;
                AstNode *novo = NULL;

                if (strcmp(op, "e") == 0)       novo = criar_no_logico(a && b, no->linha);
                else if (strcmp(op, "ou") == 0) novo = criar_no_logico(a || b, no->linha);

                if (novo != NULL) {
                    liberar_ast(no);
                    return novo;
                }
            }

            // 4. Identidades algébricas simples: x + 0, 0 + x, x - 0, x * 1, 1 * x, x / 1, x * 0, 0 * x
            if (strcmp(op, "+") == 0) {
                if (eh_zero(dir)) return substituir_por_filho(no, esq, dir); // x + 0 -> x
                if (eh_zero(esq)) return substituir_por_filho(no, dir, esq); // 0 + x -> x
            } else if (strcmp(op, "-") == 0) {
                if (eh_zero(dir)) return substituir_por_filho(no, esq, dir); // x - 0 -> x
            } else if (strcmp(op, "*") == 0) {
                if (eh_um(dir))   return substituir_por_filho(no, esq, dir); // x * 1 -> x
                if (eh_um(esq))   return substituir_por_filho(no, dir, esq); // 1 * x -> x
                if (eh_zero(dir) && !tem_chamada(esq)) return substituir_por_filho(no, dir, esq); // x * 0 -> 0
                if (eh_zero(esq) && !tem_chamada(dir)) return substituir_por_filho(no, esq, dir); // 0 * x -> 0
            } else if (strcmp(op, "/") == 0) {
                if (eh_um(dir))   return substituir_por_filho(no, esq, dir); // x / 1 -> x
            }

            return no;
        }

        default:
            return no;
    }
}