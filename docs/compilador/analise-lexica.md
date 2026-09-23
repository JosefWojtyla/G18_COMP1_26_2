# analise lexica


### Objetivo
Transformar o código-fonte (arquivo `.pt`) em uma sequência de **tokens** (unidades léxicas com significado).
 
### Processo
```
Entrada: inteiro x = 5
           ↓
        [Flex reconhece padrões]
           ↓
Saída: INTEIRO | IDENT(x) | ATRIB(=) | NUM_INT(5)
```
 
### Implementação (Flex)
 
**Arquivo:** `lexer/portugol.l`
 
#### Palavras-chave Suportadas
```flex
"inteiro"       { return T_INTEIRO; }
"real"          { return T_REAL; }
"cadeia"        { return CADEIA; }
"logico"        { return T_LOGICO; }
"escreva"       { return ESCREVA; }
"leia"          { return LEIA; }
"se"            { return SE; }
"entao"         { return ENTAO; }
"senao"         { return SENAO; }
"enquanto"      { return ENQUANTO; }
"faca"          { return FACA; }
"para"          { return PARA; }
"ate"           { return ATE; }
"funcao"        { return FUNCAO; }
"inicio"        { return INICIO; }
"retorne"       { return RETORNE; }
"verdadeiro"    { return VERDADEIRO; }
"falso"         { return FALSO; }
"e"             { return E; }
"ou"            { return OU; }
"nao"           { return NAO; }
```
 
#### Operadores
```flex
"<-"|":="       { return ATRIBUICAO; }
"=="            { return IGUAL; }
"!="            { return DIFERENTE; }
"<="            { return MENOR_IGUAL; }
">="            { return MAIOR_IGUAL; }
"<"             { return MENOR; }
">"             { return MAIOR; }
"+"             { return MAIS; }
"-"             { return MENOS; }
"*"             { return VEZES; }
"/"             { return DIVIDE; }
"%"             { return MODULO; }
"++"            { return MAIS_MAIS; }
"--"            { return MENOS_MENOS; }
"+="            { return MAIS_IGUAL; }
"-="            { return MENOS_IGUAL; }
"*="            { return VEZES_IGUAL; }
"/="            { return DIVIDE_IGUAL; }
```
 
#### Delimitadores
```flex
"("             { return ABRE_PAR; }
")"             { return FECHA_PAR; }
"{"             { return ABRE_CHAVE; }
"}"             { return FECHA_CHAVE; }
"["             { return ABRE_COLCH; }
"]"             { return FECHA_COLCH; }
","             { return VIRGULA; }
";"             { return PONTO_VIRGULA; }
":"             { return DOIS_PONTOS; }
"programa"      { return PROGRAMA; }
```
 
#### Literais e Identificadores
```flex
\"[^\"]*\"                      { yylval.str = strdup(yytext); return STRING; }
[0-9]+\.[0-9]+                 { yylval.real = atof(yytext); return REAL; }
[0-9]+                          { yylval.inteiro = atoi(yytext); return INTEIRO; }
[a-zA-Z_àáâãäåèéêëìíîïòóôõöûüçÀÁÂÃÄÅÈÉÊËÌÍÎÏÒÓÔÕÖÛÜÇ][a-zA-Z0-9_àáâãäåèéêëìíîïòóôõöûüçÀÁÂÃÄÅÈÉÊËÌÍÎÏÒÓÔÕÖÛÜÇ]* { yylval.str = strdup(yytext); return IDENTIFICADOR; }
```
 
#### Ignorar
```flex
"//".*                          { /* comentário de linha */ }
"{"[^}]*"}"                     { /* comentário de bloco */ }
[ \t]                           { /* espaço e tab */ }
\n                              { /* yylineno já conta */ }
.                               { fprintf(stderr, "Erro léxico: '%s' linha %d\n", yytext, yylineno); }
```
 
### Características
- Suporta identificadores com acentos (padrão português)
- Números inteiros e reais (com ponto)
- Strings entre aspas duplas
- Comentários: `//` (linha) e `/* */` (bloco)
- Mensagens de erro com número da linha
