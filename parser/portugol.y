%{
/* Seção de Prologue ( Declara ções C/C++) */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simbolos.h"
#include "../src/ast.h"

AstNode *raiz_ast = NULL; 

TabelaSimbolos *tabela;

void yyerror(const char *s); /* função de erro */
extern int yylex(void);        /* função do Flex */
extern int yylineno;            /* linha atual (do Flex) */
extern FILE *yyin;

static AstNode *adicionar_ao_bloco(AstNode *bloco, AstNode *comando, int linha) {
    AstNode *atual;

    if (comando == NULL) return bloco;
    if (bloco == NULL) return criar_no_bloco(comando, NULL, linha);

    atual = bloco;
    while (atual->dados.bloco.proximo != NULL) {
        atual = atual->dados.bloco.proximo;
    }
    atual->dados.bloco.proximo = criar_no_bloco(comando, NULL, linha);
    return bloco;
}

static AstNode *concatenar_blocos(AstNode *primeiro, AstNode *segundo) {
    AstNode *atual;

    if (primeiro == NULL) return segundo;
    if (segundo == NULL) return primeiro;

    atual = primeiro;
    while (atual->dados.bloco.proximo != NULL) {
        atual = atual->dados.bloco.proximo;
    }
    atual->dados.bloco.proximo = segundo;
    return primeiro;
}

%}

%code requires {
#include "../src/ast.h"
}

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
%token CARACTER
%token PROCEDIMENTO


%token <inteiro> INTEIRO
%token <real> REAL
%token <str> STRING IDENTIFICADOR

%type <no>  programa declaracoes_globais lista_funcoes funcao_inicio
%type <no>  declaracao_funcao bloco comando declaracao
%type <no>  expressao se_comando enquanto_comando lista_parametros
%type <no>  chamada_funcao lista_argumentos acesso_vetor
%type <no>  inicializacao_para incremento_para para_comando
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
    : %empty { $$ = NULL; }
    | declaracoes_globais declaracao { $$ = adicionar_ao_bloco($1, $2, yylineno); }
    ;

programa
    : PROGRAMA ABRE_CHAVE
      declaracoes_globais
      lista_funcoes
      funcao_inicio
      lista_funcoes
      FECHA_CHAVE
      {
          AstNode *nos = concatenar_blocos($3, $4);
          nos = adicionar_ao_bloco(nos, $5, yylineno);
          raiz_ast = concatenar_blocos(nos, $6);
          $$ = raiz_ast;
      }
    ;

lista_funcoes
    : %empty { $$ = NULL; }
    | lista_funcoes declaracao_funcao { $$ = adicionar_ao_bloco($1, $2, yylineno); }
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
          $$ = $7;
      }
    ;


bloco
    : %empty { $$ = NULL; }
    | bloco comando
        {
            $$ = adicionar_ao_bloco($1, $2, yylineno);
        }
    ;



comando
    : declaracao { $$ = $1; }
    | IDENTIFICADOR ATRIBUICAO expressao 
        {
            $$ = criar_no_atribuicao($1, $3, yylineno);
            free($1);
        }
    | ESCREVA ABRE_PAR expressao FECHA_PAR 
        { $$ = criar_no_escreva($3, yylineno); }
    | LEIA ABRE_PAR IDENTIFICADOR FECHA_PAR 
        { $$ = criar_no_leia($3, yylineno); free($3); }
    | se_comando
        { $$ = $1; }
    | enquanto_comando
        { $$ = $1; }
    | RETORNE expressao
        { $$ = criar_no_retorne($2, yylineno); }
    | chamada_funcao
        { $$ = $1; }
    | acesso_vetor ATRIBUICAO expressao
    {
        $$ = NULL; /* A AST ainda não possui nó de atribuição/acesso a vetor. */
    }
    | para_comando
        { $$ = $1; }
    ;


declaracao
    : tipo IDENTIFICADOR
        {   
            if(!inserirSimbolo(tabela, $2, $1, yylineno, tabela->escopo_atual)){
                yyerror("Variável já declarada neste escopo");
            }
            free($1);
            free($2);
            $$ = NULL;
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
            $$ = criar_no_atribuicao($2, $4, yylineno);
            free($1);
            free($2);
        }
    | tipo IDENTIFICADOR ABRE_COLCH INTEIRO FECHA_COLCH
        {
            free($1);
            free($2);
            $$ = NULL; /* Não há nó de declaração de vetor no design atual. */
        }
    ;

declaracao_funcao
    : FUNCAO tipo IDENTIFICADOR
      ABRE_PAR FECHA_PAR
      ABRE_CHAVE bloco FECHA_CHAVE
        {
            $$ = criar_no_funcao($3, $2, NULL, $7, yylineno);
            free($2);
            free($3);
        }
    | FUNCAO tipo IDENTIFICADOR
      ABRE_PAR lista_parametros FECHA_PAR
      ABRE_CHAVE bloco FECHA_CHAVE
        {
            $$ = criar_no_funcao($3, $2, $5, $8, yylineno);
            free($2);
            free($3);
        }
    ;

