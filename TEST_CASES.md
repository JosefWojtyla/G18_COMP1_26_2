# Suíte de Testes do Compilador Portugol

Suíte de testes automatizados do compilador, cobrindo análise léxica, sintática e semântica com foco em **funções**, **vetores**, laço **`para`** e **tabela de símbolos / escopos**.

---

## 1. Estrutura da Suíte

Os testes estão organizados em duas categorias principais dentro do diretório `test/`:

- **`test/valid/`**: Casos de teste sintática e semanticamente corretos. O compilador deve realizar o parse com sucesso e retornar código de saída `0`.
- **`test/error/`**: Casos de teste com erros léxicos, sintáticos ou semânticos propositais. O compilador deve identificar e reportar o erro, retornando código de saída diferente de zero (`!= 0`).

---

## 2. Como Executar os Testes

Para executar toda a suíte de testes de forma automatizada via `Makefile`:

```bash
make test
```

*(No ambiente Windows via MinGW/MSYS2: `mingw32-make test` ou com ambiente configurado).*

Para executar um arquivo de teste individualmente exibindo a saída completa do parser e a tabela de símbolos:

```bash
make run FILE=test/valid/funcao_simples.pt
# ou diretamente via executável:
./build/portugol test/valid/funcao_simples.pt
```

---

## 3. Catálogo de Casos de Teste Válidos (`test/valid/`)

| ID | Arquivo | Categoria | Descrição | Comportamento Esperado |
|:---|:---|:---|:---|:---|
| **TC-V01** | `funcao_simples.pt` | Funções | Declaração e chamada de função simples com retorno de tipo inteiro | Parse com sucesso (cód 0); símbolo registrado |
| **TC-V02** | `funcao_recursiva.pt` | Funções / Recursão | Função recursiva calculando a sequência de Fibonacci com desvio condicional (`se ... senao`) | Parse com sucesso (cód 0); chamadas aninhadas |
| **TC-V03** | `multiplos_parametros.pt` | Funções | Função declarada com múltiplos parâmetros de tipos distintos (`inteiro`, `real`, `cadeia`) | Parse com sucesso (cód 0); parâmetros reconhecidos |
| **TC-V04** | `retorne_expressao.pt` | Funções / Expressões | Retorno de função contendo expressão aritmética combinada (`a + b * 2`) testando precedência | Parse com sucesso (cód 0); árvore de expressão correta |
| **TC-V05** | `vetor_declaracao.pt` | Vetores | Declaração de múltiplos vetores com diferentes tipos primitivos e tamanhos literais | Parse com sucesso (cód 0); vetores reconhecidos |
| **TC-V06** | `vetor_acesso.pt` | Vetores | Indexação, atribuição posicional (`v[0] = 42`) e leitura de vetor como argumento de `escreva` | Parse com sucesso (cód 0); acesso indexado válido |
| **TC-V07** | `para_simples.pt` | Laço `para` | Laço `para` incremental simples (`para(inteiro i=0; i<5; i++)`) | Parse com sucesso (cód 0); estrutura `para` válida |
| **TC-V08** | `para_decrescente.pt` | Laço `para` | Laço `para` com passo decrescente utilizando operador pós-fixado `--` | Parse com sucesso (cód 0); decremento reconhecido |
| **TC-V09** | `para_incremento_complexo.pt`| Laço `para` | Laço `para` com passo de atribuição composta (`i += 2`) | Parse com sucesso (cód 0); operador composto aceito |
| **TC-V10** | `escopo_global_local.pt` | Tabela de Símbolos | Variável declarada em escopo global (nível 0) com mesmo identificador redeclarado em escopo local (nível 1) | Sucesso (cód 0); tabela registra ambas com escopos distintos |
| **TC-V11** | `teste_01_declaracoes.pt` | Variáveis | Declaração inicial de variáveis dos tipos primitivos | Sucesso (cód 0) |
| **TC-V12** | `teste_02_atribuicao_io.pt` | E/S | Atribuição de variáveis e comandos de entrada/saída (`leia`, `escreva`) | Sucesso (cód 0) |
| **TC-V13** | `teste_03_expressoes_aritmeticas.pt` | Expressões | Expressões aritméticas com operadores básicos (`+`, `-`, `*`, `/`, `%`) | Sucesso (cód 0) |
| **TC-V14** | `teste_04_expressoes_logicas.pt` | Expressões | Expressões lógicas e relacionais com operadores `e`, `ou`, `nao`, `==`, `!=`, `<`, `>` | Sucesso (cód 0) |
| **TC-V15** | `teste_06_se_senao.pt` | Condicionais | Estrutura de controle de fluxo condicional `se (...) entao { } senao { }` | Sucesso (cód 0) |
| **TC-V16** | `teste_08_bloco_vazio.pt` | Sintaxe | Função `inicio()` com bloco de comandos vazio | Sucesso (cód 0) |
| **TC-V17** | `teste_09_funcao_parametro.pt` | Funções | Passagem e consumo de parâmetros em chamada de função | Sucesso (cód 0) |
| **TC-V18** | `teste_10_vetores.pt` | Vetores | Declarações em lote de vetores | Sucesso (cód 0) |
| **TC-V19** | `teste_11_para_incremento.pt` | Laço `para` | Validação de laço `para` com declaração interna | Sucesso (cód 0) |

---

## 4. Catálogo de Casos de Teste de Erro (`test/error/`)

