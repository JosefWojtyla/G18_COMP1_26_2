# Sprint 3

## Objetivos

- ✅Estender as regras no Bison para cobrir todas as estruturas de controle e declarações da linguagem.

- ✅Definir a estrutura de dados da Tabela de Símbolos.

- ✅Implementar o preenchimento da Tabela de Símbolos durante a análise sintática.

## Entregas

- **Gramática expandida** (`parser/portugol.y`): funções definidas pelo usuário (com e sem parâmetros), vetores (declaração e acesso indexado), laço `para` com os passos `i++`, `i--`, `i += n`, `i -= n`, `i *= n` e `i /= n`, além de `retorne` com expressão.
- **Tabela de Símbolos** (`simbolos.h` / `simbolos.c`): struct `Simbolo` (nome, tipo, se é vetor, tamanho, linha de declaração, escopo, inicialização) e `TabelaSimbolos`, com as funções `criarTabelaSimbolos`, `inserirSimbolo`, `buscarSimbolo`, `verificarDuplicata` e `limparTabelaSimbolos`.
- **Integração com o parser**: cada declaração de variável é inserida na tabela com o escopo atual, e a redeclaração no mesmo escopo gera erro. A tabela é impressa ao final da execução.
- **Testes**: novos casos em `test/valid/` (funções, recursão, vetores, `para`, escopo global/local) e em `test/error/` (redeclaração, tamanho de vetor inválido), documentados em `TEST_CASES.md`. O `make test` roda a suíte inteira.
- **Teste unitário** da tabela de símbolos em `test_simbolos.c`.

## Dificuldades

- Definir como representar escopos (global x local) na tabela de forma simples, usando um nível numérico (`escopo_atual`).
- Garantir que a mesma variável possa existir em escopos diferentes sem acusar redeclaração, verificando a duplicata por nome **e** escopo.
- Ajustar conflitos da gramática ao adicionar funções, vetores e `para` (ponto e vírgula e vírgula nos parâmetros).

## Acompanhamento

|Pull Request|Atividade|Descrição|Responsáveis|
|-|-|-|-|
|[#24](https://github.com/JosefWojtyla/G18_COMP1_26_2/pull/24)|Estrutura da Tabela de Símbolos|Definição das structs `Simbolo` e `TabelaSimbolos` e das funções de criar, inserir, buscar e verificar duplicatas, com teste unitário e documentação de símbolos.|[Josef](https://github.com/JosefWojtyla)|
|[#25](https://github.com/JosefWojtyla/G18_COMP1_26_2/pull/25)|Expansão da gramática|Regras de função do usuário, vetores e laço `para` com incremento, no `portugol.y`.|[Zayra](https://github.com/Zayra-Moraes)|
|[#26](https://github.com/JosefWojtyla/G18_COMP1_26_2/pull/26)|Preenchimento da Tabela de Símbolos|Inserção dos símbolos durante o parse, controle de escopo e detecção de redeclaração.|[Josef](https://github.com/JosefWojtyla)|
|[#32](https://github.com/JosefWojtyla/G18_COMP1_26_2/pull/32)|Testes da Sprint 3|Casos válidos e de erro para funções, vetores, `para` e escopos, com o catálogo em `TEST_CASES.md`.|[Josef](https://github.com/JosefWojtyla)|

## Observações

- O design e a construção da AST (PRs [#33](https://github.com/JosefWojtyla/G18_COMP1_26_2/pull/33), [#34](https://github.com/JosefWojtyla/G18_COMP1_26_2/pull/34) e [#35](https://github.com/JosefWojtyla/G18_COMP1_26_2/pull/35)) começaram no fim da Sprint 3, mas fazem parte do escopo da Sprint 4 e serão registrados lá.
