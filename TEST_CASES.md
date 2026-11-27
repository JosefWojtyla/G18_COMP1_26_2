# Suíte de Testes do Compilador Portugol

Suíte de testes automatizados do compilador, cobrindo análise léxica, sintática, semântica, código intermediário (TAC), otimizações e execução ponta a ponta (E2E) do Python gerado.

---

## 1. Estrutura da Suíte

Os testes estão organizados em cinco pastas dentro de `test/`:

- **`test/valid/`**: Casos de teste sintática e semanticamente corretos. O compilador deve aceitar e retornar código `0` (`make test-base`).
- **`test/error/`**: Casos com erros léxicos, sintáticos ou semânticos propositais. O compilador deve rejeitar e retornar código `!= 0` (`make test-base`).
- **`test/semantica/`**: Erros semânticos com arquivo `.msg` contendo a mensagem esperada em `stderr` (`make test-sem`).
- **`test/tac/`**: Testes de código intermediário com golden files `.tac` e `.antes.tac` para otimizações (`make test-tac`).
- **`test/e2e/`**: Testes ponta a ponta com arquivos `.in` (entrada padrão opcional para `leia`) e `.out` (saída esperada conferida à mão) para os programas suportados de `test/valid/` (`make test-e2e`).

---

## 2. Como Executar os Testes

Para compilar e executar todas as suítes (`test-base`, `test-sem`, `test-tac` e `test-e2e`):

```bash
make clean && make test
```

Para rodar apenas uma suíte específica:

```bash
make test-base
make test-sem
make test-tac
make test-e2e
```

Para compilar e executar um arquivo individualmente no Python:

```bash
make run FILE=test/valid/funcao_simples.pt
```

---

## 3. Catálogo de Casos de Teste Válidos (`test/valid/`) e Ponta a Ponta (`test/e2e/`)

Todos os casos não relacionados a vetores possuem arquivo `.out` correspondente em `test/e2e/` validado por `make test-e2e`.

| ID | Arquivo | O que cobre | Entrada (`.in`) | Saída esperada (`.out`) |
|:---|:---|:---|:---|:---|
| **TC-V01** | `escopo_global_local.pt` | Variável global e local com mesmo nome | — | *(vazio)* |
| **TC-V02** | `exemplo.pt` | Programa mínimo com atribuição e `escreva` | — | `10` |
| **TC-V03** | `exemplo_func.pt` | Declaração com inicialização e `escreva` | — | `5` |
| **TC-V04** | `funcao_recursiva.pt` | Função recursiva (`fibonacci(5)`) com `se/senao` | — | `5` |
| **TC-V05** | `funcao_simples.pt` | Declaração, chamada e retorno de função `somar(2, 3)` | — | `5` |
| **TC-V06** | `multiplos_parametros.pt` | Múltiplos parâmetros (`inteiro`, `real`, `cadeia`) | — | `1`, `0` |
| **TC-V07** | `para.pt` | Laço `para` de `0` a `9` com `i++` | — | `0` .. `9` (1 por linha) |
| **TC-V08** | `para_decrescente.pt` | Laço `para` de `10` até `1` com `i--` | — | `10` .. `1` (1 por linha) |
| **TC-V09** | `para_incremento_complexo.pt` | Laço `para` com passo composto (`i += 2`) | — | `0`, `2`, `4`, `6`, `8` |
| **TC-V10** | `para_simples.pt` | Laço `para` de `0` a `4` com `i++` | — | `0` .. `4` (1 por linha) |
| **TC-V11** | `retorne_expressao.pt` | Retorno com precedência (`a + b * 2` para `3, 4`) | — | `11` |
| **TC-V12** | `teste_01_declaracoes.pt` | Declarações e inicializações dos 4 tipos primitivos | — | *(vazio)* |
| **TC-V13** | `teste_02_atribuicao_io.pt` | Atribuição, `escreva(x)` e `leia(x)` inteiro | `42` | `10`, `42` |
| **TC-V14** | `teste_03_expressoes_aritmeticas.pt` | Operadores `+`, `-`, `*`, `/`, `%`, parênteses e `real` | — | `20` |
| **TC-V15** | `teste_04_expressoes_logicas.pt` | Relacionais e lógicos (`e`, `ou`, `nao`), `logico` como `0/1` | — | `1` |
| **TC-V16** | `teste_05_escopo_global_local.pt` | Inicialização de `x` global (`5`) e `x` local (`10`) | — | *(vazio)* |
| **TC-V17** | `teste_06_se_senao.pt` | Condicional `se (nota >= 6) entao ... senao ...` | — | `Aprovado` |
| **TC-V18** | `teste_07_enquanto_se.pt` | Laço `enquanto` com `se (x == 5)` aninhado | — | `5` |
| **TC-V19** | `teste_08_bloco_vazio.pt` | Função `inicio()` com corpo vazio | — | *(vazio)* |
| **TC-V20** | `teste_09_funcao_parametro.pt` | Passagem de parâmetros para `somar(5, 3)` | — | `8` |
| **TC-V21** | `teste_11_para_incremento.pt` | Laço `para` de `0` a `9` com declaração no cabeçalho | — | `0` .. `9` (1 por linha) |
| **TC-V22** | `teste_10_vetores.pt` | Declaração em lote de vetores *(sem E2E: fora de escopo no TAC)* | — | — |
| **TC-V23** | `vetor.pt` | Declaração, escrita e leitura `numero[0]` *(sem E2E)* | — | — |
| **TC-V24** | `vetor_acesso.pt` | Atribuição posicional e `escreva(numero[0])` *(sem E2E)* | — | — |
| **TC-V25** | `vetor_declaracao.pt` | Declaração de vetores de tipos variados *(sem E2E)* | — | — |