| ID | Arquivo | Categoria | Descrição do Erro | Erro Esperado |
|:---|:---|:---|:---|:---|
| **TC-E01** | `vetor_tamanho_invalido.pt` | Vetores (Sintático) | Declaração de vetor utilizando identificador em vez de literal inteiro (`numero[abc]`) | Rejeição sintática (`syntax error`), código != 0 |
| **TC-E02** | `variavel_redeclarada.pt` | Semântico (Escopo) | Redeclaração da variável `numero` no mesmo escopo local | Rejeição semântica: `"Variável já declarada neste escopo"` |
| **TC-E03** | `teste_err01_ponto_virgula.pt`| Sintático | Uso indevido de ponto e vírgula após comando de saída `escreva` | Rejeição sintática com recuperação de erro |
| **TC-E04** | `teste_err02_char_invalido.pt` | Léxico | Inserção de caracteres ilegais não pertencentes ao alfabeto da linguagem (ex: `@`, `$`) | Rejeição léxica (`caractere inválido`), código != 0 |
| **TC-E05** | `teste_err03_sem_inicio.pt` | Estrutural | Programa ausente da função obrigatória `funcao inicio()` | Rejeição estrutural no parse |
| **TC-E06** | `teste_err04_redeclaracao.pt` | Semântico (Escopo) | Redeclaração de variável com inicialização simultânea no mesmo escopo | Rejeição semântica: `"Variável já declarada neste escopo"` |

---

## 4.1 Testes Semânticos (`test/semantica/`)

Cada `.pt` possui um `.msg` com o trecho da mensagem esperada em `stderr`. O teste passa se o compilador retornar código != 0 **e** emitir a mensagem. Execução: `make test-sem`.

| ID | Arquivo | Erro semântico validado | Mensagem esperada |
|:---|:---|:---|:---|
| **TC-S01** | `sem01_var_nao_declarada.pt` | Uso de variável não declarada | `Variavel 'x' nao foi declarada` |
| **TC-S02** | `sem02_atribuicao_tipo.pt` | `inteiro` recebendo `cadeia` | `Tipo incompativel na atribuicao` |
| **TC-S03** | `sem03_condicao_nao_logica.pt` | Condição de `se` não lógica | `Condicao invalida` |
| **TC-S04** | `sem04_operacao_nao_numerica.pt` | `1 + "x"` | `esperava tipos numericos` |
| **TC-S05** | `sem05_logico_invalido.pt` | `1 e 2` | `espera tipo 'logico'` |
| **TC-S06** | `sem06_funcao_nao_declarada.pt` | Chamada de função inexistente | `Funcao 'soma' nao declarada` |
| **TC-S07** | `sem07_numero_argumentos.pt` | Número errado de argumentos | `Numero incorreto de argumentos` |

## 4.2 Testes de Código Intermediário (`test/tac/`)

Cada `.pt` possui um `.tac` com o código de três endereços esperado (golden file). O teste compara a saída exata do gerador. Execução: `make test-tac`.

| ID | Arquivo | O que valida |
|:---|:---|:---|
| **TC-T01** | `tac01_aritmetica.pt` | Temporários e precedência (`2 + 3 * 4`) |
| **TC-T02** | `tac02_se_senao.pt` | `ifFalse`, `goto` e rótulos de `se/senao` |
| **TC-T03** | `tac03_enquanto.pt` | Laço `enquanto` (rótulo de início, saída e salto de volta) |
| **TC-T04** | `tac04_para.pt` | Laço `para` (inicialização, condição, passo) |
| **TC-T05** | `tac05_funcao_recursiva.pt` | `begin/end`, `param`, `call`, `return` |

Limitação conhecida: acesso a vetor (`v[i]`) ainda não é gerado no TAC (emite aviso e ignora); por isso não há golden file para vetores.

`make test` roda todas as suítes; `make test-base` roda só `valid/` e `error/`.

---

## 5. Resumo da Execução dos Testes Automatizados

Execução consolidada via `make test`:

```text
========================================
 RODANDO TESTES
========================================

-- Testes validos (devem PASS) --
  test/valid/escopo_global_local.pt        [PASS]
  test/valid/funcao_recursiva.pt           [PASS]
  test/valid/funcao_simples.pt             [PASS]
  test/valid/multiplos_parametros.pt       [PASS]
  test/valid/para_decrescente.pt           [PASS]
  test/valid/para_incremento_complexo.pt   [PASS]
  test/valid/para_simples.pt               [PASS]
  test/valid/retorne_expressao.pt          [PASS]
  test/valid/vetor_acesso.pt               [PASS]
  test/valid/vetor_declaracao.pt           [PASS]
  ... (demais testes da suite anterior)    [PASS]

-- Testes de erro (devem FAIL) --
  test/error/teste_err01_ponto_virgula.pt  [PASS] (erro detectado corretamente)
  test/error/teste_err02_char_invalido.pt  [PASS] (erro detectado corretamente)
  test/error/teste_err03_sem_inicio.pt     [PASS] (erro detectado corretamente)
  test/error/teste_err04_redeclaracao.pt   [PASS] (erro detectado corretamente)
  test/error/variavel_redeclarada.pt       [PASS] (erro detectado corretamente)
  test/error/vetor_tamanho_invalido.pt     [PASS] (erro detectado corretamente)

========================================
  Resultado: 31 PASS  |  0 FAIL
========================================
```
