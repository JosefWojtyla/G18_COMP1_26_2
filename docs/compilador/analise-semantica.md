# Análise semântica

**Status:** implementada na Sprint 5 (`src/semantica.c`, `src/simbolos.c`). Validada por `make test-sem` (`test/semantica/`) e pelos testes de erro em `test/error/`.

## 1. Posição no pipeline

```
Léxica → Sintática (AST) → Semântica → Otimização da AST → Código intermediário (TAC) → Python
```

A verificação ocorre durante o parse (ações do Bison), usando a tabela de símbolos com escopos (nível 0 = global, 1+ = local). Erros incrementam um contador; o compilador termina com código != 0 e **não gera TAC nem Python** se houver erro.

## 2. Verificações implementadas

| Verificação | Função | Mensagem |
|:---|:---|:---|
| Variável não declarada | `inferir_tipo` | `Variavel 'x' nao foi declarada` |
| Redeclaração no mesmo escopo | `simbolos.c` / parser | `Variável já declarada neste escopo` |
| Função não declarada | `inferir_tipo` | `Funcao 'f' nao declarada` |
| Número de argumentos e tipos na chamada | `inferir_tipo` | `Numero incorreto de argumentos ...` |
| Operandos aritméticos devem ser `inteiro`/`real` | `inferir_tipo` | `esperava tipos numericos` |
| Operadores lógicos (`e`, `ou`, `nao`) exigem `logico` | `inferir_tipo` | `espera tipo 'logico'` |
| Compatibilidade na atribuição (`inteiro` → `real` permitido) | `verificar_atribuicao` | `Tipo incompativel na atribuicao` |
| Condição de `se`/`enquanto`/`para` deve ser `logico` | `verificar_condicao` | `Condicao invalida` |
| Retorno garantido em todos os caminhos | `verificar_retorno_garantido` | `Funcao nao possui retorno garantido ...` |

## 3. Inferência de tipos

`inferir_tipo(no, tabela)` devolve o tipo (`"inteiro"`, `"real"`, `"cadeia"`, `"logico"`) de uma expressão, percorrendo a AST recursivamente. Retorna `NULL` quando o tipo não pode ser determinado (por exemplo, após um erro já reportado), evitando erros em cascata.

## 4. Testes

Cada caso em `test/semantica/*.pt` tem um `.msg` com a mensagem esperada. Ver catálogo TC-S01..S07 em `TEST_CASES.md`.

```bash
make test-sem    # 7 casos, 7 PASS
make test-base   # inclui os 10 casos de test/error/, todos rejeitados corretamente
```

## 5. Limitações

- Vetores: a declaração e o acesso (`v[i]`) são aceitos pelo parser, mas não entram na AST; por isso não há verificação de tipo/índice nem tradução para TAC e Python (decisão da equipe).
- A coluna `Linha` da tabela de símbolos pode aparecer deslocada em uma linha, porque `yylineno` é lido depois que o parser já consumiu o token seguinte. Não afeta as mensagens de erro semântico.
- O uso de variável sem inicialização gera apenas um aviso, não um erro.
