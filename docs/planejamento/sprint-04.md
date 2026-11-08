# Sprint 4

## Objetivos

- ✅Projetar e construir a AST (Árvore Sintática Abstrata).

- ✅Iniciar a verificação de escopo (variáveis não declaradas ou redeclaradas).

- ✅Implementar a verificação de tipos simples.

## Entregas

- **AST** (`src/ast.c` / `src/ast.h`): enum `TipoNo` com os 16 tipos de nó, `union` de dados por tipo, construtores `criar_no_*`, destrutor `liberar_ast`/`ast_liberar` e impressão indentada `ast_imprimir`.
- **Integração com o parser**: as regras de `parser/portugol.y` passaram a construir nós da AST (`criar_no_*`) em vez de só imprimir rastros de depuração; a raiz do programa é acumulada em `raiz_ast`.
- **Análise semântica** (`src/semantica.c`): `inferir_tipo` (percorre a AST e resolve o tipo de constantes, variáveis, chamadas de função e operações binárias), `verificar_atribuicao` (compatibilidade de tipos na atribuição, com promoção `inteiro` → `real`), `verificar_condicao` (exige `logico` em `se`, `enquanto` e `para`) e `verificar_retorno_garantido` (toda função precisa retornar em todos os caminhos).
- **Verificação de escopo**: uso de variável e de função não declaradas é detectado em `inferir_tipo`, reaproveitando a tabela de símbolos da Sprint 3.
- **Verificação de funções**: número e tipo dos argumentos de uma chamada são conferidos contra os parâmetros declarados.
- **Testes**: novos casos de erro em `test/error/` (`teste_err05_argumentos_funcao`, `teste_err06_tipo_retorno`, `teste_err07_sem_retorno`).

## Dificuldades


## Acompanhamento

|Pull Request|Atividade|Descrição|Responsáveis|
|-|-|-|-|
|[#33](https://github.com/JosefWojtyla/Grupo18_COMPILADORES_T01_2026_2/pull/33)|Design da AST|Criação de `src/ast.c`/`src/ast.h`: os 16 tipos de nó, união de dados, construtores e teste inicial (`test_ast.c`).|[Josef](https://github.com/JosefWojtyla)|
|[#34](https://github.com/JosefWojtyla/Grupo18_COMPILADORES_T01_2026_2/pull/34)|Construção da AST no parser|Integração das regras de `portugol.y` com os construtores `criar_no_*`, substituindo os prints de depuração.|[Zayra](https://github.com/Zayra-Moraes)|
|[#35](https://github.com/JosefWojtyla/Grupo18_COMPILADORES_T01_2026_2/pull/35)|Utilitários da AST|`ast_imprimir` (impressão indentada) e `ast_liberar`/`liberar_ast`.|[Josef](https://github.com/JosefWojtyla)|
|[#44](https://github.com/JosefWojtyla/Grupo18_COMPILADORES_T01_2026_2/pull/44)|Utilitários da AST (continuação)|Ajustes adicionais em `ast.c`/`ast.h` e atualização da documentação da Sprint 3.|[Josef](https://github.com/JosefWojtyla)|
|[#45](https://github.com/JosefWojtyla/Grupo18_COMPILADORES_T01_2026_2/pull/45)|Análise semântica|Criação de `src/semantica.c` (`inferir_tipo`, `verificar_atribuicao`, `verificar_condicao`); `simbolos.c`/`.h` movidos para `src/`.|[Josef](https://github.com/JosefWojtyla)|
|[#46](https://github.com/JosefWojtyla/Grupo18_COMPILADORES_T01_2026_2/pull/46)|Semântica avançada (funções)|Verificação de argumentos e tipo de retorno em chamadas de função, `verificar_retorno_garantido` e novos testes de erro.|[Josef](https://github.com/JosefWojtyla)|

## Observações

- Assim como a AST começou a ser feita no fim da Sprint 3, parte do trabalho desta sprint (PRs #35, #44, #45 e #46) só foi mesclada já dentro do período da Sprint 5, por atraso.
- O tipo `caractere` continua sem decisão (ver `Decisões.md`): `semantica.c` ainda não trata esse caso.
- Os três novos testes de erro do PR #46 ainda não foram documentados no `TEST_CASES.md` — a catalogação formal dos testes da Sprint 4 fica para depois, se houver tempo.
- A validação semântica de vetores segue fora de escopo, por decisão da equipe (ver `docs/compilador/linguagem-e-escopo.md`).