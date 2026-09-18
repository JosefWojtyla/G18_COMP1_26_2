%{
#include "portugol.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

%}

%option noyywrap
%option yylineno

DIGITO      [0-9]
LETRA       [a-z,A-Zà-úÀ-Ú_]
IDENTIFICADOR          {LETRA}({LETRA}|{DIGITO})*
NUM_INT     {DIGITO}+
NUM_REAL    {DIGITO}+"."?{DIGITO}+
ESPACO      [ \t\r\n]+  


%%

"//".*          {/*comentario de linha so ignora*/}
"/*"([^*]|\*+[^*/])*\*+"/"     {/*comentario de bloco so ignora tbm*/}

"programa"                  { return PROGRAMA;}
"inclua"                    {return INCLUA;}
"const"                     {return CONST;}
"cadeia"                    {return CADEIA;}
"caracter"                  {return CARACTER;}
"inteiro"                   { return T_INTEIRO; }
"real"                      { return T_REAL; }
"logico"                    { return T_LOGICO; }

"escreva"                   { return ESCREVA; }
"leia"                      { return LEIA; }
"funcao"                    { return FUNCAO; }
"procedimento"              { return PROCEDIMENTO; }
"retorne"                  { return RETORNE; }

"verdadeiro"               { return VERDADEIRO; }
"falso"                    { return FALSO; }
"e"                        { return E; }
"ou"                       { return OU; }
"nao"                      { return NAO; }
"se"                       { return SE; }
"entao"                    { return ENTAO; }
"senao"                    { return SENAO; }
"fimse"                    { return FIMSE; }
"enquanto"                 { return ENQUANTO; }
"faca"                     { return FACA; }
"fimenquanto"              { return FIMENQUANTO; }
"para"                     { return PARA; }
"ate"                      { return ATE; }
"fimpara"                  { return FIMPARA; }

"=="                       { return IGUAL; }
"="                  { return ATRIBUICAO; }
"!="                       { return DIFERENTE; }
"<="                       { return MENOR_IGUAL; }
">="                       { return MAIOR_IGUAL; }
"<"                        { return MENOR; }
">"                        { return MAIOR; }
"+"                        { return MAIS; }
"-"                        { return MENOS; }
"*"                        { return VEZES; }
"/"                        { return DIVIDE; }
"%"                        { return MODULO; }
"("                        { return ABRE_PAR; }
")"                        { return FECHA_PAR; }
","                        { return VIRGULA; }
";"                        { return PONTO_VIRGULA; }
":"                        { return DOIS_PONTOS; }

\"[^\"]*\"                 { yylval.str = strdup(yytext); return STRING; }
{NUM_INT}                  { yylval.inteiro = atoi(yytext); return INTEIRO; }
{NUM_REAL}                 { yylval.real = atof(yytext); return REAL; }
{IDENTIFICADOR}                       { yylval.str = strdup(yytext); return IDENTIFICADOR; }

{ESPACO}                   { /* ignora espaços */ }
\n                         { /* yylineno já conta a linha */ }

.                          { fprintf(stderr, "Erro léxico: caractere inválido '%s' na linha %d\n", yytext, yylineno); }

%%