---

## 4. Catálogo de Casos de Teste de Erro (`test/error/`)

Todos não possuem entrada externa e devem encerrar com código de saída `!= 0` em `make test-base`.

| ID | Arquivo | Categoria | O que cobre / Erro esperado |
|:---|:---|:---|:---|
| **TC-E01** | `teste_err01_ponto_virgula.pt` | Sintático | Uso indevido de `;` após atribuição e `escreva` (`syntax error`) |
| **TC-E02** | `teste_err02_char_invalido.pt` | Léxico | Caractere ilegal `@` fora do alfabeto da linguagem |
| **TC-E03** | `teste_err03_sem_inicio.pt` | Sintático / Estrutural | Programa sem a função obrigatória `funcao inicio()` |
| **TC-E04** | `teste_err04_redeclaracao.pt` | Semântico (Escopo) | Redeclaração de `x` do mesmo tipo no mesmo escopo |
| **TC-E05** | `teste_err05_argumentos_funcao.pt` | Semântico (Funções) | Chamada `somar(5, "texto")` com tipo de argumento incompatível |
| **TC-E06** | `teste_err06_tipo_retorno.pt` | Semântico (Funções) | `retorne "nao sou inteiro"` em função declarada como `inteiro` |
| **TC-E07** | `teste_err07_sem_retorno.pt` | Semântico (Funções) | Função não-void com `retorne` apenas dentro do `se` (sem `senao`) |
| **TC-E08** | `variavel_redeclarada.pt` | Semântico (Escopo) | Redeclaração da variável `numero` com outro tipo no mesmo escopo |
| **TC-E09** | `vetor_tamanho_invalido.pt` | Sintático (Vetores) | Declaração de vetor `numero[abc]` com identificador em vez de inteiro |
| **TC-E10** | `vet_tamanho_invalido.pt` | Sintático (Vetores) | Declaração de vetor `numero[abc]` com tamanho não literal |

---

## 5. Testes Semânticos (`test/semantica/`)

Cada `.pt` possui um `.msg` com o trecho esperado em `stderr`. O teste passa se o compilador retornar código `!= 0` **e** emitir a mensagem (`make test-sem`).

| ID | Arquivo | O que cobre | Mensagem esperada (`.msg`) |
|:---|:---|:---|:---|
| **TC-S01** | `sem01_var_nao_declarada.pt` | Uso de variável não declarada (`x`) | `Variavel 'x' nao foi declarada` |
| **TC-S02** | `sem02_atribuicao_tipo.pt` | Atribuição de `cadeia` em variável `inteiro` | `Tipo incompativel na atribuicao` |
| **TC-S03** | `sem03_condicao_nao_logica.pt` | Condição de `se` com expressão não lógica | `Condicao invalida` |
| **TC-S04** | `sem04_operacao_nao_numerica.pt` | Soma entre `inteiro` e `cadeia` (`1 + "x"`) | `esperava tipos numericos` |
| **TC-S05** | `sem05_logico_invalido.pt` | Operador `e` com operandos inteiros (`1 e 2`) | `espera tipo 'logico'` |
| **TC-S06** | `sem06_funcao_nao_declarada.pt` | Chamada à função inexistente `soma(...)` | `Funcao 'soma' nao declarada` |
| **TC-S07** | `sem07_numero_argumentos.pt` | Chamada com quantidade errada de argumentos | `Numero incorreto de argumentos` |

---

## 6. Testes de Código Intermediário (`test/tac/`)

Cada `.pt` possui um golden file `.tac` comparado via `diff` (`make test-tac`).

| ID | Arquivo | O que valida |
|:---|:---|:---|
| **TC-T01** | `tac01_aritmetica.pt` | Expressão constante dobrada antes do TAC (`2 + 3 * 4` → `14`) |
| **TC-T02** | `tac02_se_senao.pt` | `ifFalse`, `goto` e rótulos de `se/senao` |
| **TC-T03** | `tac03_enquanto.pt` | Laço `enquanto` (rótulo de início, saída e salto de volta) |
| **TC-T04** | `tac04_para.pt` | Laço `para` (inicialização, condição, passo) |
| **TC-T05** | `tac05_funcao_recursiva.pt` | `begin/end`, `formal`, `param`, `call`, `return` e remoção de `goto` morto |
| **TC-T06** | `tac06_codigo_morto.pt` | Remoção de código inalcançável pós-`retorne` (golden `.antes.tac` vs `.tac`) |
| **TC-T07** | `tac07_simplificacao.pt` | Dobragem de constantes e identidades algébricas (`x + 0`, `x * 1`, `x * 0`, etc.) |

Limitação conhecida: acesso e atribuição a vetor (`v[i]`) não geram nós na AST nem instruções no TAC ou código Python; por isso os 4 casos de vetor ficam apenas em `test-base` (sem `.tac` nem `.out` em `test/e2e/`).
