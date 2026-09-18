%{
/* Seção de Prologue ( Declara ções C/C++) */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void yyerror(const char *s);
extern int yylex(void);
extern int yylineno;

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

%token <inteiro> INTEIRO
%token <real> REAL
%token <str> STRING IDENTIFICADOR


%union {
    int inteiro;
    float real;
    char *str;
}


/* (opcional por enquanto) */
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

 