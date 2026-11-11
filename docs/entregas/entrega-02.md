# Ponto de controle 2

## Compilador Portugol → Python | Sprints 3, 4 e 5

---

### Número de Sprints Realizadas

**Sprints concluídas ou em andamento desde o P1:**

- **S3 (29/09 a 12/10)**: concluída (design da tabela de símbolos)
- **S4 (13/10 a 26/10)**: concluída em grande parte (AST e primeira semântica); a atualização do planejamento ainda está pendente
- **S5 (27/10 a 09/11)**: concluída (código intermediário, fechamento da semântica)

---

### Avanços desde o Ponto de Controle 1

**S3 - Alcançados:**

- ✅ Gramática expandida: funções definidas pelo usuário (com e sem parâmetros), vetores (declaração e acesso), laço `para` com `++`, `--`, `+=`, `-=`, `*=`, `/=` e `retorne` com expressão (PR #25)
- ✅ Tabela de Símbolos: structs `Simbolo` e `TabelaSimbolos`, funções de criar, inserir, buscar e verificar duplicata, com teste unitário (PR #24)
- ✅ Preenchimento da tabela durante o parse, com controle de escopo global/local e detecção de redeclaração (PR #26)
- ✅ Catálogo de testes em `TEST_CASES.md` com casos válidos e de erro (PR #32)

**S4 - Alcançados:**

- ✅ Design da AST com 17 tipos de nó (`TipoNo` em `src/ast.h`), construtores `criar_no_*`, impressão com indentação (`ast_imprimir`) e liberação de memória (PRs #33, #34, #35 e #44)
- ✅ Verificação semântica: variáveis não declaradas, tipos em atribuição, condições lógicas, operadores, chamadas de função e número de argumentos (`src/semantica.c`, PRs #45 e #46)
- ✅ Verificação de retorno garantido em funções com `se/senao`

**S5 - Em andamento:**

- ✅ Gerador de código de três endereços (`src/codigo_intermediario.c`), com impressão no formato `(op, arg1, arg2, resultado)`
- ✅ Documento de design do código intermediário (`docs/compilador/codigo-intermediario.md`)
- 🔲 Fechar as decisões pendentes do código intermediário (coerção `inteiro`/`real`, concatenação de cadeias, curto-circuito de `e`/`ou`)
- 🔲 Vetores na AST e no código intermediário (fora do escopo por decisão da equipe)

---

### Decisões Tomadas

| Decisão | Motivo |
|---|---|
| Vetores fora do escopo atual. O parser reconhece `tipo nome[n]` e `nome[i]`, mas a AST devolve `NULL` nesses pontos | Priorizar o núcleo (funções, escopo, semântica e TAC) antes de vetores |
| Duplicata verificada por nome **e** escopo | Permite o mesmo nome em escopos diferentes sem acusar redeclaração |
| Código intermediário em quádruplas `(op, arg1, arg2, resultado)` | Formato independente de máquina, fácil de gerar a partir da AST e de otimizar depois |
| Temporários e rótulos com contador global (`t0`, `t1`…, `L0`, `L1`…) | Nomes únicos em todo o programa |
| `=` é sempre atribuição no TAC, independente do símbolo da linguagem-fonte | Separa a representação intermediária da sintaxe do Portugol |
| `i++` e `i += n` viram operação binária com destino na variável | Uma única forma para todas as compostas |
| Laço `para` gera um rótulo de passo (`L_passo`) mesmo sem `continue` | Facilita a futura otimização e a inclusão de `continue` |
| `e`/`ou` sem curto-circuito por enquanto | Simplicidade; decisão ainda **pendente** |

---

### Árvore Sintática Abstrata (AST)

A AST é construída pelas ações semânticas do Bison em `parser/portugol.y`. Cada nó tem um tipo (`TipoNo`), a linha de origem e uma união com os dados do nó. Listas (parâmetros, argumentos, blocos) são encadeadas por nós `NO_BLOCO`.

**Nós:** `NO_INTEIRO`, `NO_REAL`, `NO_CADEIA`, `NO_LOGICO`, `NO_OP_BINARIA`, `NO_SE`, `NO_ENQUANTO`, `NO_PARA`, `NO_RETORNE`, `NO_ATRIBUICAO`, `NO_FUNCAO`, `NO_BLOCO`, `NO_VARIAVEL`, `NO_ESCREVA`, `NO_LEIA`, `NO_CHAMADA`, `NO_PARAMETRO`.

**Exemplo impresso pela `ast_imprimir`** (programa `x = 2 + 3 * y` seguido de `se (x > 10) { escreva(x) }`, montado pelos construtores):

```
[ATRIBUICAO: x] (linha 1)
  [OP_BINARIA: +] (linha 1)
    [INTEIRO: 2] (linha 1)
    [OP_BINARIA: *] (linha 1)
      [INTEIRO: 3] (linha 1)
      [VARIAVEL: y] (linha 1)
[SE] (linha 1)
  [OP_BINARIA: >] (linha 1)
    [VARIAVEL: x] (linha 1)
    [INTEIRO: 10] (linha 1)
  [ESCREVA] (linha 1)
    [VARIAVEL: x] (linha 1)
```

No `main` do compilador, a AST só é impressa com a opção `--ast`, e somente se não houver erros.

---

### Tabela de Símbolos 

Saída real do compilador (`./build/portugol arquivo.pt`) para o programa:

```
programa {
    inteiro x
    funcao inteiro somar(inteiro a, inteiro b) {
        inteiro x
        retorne a + b
    }
    funcao inicio() {
        escreva(x)
    }
}
```

```
= = = Tabela de Simbolos = = =
Nome: x          | Tipo: inteiro  | Escopo: 0 | Linha: 3 | Inicializado: 0
Nome: a          | Tipo: inteiro  | Escopo: 1 | Linha: 3 | Inicializado: 1
Nome: b          | Tipo: inteiro  | Escopo: 1 | Linha: 3 | Inicializado: 1
Nome: somar      | Tipo: inteiro  | Escopo: 0 | Linha: 3 | Inicializado: 0
Nome: x          | Tipo: inteiro  | Escopo: 1 | Linha: 5 | Inicializado: 0
```

O `x` global (escopo 0) e o `x` local de `somar` (escopo 1) aparecem como entradas separadas. Parâmetros são marcados como inicializados. O aviso `Variavel 'x' usada sem ser inicializada` aparece na linha 8, porque `escreva(x)` usa o `x` global, já que o escopo de `somar` foi fechado.

A recusa de redeclaração no mesmo escopo é coberta pelos casos `test/error/variavel_redeclarada.pt` e `test/error/teste_err04_redeclaracao.pt`, que passaram na suíte.

**Problema conhecido:** a coluna `Linha` está deslocada em uma linha. O `x` global está na linha 2 e aparece como 3; o `x` local está na linha 4 e aparece como 5. A causa é que `linha_declaracao` recebe `yylineno` no momento da redução da regra, depois que o parser lê o token seguinte. Ainda não corrigido.

---

### Análise Semântica

Implementada em `src/semantica.c` e  `src/simbolos.c`, chamada durante o parse (ações do Bison) com a tabela de símbolos e a AST. Cada erro incrementa `qtd_erros`: com erro semântico o compilador termina com código != 0 e **não gera TAC**. Detalhes em `docs/compilador/analise-semantica.md`.

**Verificações:**

| Verificação | Função | Mensagem |
|---|---|---|
| Variável não declarada | `inferir_tipo` | `Variavel 'x' nao foi declarada` |
| Redeclaração no mesmo escopo | `simbolos.c` | `Variável já declarada neste escopo` |
| Função não declarada | `inferir_tipo` | `Funcao 'f' nao declarada` |
| Número/tipo de argumentos na chamada | `inferir_tipo` | `Numero incorreto de argumentos ...` |
| Operandos aritméticos numéricos | `inferir_tipo` | `esperava tipos numericos` |
| Operadores lógicos exigem `logico` | `inferir_tipo` | `espera tipo 'logico'` |
| Atribuição compatível (`inteiro` → `real` permitido) | `verificar_atribuicao` | `Tipo incompativel na atribuicao` |
| Condição de `se`/`enquanto`/`para` lógica | `verificar_condicao` | `Condicao invalida` |
| Retorno garantido em todos os caminhos | `verificar_retorno_garantido` | `Funcao nao possui retorno garantido ...` |

**Inferência de tipos:** `inferir_tipo(no, tabela)` percorre a AST e devolve `inteiro`, `real`, `cadeia` ou `logico`. Retorna `NULL` quando o tipo é indeterminado (por exemplo, após erro já reportado), evitando erros em cascata.

**Saída real** para `inteiro a = "texto"` (`test/semantica/sem02_atribuicao_tipo.pt`):

```
Erro semantico na linha 4: Tipo incompativel na atribuicao. Esperado 'inteiro', mas encontrado 'cadeia'.
```

---

### Código Intermediário (Três Endereços)

Formato: `(op, arg1, arg2, resultado)`. Saída real do gerador (`gera_codigo` + `imprime_tac`) para as árvores dos exemplos da seção 7 de `codigo-intermediario.md`.

**Exemplo 1: atribuição e condicional**

```
x = 2 + 3 * y
se (x > 10) { escreva(x) }
```

```
(*, 3, y, t0)
(+, 2, t0, t1)
(=, t1, -, x)
(>, x, 10, t2)
(ifFalse, t2, -, L0)
(write, x, -, -)
(label, -, -, L0)
```

**Exemplo 2: laço `para`**

```
para (inteiro i = 0; i < 5; i++) { escreva(i) }
```

```
(=, 0, -, i)
(label, -, -, L2)
(<, i, 5, t3)
(ifFalse, t3, -, L4)
(write, i, -, -)
(label, -, -, L3)
(+, i, 1, t4)
(=, t4, -, i)
(goto, -, -, L2)
(label, -, -, L4)
```

---

### Testes

Os testes desta entrega foram executados e verificados nas seguintes frentes:

| Teste | Como foi feito | Resultado |
|---|---|---|
| Tabela de símbolos (unitário) | `test_simbolos.c` compilado com `simbolos.c` | ✅ Todos os asserts passaram |
| AST (unitário) | `test/test_ast.c` compilado com `ast.c` e `simbolos.c` | ✅ Nó criado e memória liberada |
| Impressão da AST | Árvore montada pelos construtores e impressa por `ast_imprimir` | ✅ Saída acima |
| Gerador de TAC | Árvores dos exemplos 1 e 2 processadas por `gera_codigo` | ✅ Saídas acima |
| Impressão da tabela de símbolos | Sequência de inserções do programa de exemplo | ✅ Saída acima |

**Resultado de `make test` (executado com o parser regenerado por Bison e Flex):**

| Suíte | Casos | Resultado |
|---|---|---|
| Base: `test/valid/` (testes que devem passar) | 25 | 25 PASS |
| Base: `test/error/` (testes que devem falhar) | 10 | 10 PASS |
| Semântica: `test/semantica/` (erro com mensagem esperada) | 7 | 7 PASS |
| Código intermediário: `test/tac/` (TAC comparado com o `.tac` esperado) | 5 | 5 PASS |
| **Total** | **47** | **47 PASS, 0 FAIL** |


Para rodar:

```
make clean
make test
```

---

### O Que Ainda Falta

**Para S6 (10/11 a 23/11) e entrega final:**

- Otimizações básicas do código intermediário
- Geração de código Python a partir do TAC
- Testes de ponta a ponta comparando a saída do TAC com os exemplos da documentação

**A S7 está reservada para imprevistos e revisões de código.**

---

### Uso de IA

- Foi usado para gerar referências didáticas principalmente no conteúdo de código intermediário.
- Foi usado o Claude para revisar a documentação e cruzar o código com os documentos de planejamento.

---

### Progresso

A análise sintática evoluiu para uma AST completa, com funções, `para` e `retorne`. A tabela de símbolos controla escopos e redeclarações. A análise semântica cobre tipos, condições, chamadas e retorno garantido. O gerador produz código de três endereços para atribuições, expressões, `se`, `enquanto`, `para`, chamadas e `escreva`. Vetores seguem fora do escopo. A principal pendência é rodar a suíte de ponta a ponta com a gramática regenerada.

---

**Data:** 10/11/2026
**Status:** Em andamento (S6)
**Próxima entrega:** Entrega final (30/11, Sprint 7)
