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
%token ABRE_COLCH FECHA_COLCH
%token VIRGULA PONTO_VIRGULA DOIS_PONTOS
%token ATRIBUICAO
%token IGUAL DIFERENTE MENOR MAIOR MENOR_IGUAL MAIOR_IGUAL
%token MAIS MENOS VEZES DIVIDE MODULO
%token MAIS_MAIS MENOS_MENOS MAIS_IGUAL MENOS_IGUAL VEZES_IGUAL DIVIDE_IGUAL


/* ainda n tem definição na seção debaixo*/
%token INCLUA
%token CONST
%token CADEIA
%token CARACTER
%token PROCEDIMENTO


%token <inteiro> INTEIRO
%token <real> REAL
%token <str> STRING IDENTIFICADOR





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

programa
    : PROGRAMA ABRE_CHAVE
      lista_funcoes
      funcao_inicio
      lista_funcoes
      FECHA_CHAVE
    ;

lista_funcoes
    : /* vazio */
    | lista_funcoes declaracao_funcao
    ;

funcao_inicio
    : FUNCAO INICIO ABRE_PAR FECHA_PAR
      ABRE_CHAVE bloco FECHA_CHAVE
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
    | RETORNE expressao
        {
            printf("Retorno de função reconhecido\n");
        }
    | chamada_funcao
        {
            printf("Chamada usada como comando\n");
        }
    | error PONTO_VIRGULA
        {
            yyerror("Comando inválido");
            yyerrok;
        }
    | acesso_vetor ATRIBUICAO expressao
    {
        printf("Atribuição em posição de vetor\n");
    }
    | para_comando
    {
        printf("Comando para\n");
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
    | tipo IDENTIFICADOR ABRE_COLCH INTEIRO FECHA_COLCH
        {
            printf("Vetor %s de tamanho %d declarado\n", $2, $4);
        }
    ;

declaracao_funcao
    : FUNCAO tipo IDENTIFICADOR
      ABRE_PAR FECHA_PAR
      ABRE_CHAVE bloco FECHA_CHAVE
        {
            printf("Função %s sem parâmetros reconhecida\n", $3);
        }
    | FUNCAO tipo IDENTIFICADOR
      ABRE_PAR lista_parametros FECHA_PAR
      ABRE_CHAVE bloco FECHA_CHAVE
        {
            printf("Função %s com parâmetros reconhecida\n", $3);
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

lista_parametros
    : tipo IDENTIFICADOR
        {
            printf("Parâmetro %s reconhecido\n", $2);
        }
    | lista_parametros VIRGULA tipo IDENTIFICADOR
        {
            printf("Parâmetro %s reconhecido\n", $4);
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
    | chamada_funcao
        {
            printf("Chamada usada como expressão\n");
        }
    | acesso_vetor
        {
            printf("Vetor usado em expressão\n");
        }
    ;

chamada_funcao
    : IDENTIFICADOR ABRE_PAR FECHA_PAR
        {
            printf("Chamada da função %s sem argumentos\n", $1);
        }
    | IDENTIFICADOR ABRE_PAR lista_argumentos FECHA_PAR
        {
            printf("Chamada da função %s com argumentos\n", $1);
        }
    ;

lista_argumentos
    : expressao
    | lista_argumentos VIRGULA expressao
    ;

acesso_vetor
    : IDENTIFICADOR ABRE_COLCH expressao FECHA_COLCH
        {
            printf("Acesso ao vetor %s\n", $1);
        }
    ;

inicializacao_para
    : tipo IDENTIFICADOR ATRIBUICAO expressao
        {
            printf("Inicialização do para com declaração de %s\n", $2);
        }
    | IDENTIFICADOR ATRIBUICAO expressao
        {
            printf("Inicialização do para com variável %s\n", $1);
        }
    ;

incremento_para
    : IDENTIFICADOR MAIS_MAIS
        {
            printf("Incremento ++ em %s\n", $1);
        }
    | IDENTIFICADOR MENOS_MENOS
        {
            printf("Incremento -- em %s\n", $1);
        }
    | IDENTIFICADOR MAIS_IGUAL expressao
        {
            printf("Incremento += em %s\n", $1);
        }
    | IDENTIFICADOR MENOS_IGUAL expressao
        {
            printf("Incremento -= em %s\n", $1);
        }
    | IDENTIFICADOR VEZES_IGUAL expressao
        {
            printf("Incremento *= em %s\n", $1);
        }
    | IDENTIFICADOR DIVIDE_IGUAL expressao
        {
            printf("Incremento /= em %s\n", $1);
        }
    ;

para_comando
    : PARA ABRE_PAR
      inicializacao_para PONTO_VIRGULA
      expressao PONTO_VIRGULA
      incremento_para
      FECHA_PAR
      ABRE_CHAVE bloco FECHA_CHAVE
        {
            printf("Laço para reconhecido\n");
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

    if(qtd_erros){
        return qtd_erros;
    }

    return resultado;
}

