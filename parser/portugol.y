%{
/* Seção de Prologue ( Declara ções C/C++) */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simbolos.h"

AstNode *raiz_ast = NULL; 

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
    AstNode *no;
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

%type <no>  programa bloco comando declaracao atribuicao
%type <no>  expressao se_comando enquanto_comando
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
    : PROGRAMA ABRE_CHAVE
      declaracoes_globais
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
      ABRE_CHAVE 
      {
          tabela->escopo_atual++;
      }
      bloco FECHA_CHAVE
      {
          tabela->escopo_atual--;
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
        { $$ = criar_no_inteiro($1, yylineno); }
    | REAL
        { $$ = criar_no_real($1, yylineno); }
    | STRING
        { $$ = criar_no_cadeia($1, yylineno); }
    | IDENTIFICADOR
        { $$ = criar_no_variavel($1, yylineno); }
    | VERDADEIRO
        { $$ = criar_no_logico(1, yylineno); }
    | FALSO
        { $$ = criar_no_logico(0, yylineno); }
    | ABRE_PAR expressao FECHA_PAR
        { $$ = $2; }
    | expressao MAIS expressao
        { $$ = criar_no_op_binaria("+", $1, $3, yylineno); }
    | expressao MENOS expressao
        { $$ = criar_no_op_binaria("-", $1, $3, yylineno); }
    | expressao VEZES expressao
        { $$ = criar_no_op_binaria("*", $1, $3, yylineno); }
    | expressao DIVIDE expressao
        { $$ = criar_no_op_binaria("/", $1, $3, yylineno); }
    | expressao MODULO expressao
        { $$ = criar_no_op_binaria("%%", $1, $3, yylineno); }
    | expressao IGUAL expressao
        { $$ = criar_no_op_binaria("==", $1, $3, yylineno); }
    | expressao DIFERENTE expressao
        { $$ = criar_no_op_binaria("!=", $1, $3, yylineno); }
    | expressao MENOR expressao
        { $$ = criar_no_op_binaria("<", $1, $3, yylineno); }
    | expressao MAIOR expressao
        { $$ = criar_no_op_binaria(">", $1, $3, yylineno); }
    | expressao MENOR_IGUAL expressao
        { $$ = criar_no_op_binaria("<=", $1, $3, yylineno); }
    | expressao MAIOR_IGUAL expressao
        { $$ = criar_no_op_binaria(">=", $1, $3, yylineno); }
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
