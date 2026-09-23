# analise sintatica


### Objetivo
Verificar se a sequência de tokens segue as **regras gramaticais** da linguagem.
 
### Processo
```
Entrada: INTEIRO | IDENT(x) | ATRIB(=) | NUM_INT(5)
          ↓
     [Bison valida com gramática]
          ↓
Saída: Árvore Sintática (AST) ou erro
```
 
### Implementação (Bison)
 
**Arquivo:** `parser/portugol.y`
 
#### Símbolos Iniciais
```bison
%start programa
 
programa
    : PROGRAMA ABRE_CHAVE FUNCAO INICIO ABRE_PAR FECHA_PAR ABRE_CHAVE bloco FECHA_CHAVE FECHA_CHAVE
    ;
```
 
#### Estrutura de Blocos
```bison
bloco
    : /* vazio */
    | bloco comando
    ;
 
comando
    : declaracao 
    | atribuicao 
    | ESCREVA ABRE_PAR lista_expressoes FECHA_PAR 
    | LEIA ABRE_PAR IDENTIFICADOR FECHA_PAR 
    | se_comando
    | enquanto_comando
    | para_comando
    | chamada_funcao 
    | RETORNE expressao 
    ;
```
 
#### Declaração de Variáveis
```bison
declaracao
    : tipo IDENTIFICADOR
    | tipo IDENTIFICADOR ATRIBUICAO expressao
    | tipo IDENTIFICADOR ABRE_COLCH NUM_INT FECHA_COLCH
    ;
 
tipo
    : T_INTEIRO
    | T_REAL
    | CADEIA
    | T_LOGICO
    ;
```
 
#### Atribuição
```bison
atribuicao
    : IDENTIFICADOR ATRIBUICAO expressao
    | IDENTIFICADOR ABRE_COLCH expressao FECHA_COLCH ATRIBUICAO expressao
    | IDENTIFICADOR MAIS_MAIS
    | IDENTIFICADOR MENOS_MENOS
    | IDENTIFICADOR MAIS_IGUAL expressao
    | IDENTIFICADOR MENOS_IGUAL expressao
    | IDENTIFICADOR VEZES_IGUAL expressao
    | IDENTIFICADOR DIVIDE_IGUAL expressao
    ;
```
 
#### Estruturas de Controle
```bison
se_comando
    : SE ABRE_PAR expressao FECHA_PAR ENTAO ABRE_CHAVE bloco FECHA_CHAVE
    | SE ABRE_PAR expressao FECHA_PAR ENTAO ABRE_CHAVE bloco FECHA_CHAVE SENAO ABRE_CHAVE bloco FECHA_CHAVE
    ;
 
enquanto_comando
    : ENQUANTO ABRE_PAR expressao FECHA_PAR FACA ABRE_CHAVE bloco FECHA_CHAVE
    ;
 
para_comando
    : PARA ABRE_PAR tipo IDENTIFICADOR ATRIBUICAO expressao PONTO_VIRGULA expressao PONTO_VIRGULA atribuicao FECHA_PAR ABRE_CHAVE bloco FECHA_CHAVE
    ;
```
 
#### Expressões
```bison
expressao
    : termo
    | expressao MAIS expressao
    | expressao MENOS expressao
    | expressao VEZES expressao
    | expressao DIVIDE expressao
    | expressao MODULO expressao
    | expressao IGUAL expressao
    | expressao DIFERENTE expressao
    | expressao MENOR expressao
    | expressao MAIOR expressao
    | expressao MENOR_IGUAL expressao
    | expressao MAIOR_IGUAL expressao
    | expressao E expressao
    | expressao OU expressao
    | NAO expressao
    | ABRE_PAR expressao FECHA_PAR
    ;
 
termo
    : INTEIRO
    | REAL
    | STRING
    | VERDADEIRO
    | FALSO
    | IDENTIFICADOR
    | IDENTIFICADOR ABRE_COLCH expressao FECHA_COLCH
    | chamada_funcao
    ;
```
 
#### Funções
```bison
declaracao_funcao
    : FUNCAO tipo IDENTIFICADOR ABRE_PAR lista_parametros FECHA_PAR ABRE_CHAVE bloco FECHA_CHAVE
    | FUNCAO tipo IDENTIFICADOR ABRE_PAR FECHA_PAR ABRE_CHAVE bloco FECHA_CHAVE
    ;
 
lista_parametros
    : tipo IDENTIFICADOR
    | lista_parametros VIRGULA tipo IDENTIFICADOR
    ;
 
chamada_funcao
    : IDENTIFICADOR ABRE_PAR FECHA_PAR
    | IDENTIFICADOR ABRE_PAR lista_argumentos FECHA_PAR
    ;
 
lista_argumentos
    : expressao
    | lista_argumentos VIRGULA expressao
    ;
```
 
### Precedência de Operadores
```bison
%left OU
%left E
%left IGUAL DIFERENTE
%left MENOR MAIOR MENOR_IGUAL MAIOR_IGUAL
%left MAIS MENOS
%left VEZES DIVIDE MODULO
%right NAO
```
 
### Características
- Suporta aninhamento de blocos (`se` dentro de `se`, etc)
- Funções com parâmetros tipados
- Expressões aritméticas, lógicas e relacionais
- Linguagem sem ; no final
<!-- - Vetores unidimensionais (ainda não na sprint 2) -->