%{
#include <stdio.h>
#include <stdlib.h>

// Declaração de funções obrigatórias para o Bison
int yylex(void);
void yyerror(const char *s);

// Variável global do Flex para ler arquivos
extern FILE *yyin;
extern char *yytext;
%}

/* 1. Definição da estrutura yylval */
%union {
    char* str;
    int inteiro;
    float real;
}

/* 2. Declaração de todos os Tokens que aparecem no seu .l */
%token PROGRAMA INCLUA CONST CADEIA CARACTER T_INTEIRO T_REAL T_LOGICO
%token ESCREVA LEIA FUNCAO PROCEDIMENTO RETORNE VERDADEIRO FALSO
%token E OU NAO SE ENTAO SENAO FIMSE ENQUANTO FACA FIMENQUANTO
%token PARA ATE FIMPARA ATRIBUICAO IGUAL DIFERENTE MENOR_IGUAL
%token MAIOR_IGUAL MENOR MAIOR MAIS MENOS VEZES DIVIDE MODULO
%token ABRE_PAR FECHA_PAR VIRGULA PONTO_VIRGULA DOIS_PONTOS

/* Tokens com tipos associados à união */
%token <str> STRING IDENTIFICADOR
%token <real> REAL
%token <inteiro> INTEIRO

%%

/* 3. Regra gramatical minimalista apenas para consumir os tokens */
input:
    /* vazio */
    | input token
    ;

token:
    PROGRAMA        { printf("Token: PROGRAMA | Texto: '%s'\n", yytext); }

    | INCLUA        { printf("Token: INCLUA | Texto: '%s'\n", yytext); }
    | CONST         { printf("Token: CONST | Texto: '%s'\n", yytext); }
    | CADEIA        { printf("Token: CADEIA | Texto: '%s'\n", yytext); }
    | CARACTER      { printf("Token: CARACTER | Texto: '%s'\n", yytext); }
    | T_INTEIRO     { printf("Token: T_INTEIRO | Texto: '%s'\n", yytext); }
    | T_REAL        { printf("Token: T_REAL | Texto: '%s'\n", yytext); }
    | T_LOGICO      { printf("Token: T_LOGICO | Texto: '%s'\n", yytext); }
    | ESCREVA       { printf("Token: ESCREVA | Texto: '%s'\n", yytext); }
    | LEIA          { printf("Token: LEIA | Texto: '%s'\n", yytext); }
    | FUNCAO        { printf("Token: FUNCAO | Texto: '%s'\n", yytext); }
    | PROCEDIMENTO  { printf("Token: PROCEDIMENTO | Texto: '%s'\n", yytext); }
    | RETORNE       { printf("Token: RETORNE | Texto: '%s'\n", yytext); }
    | VERDADEIRO    { printf("Token: VERDADEIRO | Texto: '%s'\n", yytext); }
    | FALSO         { printf("Token: FALSO | Texto: '%s'\n", yytext); }
    | E             { printf("Token: E | Texto: '%s'\n", yytext); }
    | OU            { printf("Token: OU | Texto: '%s'\n", yytext); }
    | NAO           { printf("Token: NAO | Texto: '%s'\n", yytext); }
    | SE            { printf("Token: SE | Texto: '%s'\n", yytext); }
    | ENTAO         { printf("Token: ENTAO | Texto: '%s'\n", yytext); }
    | SENAO         { printf("Token: SENAO | Texto: '%s'\n", yytext); }
    | FIMSE         { printf("Token: FIMSE | Texto: '%s'\n", yytext); }
    | ENQUANTO      { printf("Token: ENQUANTO | Texto: '%s'\n", yytext); }
    | FACA          { printf("Token: FACA | Texto: '%s'\n", yytext); }
    | FIMENQUANTO   { printf("Token: FIMENQUANTO | Texto: '%s'\n", yytext); }
    | PARA          { printf("Token: PARA | Texto: '%s'\n", yytext); }
    | ATE           { printf("Token: ATE | Texto: '%s'\n", yytext); }
    | FIMPARA       { printf("Token: FIMPARA | Texto: '%s'\n", yytext); }
    | ATRIBUICAO    { printf("Token: ATRIBUICAO | Texto: '%s'\n", yytext); }
    | IGUAL         { printf("Token: IGUAL | Texto: '%s'\n", yytext); }
    | DIFERENTE     { printf("Token: DIFERENTE | Texto: '%s'\n", yytext); }
    | MENOR_IGUAL   { printf("Token: MENOR_IGUAL | Texto: '%s'\n", yytext); }
    | MAIOR_IGUAL   { printf("Token: MAIOR_IGUAL | Texto: '%s'\n", yytext); }
    | MENOR         { printf("Token: MENOR | Texto: '%s'\n", yytext); }
    | MAIOR         { printf("Token: MAIOR | Texto: '%s'\n", yytext); }
    | MAIS          { printf("Token: MAIS | Texto: '%s'\n", yytext); }
    | MENOS         { printf("Token: MENOS | Texto: '%s'\n", yytext); }
    | VEZES         { printf("Token: VEZES | Texto: '%s'\n", yytext); }
    | DIVIDE        { printf("Token: DIVIDE | Texto: '%s'\n", yytext); }
    | MODULO        { printf("Token: MODULO | Texto: '%s'\n", yytext); }
    | ABRE_PAR      { printf("Token: ABRE_PAR | Texto: '%s'\n", yytext); }
    | FECHA_PAR     { printf("Token: FECHA_PAR | Texto: '%s'\n", yytext); }
    | VIRGULA       { printf("Token: VIRGULA | Texto: '%s'\n", yytext); }
    | PONTO_VIRGULA { printf("Token: PONTO_VIRGULA | Texto: '%s'\n", yytext); }
    | DOIS_PONTOS   { printf("Token: DOIS_PONTOS | Texto: '%s'\n", yytext); }
    | STRING        { printf("Token: STRING | Valor: '%s'\n", $1); }
    | REAL          { printf("Token: REAL | Valor: '%.2f'\n", $1); }
    | INTEIRO       { printf("Token: INTEIRO | Valor: '%d'\n", $1); }
    | IDENTIFICADOR { printf("Token: IDENTIFICADOR | Texto: '%s'\n", $1); }
    ;

%%

/* 4. Função principal para rodar o teste */
int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            perror("Erro ao abrir arquivo de teste");
            return 1;
        }
        yyin = file;
    } else {
        printf("Digite o código Portugol (Pressione Ctrl+D para encerrar):\n");
    }

    // Executa o analisador (que chama o yylex por baixo dos panos)
    yyparse();
    return 0;
}

// Tratamento de erros sintáticos simples
void yyerror(const char *s) {
    fprintf(stderr, "Erro de sintaxe próximo a '%s': %s\n", yytext, s);
}
