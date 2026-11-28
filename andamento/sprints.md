# Sprints do Projeto
---
## Sprint 1
- **Objetivos Principais:**
  - [ ] Organizar a equipe e configurar repositório.
  - [ ] Definir a linguagem-fonte do compilador (sintaxe e semântica básica).
  - [ ] Elaborar a primeira versão da gramática formal (livre de contexto).
- **Principais Entregas:**
  - [ ] Documento inicial descrevendo a linguagem (tokens, estruturas, exemplos de código).
  - [ ] Protótipo de gramática reconhecida pelo Bison (ainda não funcional, mas já planejada).
  - [ ] Ambiente configurado e testado (cada membro deve conseguir compilar e rodar um “hello world” do Flex/Bison).
  - [ ] Documento definindo o escopo do projeto.
- **Tarefas e Atividades:**
  - **Definir o escopo do compilador**: quais construções essenciais da linguagem serão suportadas
  - Configurar repositório (GitHub ou similar) e adicionar todos os membros (incluindo o professor: “sergioaafreitas” ou “sergiofreitas@unb.br”).
  - **Configurar Ferramentas:** Flex, Bison, compilador C/C++ (ou outra linguagem) no ambiente local.
  - Criar gramática inicial no Bison (arquivo .y) e o arquivo de regras léxicas no Flex (arquivo .l), ainda que incompletos.

---
## Sprint 2 a 5
- Ver `docs/planejamento/sprint-02.md` a `sprint-05.md` e os pontos de controle em `docs/entregas/`.

---
## Sprint 6 (10/11 a 23/11)
- **Objetivos Principais:**
  - [x] Otimizações básicas: remoção de código morto (#50) e simplificação de expressões (#51).
  - [x] Tradução do código intermediário para Python (#53).
  - [x] Testes integrados de ponta a ponta (#54).
- **Principais Entregas:**
  - [x] `otimizar_ast` (AST) e `otimiza_tac` (TAC).
  - [x] `src/codigo_final.c` com máquina de estados `_pc`.
  - [x] Suíte `test/e2e/` com 21 programas.
- **Dificuldades:** ausência de `goto` em Python, tipos `real` perdidos no TAC, divisão inteira, variáveis globais dentro de funções.

---
## Sprint 7 (24/11 a 30/11)
- **Objetivos Principais:**
  - [x] Code freeze e correções finais.
  - [x] Documentação final (README, código final, entrega final).
  - [ ] Ensaio das entrevistas individuais.
  - [ ] Entrega final e entrevistas em 30/11.
- **Principais Entregas:**
  - [x] `make test`: 66 casos, 66 PASS.
  - [ ] Formulário enviado e professor com acesso ao repositório.
