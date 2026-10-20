%{
/* Seção de Prologue ( Declara ções C/C++) */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simbolos.h"

TabelaSimbolos *tabela;

void yyerror(const char *s); /* função de erro */
extern int yylex(void);        /* função do Flex */
extern int yylineno;            /* linha atual (do Flex) */
extern FILE *yyin;

%}

/* Seção de Declara ções do Bison */

%union {
    int inteiro;
    float real;
    char *str;
}


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


%type <str> tipo


/* (opcional por enquanto define a ordem) */
%left OU
%left E
%left IGUAL DIFERENTE MENOR MAIOR MENOR_IGUAL MAIOR_IGUAL
%left MAIS MENOS
%left VEZES DIVIDE MODULO
%right NAO

%start programa

%% 

 /* Seção de Regras Gramaticais */

declaracoes_globais
    : /* vazio */
    | declaracoes_globais declaracao
    ;
programa
    : PROGRAMA ABRE_CHAVE declaracoes_globais FUNCAO INICIO ABRE_PAR FECHA_PAR ABRE_CHAVE
    {
        tabela->escopo_atual++;
    } 
    bloco FECHA_CHAVE 
    {
        tabela->escopo_atual--;
    }
    FECHA_CHAVE



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
    : declaracao 
        {
            printf(" Declaração\n");
        }
    | IDENTIFICADOR ATRIBUICAO expressao 
        {
            printf(" Atribuição a %s\n", $1);
        }
    | ESCREVA ABRE_PAR expressao FECHA_PAR 
        {
            printf(" escreva()\n");
        }
    | LEIA ABRE_PAR IDENTIFICADOR FECHA_PAR 
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
            if(!inserirSimbolo(tabela, $2, $1, yylineno, tabela->escopo_atual)){
                yyerror("Variável já declarada neste escopo");
            }         
        }

    | tipo IDENTIFICADOR ATRIBUICAO expressao
        {
            
            if(!inserirSimbolo(tabela, $2, $1, yylineno, tabela->escopo_atual)){
                yyerror("Variável já declarada neste escopo");
            } else {
                Simbolo *s = buscarSimbolo(tabela, $2);

                if(s != NULL){
                    s->inicializado = 1;
                }
            }
        }
    ;


tipo
    : T_INTEIRO     { $$ = "inteiro"; }
    | T_REAL        { $$ = "real"; }
    | T_CADEIA      { $$ = "cadeia"; }
    | T_LOGICO      { $$ = "logico"; }
    ;

enquanto_comando
    : ENQUANTO ABRE_PAR expressao FECHA_PAR FACA ABRE_CHAVE bloco FECHA_CHAVE
        {
            printf(" enquanto\n");
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
            printf("(inteiro: %d) ", $1);
        }
    | REAL
        {
            printf("(real: %.2f) ", $1);
        }
    | STRING
        {
            printf("(string: %s) ", $1);
        }
    | IDENTIFICADOR
        {
            printf("(var: %s) ", $1);
        }
    | VERDADEIRO
        {
            printf("(verdadeiro) ");
        }
    | FALSO
        {
            printf("(falso) ");
        }
    | ABRE_PAR expressao FECHA_PAR
        {
            printf("(expr) ");
        }
    | expressao MAIS expressao
        {
            printf("(+) ");
        }
    | expressao MENOS expressao
        {
            printf("(-) ");
        }
    | expressao VEZES expressao
        {
            printf("(*) ");
        }
    | expressao DIVIDE expressao
        {
            printf("(/) ");
        }
    | expressao MODULO expressao
        {
            printf("(%%) ");
        }
    | expressao IGUAL expressao
        {
            printf("(==) ");
        }
    | expressao DIFERENTE expressao
        {
            printf("(!=) ");
        }
    | expressao MENOR expressao
        {
            printf("(<) ");
        }
    | expressao MAIOR expressao
        {
            printf("(>) ");
        }
    | expressao MENOR_IGUAL expressao
        {
            printf("(<=) ");
        }
    | expressao MAIOR_IGUAL expressao
        {
            printf("(>=) ");
        }
    | expressao E expressao
        {
            printf("(e) ");
        }
    | expressao OU expressao
        {
            printf("(ou) ");
        }
    | NAO expressao
        {
            printf("(nao) ");
        }
    ;


/* Ainda falta coisa pra adicionar*/

%%

/* SEÇÃO 4: CÓDIGO C */

int qtd_erros = 0;

void yyerror(const char *s) {
    fprintf(stderr, "Erro na linha %d: %s\n", yylineno, s);
    qtd_erros++;

}

int main(int argc, char *argv[]) {
    tabela = malloc(sizeof(TabelaSimbolos));
    criarTabelaSimbolos(tabela);

    if (argc > 1) {
        FILE *f = fopen(argv[1], "r");
        if (!f) {
            perror("Erro ao abrir arquivo");
            limparTabelaSimbolos(tabela);
            free(tabela);
            
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
        printf("\n = = = Tabela de Simbolos = = =\n");

        for(int i = 0; i < tabela->count; i++){
            printf("Nome: %-10s | Tipo: %-8s | Escopo: %d | Linha: %d | Inicializado: %d\n",
            tabela->simbolos[i]->nome,
            tabela->simbolos[i]->tipo,
            tabela->simbolos[i]->escopo,
            tabela->simbolos[i]->linha_declaracao,
            tabela->simbolos[i]->inicializado);
        }
    }

    if(qtd_erros){

        limparTabelaSimbolos(tabela);
        free(tabela);
        
        return qtd_erros;
    }

    limparTabelaSimbolos(tabela);
    free(tabela);

    return resultado;
    
}
