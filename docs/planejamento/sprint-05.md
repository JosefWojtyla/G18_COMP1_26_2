# Sprint 5

## Objetivos

- ✅Concluir a análise semântica, tratando coerência de tipos em funções e expressões complexas.

- ✅Desenvolver o gerador de código intermediário (código de 3 endereços) percorrendo a AST.

- ✅Preparar a apresentação focada na evolução estrutural e preencher o formulário.

- 🔲Apresentação e entrega do **P2 em 11/11**.

## Entregas

- Análise semântica (`src/semantica.c`): inferência de tipos, verificação de atribuição, condições lógicas, operadores, chamadas de função (existência e nº de argumentos) e retorno garantido.
- Gerador de código de três endereços (`src/codigo_intermediario.c`) percorrendo a AST: expressões com temporários, `se/senao`, `enquanto`, `para`, funções (`begin/end/param/call/return`) e E/S. Impressão via `imprime_tac()`.
- Documentação em `docs/compilador/codigo-intermediario.md` e `analise-semantica.md`.
- Novos testes: `test/semantica/` (7 casos, erro + mensagem) e `test/tac/` (5 casos, golden files), com alvos `make test-sem`, `make test-tac` e `make test-all`. Catálogo em `TEST_CASES.md`.

## Dificuldades

- Acesso a vetor (`v[i]`) ainda não é traduzido para TAC (aviso e ignorado); fica como pendência para a Sprint 6.


## Acompanhamento

|Issue|Atividade|Descrição|Responsáveis|
|-|Semântica|Verificação de tipos, escopo, chamadas e retorno|Grupo|
|-|Código intermediário|Gerador de TAC a partir da AST|Grupo|
|-|Testes|Suítes `test/semantica` e `test/tac` + alvos no Makefile|Grupo|
|-|-|-|-|
