# Sprint 6

**Período:** 10/11 a 23/11 (estendida até 27/11)

## Objetivos

- ✅Projetar e implementar otimizações básicas: remoção de código morto no TAC e simplificação de expressões na AST.

- ✅Traduzir o código intermediário para a linguagem-alvo. A equipe manteve **Python** como alvo (em vez de assembly).

- ✅Executar testes integrados passando programas completos pelo compilador (`make test-e2e`).

## Entregas

- `otimizar_ast` em `src/ast.c`: dobragem de constantes e identidades algébricas.
- `otimiza_tac` em `src/codigo_intermediario.c`: remoção de código inalcançável e de temporários mortos.
- `src/codigo_final.c`: tradução TAC → Python, com máquina de estados (`_pc`) para os saltos.
- Opção `-o` no compilador e alvos `make run` / `make debug`.
- Suíte `test/e2e/` (21 casos) e casos `tac06_codigo_morto` e `tac07_simplificacao` com o TAC antes/depois.

## Dificuldades

| Dificuldade | Solução |
|---|---|
| Python não tem `goto` e o TAC é feito de saltos | Cada função com saltos vira um `while True` com o contador `_pc`; cada rótulo inicia um bloco `if _pc == k` |
| O TAC não guarda tipos: `real r = 7` virava `int` no Python e `r / 2` dava `3` | O código final consulta a tabela de símbolos e aplica `float(...)` em atribuições, parâmetros e retornos `real` |
| Divisão inteira do Portugol × divisão real do Python | Função auxiliar `_div` no Python gerado |
| Atribuir a uma global dentro de uma função criava uma local no Python | O gerador escreve `global x` na `def` |
| Remover código morto sem apagar efeitos colaterais | Só operações puras sobre temporários são removidas; `x * 0` só é simplificado se `x` não tiver chamada de função |
| `parser/portugol.tab.c` versionado ficava desatualizado em relação ao `.y` | Rodar `make clean && make` para regenerar com o Bison |

## Acompanhamento

|Issue|Atividade|Descrição|Responsáveis|
|-|-|-|-|
|#50|Remoção de código morto|Código inalcançável e temporários mortos no TAC (PR #55)|Josef|
|#51|Simplificação de expressões|Dobragem de constantes e identidades na AST (PR #55)|Josef|
|#53|Código final|Tradução TAC → Python, tipos, globais e `-o` (PRs #59, #60)|Zayra|
|#54|Testes integrados|Suíte ponta a ponta `test/e2e/` e retirada dos testes de vetor (PR #61)|Josef|
