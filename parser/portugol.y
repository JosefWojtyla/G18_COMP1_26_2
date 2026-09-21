%{
/* Seção de Prologue ( Declara ções C/C++) */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void yyerror(const char *s); /* função de erro */
extern int yylex(void);        /* função do Flex */
extern int yylineno;            /* linha atual (do Flex) */
extern FILE *yyin;

%}

/* Seção de Declara ções do Bison */

%token PROGRAMA FUNCAO INICIO
%token T_INTEIRO T_REAL T_CADEIA T_LOGICO
%token ESCREVA LEIA
%token SE ENTAO SENAO FIMSE
%token ENQUANTO FACA FIMENQUANTO
%token PARA ATE FIMPARA
%token RETORNE
%token VERDADEIRO FALSO
%token E OU NAO
%token ABRE_PAR FECHA_PAR ABRE_CHAVE FECHA_CHAVE
%token VIRGULA PONTO_VIRGULA DOIS_PONTOS
%token ATRIBUICAO
%token IGUAL DIFERENTE MENOR MAIOR MENOR_IGUAL MAIOR_IGUAL
%token MAIS MENOS VEZES DIVIDE MODULO
%token MAIS_MAIS MENOS_MENOS MAIS_IGUAL MENOS_IGUAL

/* ainda n tem definição na seção debaixo*/
%token INCLUA
%token CONST
%token CADEIA
%token CARACTER
%token PROCEDIMENTO


%token <inteiro> INTEIRO
%token <real> REAL
%token <str> STRING IDENTIFICADOR


%union {
    int inteiro;
    float real;
    char *str;
}


/* (opcional por enquanto define a ordem) */
%type <str> programa bloco comando declaracao expressao
%left OU
%left E
%left IGUAL DIFERENTE MENOR MAIOR MENOR_IGUAL MAIOR_IGUAL
%left MAIS MENOS
%left VEZES DIVIDE MODULO
%right NAO

%start programa

%% 

 /* Seção de Regras Gramaticais */

programa
    : PROGRAMA ABRE_CHAVE FUNCAO INICIO ABRE_PAR FECHA_PAR ABRE_CHAVE bloco FECHA_CHAVE FECHA_CHAVE
        {
            printf(" Programa reconhecido \n");
        }
    | error
        {
            yyerror("Erro na estrutura do programa");
        }
    ;



bloco
    : 
        {
            printf(" Bloco vazio\n");
        }
    | bloco comando
        {
            printf(" Comando adicionado ao bloco\n");
        }
    ;



comando
    : declaracao PONTO_VIRGULA
        {
            printf(" Declaração\n");
        }
    | IDENTIFICADOR ATRIBUICAO expressao PONTO_VIRGULA
        {
            printf(" Atribuição a %s\n", $1);
        }
    | ESCREVA ABRE_PAR expressao FECHA_PAR PONTO_VIRGULA
        {
            printf(" escreva()\n");
        }
    | LEIA ABRE_PAR IDENTIFICADOR FECHA_PAR PONTO_VIRGULA
        {
            printf(" leia(%s)\n", $3);
        }
    | se_comando
        {
            printf(" se/senao\n");
        }
    | enquanto_comando
        {
            printf(" enquanto\n");
        }
    | error PONTO_VIRGULA
        {
            yyerror("Comando inválido");
            yyerrok;
        }
    ;


declaracao
    : tipo IDENTIFICADOR
        {
            printf("      Declaração de %s\n", $2);
        }
    | tipo IDENTIFICADOR ATRIBUICAO expressao
        {
            printf("      Declaração de %s com atribuição\n", $2);
        }
    ;


tipo
    : T_INTEIRO     { printf("(tipo: inteiro) "); }
    | T_REAL        { printf("(tipo: real) "); }
    | T_CADEIA      { printf("(tipo: cadeia) "); }
    | T_LOGICO      { printf("(tipo: logico) "); }
    ;

enquanto_comando
    : ENQUANTO ABRE_PAR expressao FECHA_PAR FACA ABRE_CHAVE bloco FECHA_CHAVE
        {
        }
    ;

se_comando
    : SE ABRE_PAR expressao FECHA_PAR ENTAO ABRE_CHAVE bloco FECHA_CHAVE
        {
            printf(" se sem senao\n");
        }
    | SE ABRE_PAR expressao FECHA_PAR ENTAO ABRE_CHAVE bloco FECHA_CHAVE SENAO ABRE_CHAVE bloco FECHA_CHAVE
        {
            printf(" se com senao\n");
        }
    ;

expressao
    : INTEIRO
        {
            printf("inteiro:%d", $1);
        }

/* Ainda falta coisa pra adicionar*/

%%

/* SEÇÃO 4: CÓDIGO C */


void yyerror(const char *s) {
    fprintf(stderr, "Erro na linha %d: %s\n", yylineno, s);
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        FILE *f = fopen(argv[1], "r");
        if (!f) {
            perror("Erro ao abrir arquivo");
            return 1;
        }
        yyin = f;
    } else {
        yyin = stdin;
    }

    printf("=== PARSER PORTUGOL STUDIO UNIVALI ===\n\n");

    int resultado = yyparse();

    if (resultado == 0) {
        printf("\n\n Parse concluído com sucesso!\n");
    } else {
        printf("\n\n Erros encontrados durante o parse.\n");
    }

    if (argc > 1) {
        fclose(yyin);
    }

    return resultado;
}