tipo
    : T_INTEIRO     { $$ = strdup("inteiro"); }
    | T_REAL        { $$ = strdup("real"); }
    | T_CADEIA      { $$ = strdup("cadeia"); }
    | T_LOGICO      { $$ = strdup("logico"); }
    ;

enquanto_comando
    : ENQUANTO ABRE_PAR expressao FECHA_PAR FACA ABRE_CHAVE bloco FECHA_CHAVE
        {
            $$ = criar_no_enquanto($3, $7, yylineno);
        }
    ;

se_comando
    : SE ABRE_PAR expressao FECHA_PAR ENTAO ABRE_CHAVE bloco FECHA_CHAVE
        {
            $$ = criar_no_se($3, $7, NULL, yylineno);
        }
    | SE ABRE_PAR expressao FECHA_PAR ENTAO ABRE_CHAVE bloco FECHA_CHAVE SENAO ABRE_CHAVE bloco FECHA_CHAVE
        {
            $$ = criar_no_se($3, $7, $11, yylineno);
        }
    ;

lista_parametros
    : tipo IDENTIFICADOR
        {
            free($1);
            free($2);
            $$ = NULL;
        }
    | lista_parametros VIRGULA tipo IDENTIFICADOR
        {
            free($3);
            free($4);
            $$ = $1;
        }
    ;



expressao
    : INTEIRO
        { $$ = criar_no_inteiro($1, yylineno); }
    | REAL
        { $$ = criar_no_real($1, yylineno); }
    | STRING
        { $$ = criar_no_cadeia($1, yylineno); free($1); }
    | IDENTIFICADOR
        { $$ = criar_no_variavel($1, yylineno); free($1); }
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
        { $$ = criar_no_op_binaria("%", $1, $3, yylineno); }
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
        { $$ = criar_no_op_binaria("e", $1, $3, yylineno); }
    | expressao OU expressao
        { $$ = criar_no_op_binaria("ou", $1, $3, yylineno); }
    | NAO expressao
        { $$ = criar_no_op_binaria("nao", NULL, $2, yylineno); }
    | chamada_funcao
        { $$ = $1; }
    | acesso_vetor
        { $$ = NULL; /* A AST ainda não representa acesso a vetor. */ }
    ;

chamada_funcao
    : IDENTIFICADOR ABRE_PAR FECHA_PAR
        {
            $$ = criar_no_chamada($1, NULL, yylineno);
            free($1);
        }
    | IDENTIFICADOR ABRE_PAR lista_argumentos FECHA_PAR
        {
            $$ = criar_no_chamada($1, $3, yylineno);
            free($1);
        }
    ;

lista_argumentos
    : expressao { $$ = adicionar_ao_bloco(NULL, $1, yylineno); }
    | lista_argumentos VIRGULA expressao
        { $$ = adicionar_ao_bloco($1, $3, yylineno); }
    ;

acesso_vetor
    : IDENTIFICADOR ABRE_COLCH expressao FECHA_COLCH
        {
            free($1);
            liberar_ast($3);
            $$ = NULL; /* A AST ainda não possui nó de acesso a vetor. */
        }
    ;

inicializacao_para
    : tipo IDENTIFICADOR ATRIBUICAO expressao
        {
            $$ = criar_no_atribuicao($2, $4, yylineno);
            free($1);
            free($2);
        }
    | IDENTIFICADOR ATRIBUICAO expressao
        {
            $$ = criar_no_atribuicao($1, $3, yylineno);
            free($1);
        }
    ;

incremento_para
    : IDENTIFICADOR MAIS_MAIS
        {
            $$ = criar_no_atribuicao($1, criar_no_op_binaria("+", criar_no_variavel($1, yylineno), criar_no_inteiro(1, yylineno), yylineno), yylineno);
            free($1);
        }
    | IDENTIFICADOR MENOS_MENOS
        {
            $$ = criar_no_atribuicao($1, criar_no_op_binaria("-", criar_no_variavel($1, yylineno), criar_no_inteiro(1, yylineno), yylineno), yylineno);
            free($1);
        }
    | IDENTIFICADOR MAIS_IGUAL expressao
        {
            $$ = criar_no_atribuicao($1, criar_no_op_binaria("+", criar_no_variavel($1, yylineno), $3, yylineno), yylineno);
            free($1);
        }
    | IDENTIFICADOR MENOS_IGUAL expressao
        {
            $$ = criar_no_atribuicao($1, criar_no_op_binaria("-", criar_no_variavel($1, yylineno), $3, yylineno), yylineno);
            free($1);
        }
    | IDENTIFICADOR VEZES_IGUAL expressao
        {
            $$ = criar_no_atribuicao($1, criar_no_op_binaria("*", criar_no_variavel($1, yylineno), $3, yylineno), yylineno);
            free($1);
        }
    | IDENTIFICADOR DIVIDE_IGUAL expressao
        {
            $$ = criar_no_atribuicao($1, criar_no_op_binaria("/", criar_no_variavel($1, yylineno), $3, yylineno), yylineno);
            free($1);
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
            $$ = criar_no_para($3, $5, $10, $7, yylineno);
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
