## Definições 2/9/2026

#### Escopo do trabalho 
- tipos de variáveis utilizados no trabalho: `real` e `cadeia`
## Definições da linguagem Portugol:
- *Definição de sintaxe e semantica base do Portugol:*
  * **Tipos utilizados:** `real`, `cadeia`, `inteiro`, `caractere` e `logico`.
  * **Delimitadores:** instruções não terminam com ponto e vírgula (`;`). Os blocos de códigos são definidos entre chaves (`{}`). Após utilizar funções reservadas, é necessário usar o parênteses `()` e adicionar o argumento dentro dele.
  * **Semântica:** é proibido fazer operações matemáticas com váriaveis que não seja do tipo `inteiro` ou `real` com do tipo `caractere`. Variáveis devem ser declaradas antes de serem utilizadas.
    
- *Tokens principais:*
  * Palavras-chave: `real`, `cadeia`, `se`, `senao`, `enquanto`, `escreva` e `leia`.
  * Operadores: `+`, `-`, `=`, `==`, `<`, `>`.
  * Símbolos: `{}`, `()`, `:=`.
 
- *Gramática Formal:*
  PROGRAMA -> DECLARA INÍCIO DO PROGRA
  DECLARACOES -> DECLARA AS VARIAVEIS
  TIPO -> TIPOS DAS VARIAVEIS
  COMANDOS -> INSERE O CÓDIGO
  FIMPROGRAMA -> FINALIZA O BLOCO TOTAL DO PROGRAMA
  
  
### Definicçao de descrição de linguagem

## Definição do código final (26/11/2026)
- **Alvo do compilador: Python.** O pipeline é Portugol -> AST -> semântica -> TAC -> otimizações -> Python; não há etapa de assembly.
- Motivo: o README já define Python como linguagem-alvo e dá para entregar a tempo; o Python gerado roda em qualquer máquina e permite conferir a saída dos programas nos testes ponta a ponta.
- O código final é gerado por `src/codigo_final.c` a partir da lista de instruções do TAC. Uso: `build/portugol programa.pt -o saida.py`; sem `-o`, o Python vai para a saída padrão.
