# Sprint 7

**Período:** 24/11 a 30/11

## Objetivos

- ✅Congelar o código (code freeze): só correções, sem funcionalidades novas.

- ✅Finalizar a documentação (README com manual de uso, código final, entrega final, revisão das páginas do compilador).

- 🔲Ensaiar para as entrevistas individuais (roteiro de 1 página por integrante e demonstração de 3 minutos).

- 🔲Entrevistas e **Entrega Final em 30/11**.

## Entregas

- README: dependências, compilação, uso, testes e exemplo completo.
- `docs/compilador/codigo-final.md` e `docs/entregas/entrega-final.md`.
- Revisão de `analise-semantica.md` e `codigo-intermediario.md`, com as otimizações.
- Suíte completa passando: 66 casos, 0 falhas.

## Dificuldades

| Dificuldade | Solução |
|---|---|
| Prazo curto entre o código final e a entrega | Issues pequenas e divididas entre as duas pessoas, com critérios de aceitação |
| Saídas `.out` dos testes geradas para programas sem saída e para vetores | Arquivos corrigidos e testes de vetor retirados (PR #61) |
| Páginas da documentação vazias ou desatualizadas | Revisão de todas as páginas de `docs/` contra o código |

## Roteiro da demonstração (3 minutos)

1. `make clean && make` (compila o compilador).
2. `build/portugol --tac test/valid/funcao_recursiva.pt -o fib.py` e mostrar o TAC.
3. Abrir `fib.py` e explicar o esquema do `_pc`.
4. `python3 fib.py` → `5`.
5. `make test` → 66 PASS.

## Acompanhamento

|Issue|Atividade|Descrição|Responsáveis|
|-|-|-|-|
|-|Documentação final|README, código final, entrega final, revisão de `docs/` e tabelas das sprints|Zayra|
|-|Entrega|Formulário, link e acesso do professor ao repositório|Josef e Zayra|
|-|Entrevistas|Roteiro individual e ensaio da demonstração|Josef e Zayra|
