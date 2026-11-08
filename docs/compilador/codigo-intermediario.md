# Código intermediário (código de três endereços)

**Status:** implementado na Sprint 5 (`src/codigo_intermediario.c`) e validado por `make test-tac` (`test/tac/`). Itens marcados como **[pendente]** não foram implementados (ex.: acesso a vetor, conversão de tipos).

---

## 1. Objetivo e posição no pipeline

```
Léxica → Sintática (AST) → Semântica (AST anotada) → Código intermediário (TAC) → Otimização (Sprint 6) → Código final
```

O código de três endereços (TAC, *three-address code*) é uma representação linear e simples do programa, em que cada instrução realiza no máximo uma operação e tem, no máximo, dois operandos de entrada e um de saída. Ele:

- é independente de máquina (não tem registradores nem endereços reais);
- é fácil de gerar a partir da AST, percorrendo a árvore;
- é fácil de otimizar (Sprint 6) e de traduzir para uma arquitetura-alvo no fim.

---

## 2. Estrutura da instrução

Cada instrução é uma **quádrupla**:

```
(op, arg1, arg2, resultado)
```

- `op`: operação (ver seção 5).
- `arg1`, `arg2`: operandos de entrada. Use `-` quando não houver.
- `resultado`: destino. Em desvios, guarda o rótulo de destino. Use `-` quando não houver.

Representação em C, coerente com o restante do projeto:

```c
typedef enum { OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD, /* ... */ } OpTAC;

typedef enum { OPD_NENHUM, OPD_VAR, OPD_TEMP, OPD_CONST_INT, OPD_CONST_REAL,
               OPD_CONST_STR, OPD_ROTULO, OPD_FUNC } TipoOperando;

typedef struct {
    TipoOperando tipo;
    union {
        char  *nome;      /* OPD_VAR, OPD_FUNC, OPD_ROTULO ("L1") e OPD_TEMP ("t1") */
        long   inteiro;   /* OPD_CONST_INT */
        double real;      /* OPD_CONST_REAL */
        char  *str;       /* OPD_CONST_STR */
    } valor;
} Operando;

typedef struct Instrucao {
    OpTAC op;
    Operando arg1, arg2, resultado;
    struct Instrucao *prox;   /* lista encadeada, na ordem de emissão */
} Instrucao;
```

A lista é encadeada na ordem de emissão. Rótulos são instruções próprias (`label`) inseridas na lista.

---

## 3. Operandos

| Tipo | Exemplo | Origem |
|---|---|---|
| Variável | `x`, `v`, `soma` | Nome na tabela de símbolos |
| Temporário | `t1`, `t2`, ... | Resultado intermediário de uma expressão |
| Constante | `10`, `3.5`, `"oi"` | Literais |
| Rótulo | `L1`, `L2`, ... | Destino de desvio |
| Função | `soma` | Nome em `begin`, `end` e `call` |

### Temporários
- Um temporário novo para cada nó de expressão que produz valor.
- Contador global (`t1`, `t2`, ...), para que os nomes sejam únicos no programa inteiro.
- Não há reaproveitamento nesta fase. Isso fica para a otimização (Sprint 6).

### Rótulos
- Contador global (`L1`, `L2`, ...), gerado por cada estrutura de controle.
- Um rótulo é definido com `label` e referenciado por `goto`, `ifFalse` ou `ifTrue`.

---

## 4. Convenções gerais

- O resultado é sempre o último campo (destino).
- `=` é **atribuição** (cópia). Não confundir com `==`, que é comparação.
- Operações relacionais e lógicas produzem um valor booleano em um temporário.
- Declarações sem inicialização (`inteiro x`) não geram instrução.
- Declarações com inicialização (`inteiro x = e`) geram o mesmo código que a atribuição `x = e`.

---

## 5. Conjunto de instruções

### 5.1 Atribuição e aritmética

| Instrução | Forma | Significado |
|---|---|---|
| Cópia | `(=, a, -, r)` | `r = a` |
| Soma | `(+, a, b, r)` | `r = a + b` |
| Subtração | `(-, a, b, r)` | `r = a - b` |
| Multiplicação | `(*, a, b, r)` | `r = a * b` |
| Divisão | `(/, a, b, r)` | `r = a / b` |
| Módulo | `(%, a, b, r)` | `r = a % b` |
| Negação unária | `(neg, a, -, r)` | `r = -a` |

Compostas (`++`, `--`, `+=`, `-=`, `*=`, `/=`) são **reescritas** como operação binária com o mesmo destino:

| Fonte | TAC |
|---|---|
| `i++` | `(+, i, 1, i)` |
| `i--` | `(-, i, 1, i)` |
| `i += n` | `(+, i, n, i)` |
| `i -= n` | `(-, i, n, i)` |
| `i *= n` | `(*, i, n, i)` |
| `i /= n` | `(/, i, n, i)` |

### 5.2 Relacionais e lógicas

| Instrução | Forma | Significado |
|---|---|---|
| Igual | `(==, a, b, r)` | `r = (a == b)` |
| Diferente | `(!=, a, b, r)` | `r = (a != b)` |
| Menor | `(<, a, b, r)` | `r = (a < b)` |
| Maior | `(>, a, b, r)` | `r = (a > b)` |
| Menor ou igual | `(<=, a, b, r)` | `r = (a <= b)` |
| Maior ou igual | `(>=, a, b, r)` | `r = (a >= b)` |
| E | `(and, a, b, r)` | `r = a E b` |
| OU | `(or, a, b, r)` | `r = a OU b` |
| NÃO | `(not, a, -, r)` | `r = NAO a` |

- Nesta versão, `E` e `OU` **não** são de curto-circuito: os dois lados são avaliados. **[pendente]** Se o grupo quiser curto-circuito, a expressão é reescrita com desvios (ver seção 6.4).

### 5.3 Conversão de tipos **[pendente]**

| Instrução | Forma | Significado |
|---|---|---|
| Inteiro para real | `(itor, a, -, r)` | `r = (real) a` |

Só entra se a análise semântica decidir coerção implícita entre `inteiro` e `real`. Quem insere a instrução é a semântica (ela sabe os tipos), e o gerador apenas a emite.

### 5.4 Desvios e rótulos

| Instrução | Forma | Significado |
|---|---|---|
| Rótulo | `(label, -, -, L)` | Define o ponto de destino `L` |
| Desvio incondicional | `(goto, -, -, L)` | Salta para `L` |
| Desvio se falso | `(ifFalse, c, -, L)` | Se `c` for falso, salta para `L` |
| Desvio se verdadeiro | `(ifTrue, c, -, L)` | Se `c` for verdadeiro, salta para `L` |

O destino do desvio fica sempre em `resultado`.

### 5.5 Funções

| Instrução | Forma | Significado |
|---|---|---|
| Início | `(begin, f, n, -)` | Início da função `f` com `n` parâmetros |
| Fim | `(end, f, -, -)` | Fim da função `f` |
| Parâmetro | `(param, v, -, -)` | Empilha o valor `v` como próximo argumento |
| Chamada | `(call, f, n, r)` | Chama `f` com `n` argumentos; grava o retorno em `r` (ou `-` se não usado) |
| Retorno | `(return, v, -, -)` | Retorna `v` (ou `-` se não houver valor) |

- Parâmetros formais são acessados pelo nome, na ordem de `begin`.
- Os `param` de uma chamada são emitidos **na ordem dos argumentos**, imediatamente antes do `call`.
- Os argumentos são avaliados antes dos `param`, então uma chamada aninhada não interfere na anterior.

### 5.6 Vetores **[fora do escopo por ora]**

**Decisão do grupo:** vetores não são prioridade agora. A AST (`src/ast.h`) não tem nó de acesso ou atribuição a vetor — `acesso_vetor` no parser devolve `NULL` de propósito. O gerador de código intermediário, por consequência, não trata vetores por enquanto. Essas instruções ficam especificadas aqui para o caso de haver tempo de implementar depois da Sprint 5; não é uma lacuna a cobrir às pressas.

| Instrução | Forma | Significado |
|---|---|---|
| Leitura | `(=[], v, i, r)` | `r = v[i]` |
| Escrita | `([]=, x, i, v)` | `v[i] = x` |

- Na escrita, `arg1` é o valor, `arg2` é o índice e `resultado` é o vetor.
- Índice e valor são avaliados antes da instrução.
- Para habilitar, antes é preciso: um nó de acesso (`NO_VETOR_LEITURA` ou similar) e um nó de atribuição a vetor na AST, mais a correspondente regra semântica de tipo do vetor.

### 5.7 Entrada e saída

| Instrução | Forma | Significado |
|---|---|---|
| Escrita | `(write, a, -, -)` | Escreve `a` |
| Leitura | `(read, -, -, x)` | Lê um valor em `x` |

`escreva` recebe uma única expressão na gramática atual (`ESCREVA ABRE_PAR expressao FECHA_PAR`), então gera um único `write`.

---

## 6. Como as estruturas são traduzidas

### 6.1 `se` / `senao`

```
se (cond) ENTAO { A } SENAO { B }
```
```
  <código de cond → t>
  (ifFalse, t, -, L_senao)
  <A>
  (goto, -, -, L_fim)
  (label, -, -, L_senao)
  <B>
  (label, -, -, L_fim)
```
Sem `senao`, o `goto` e o segundo `label` são omitidos.

### 6.2 `enquanto`

```
enquanto (cond) FACA { A }
```
```
  (label, -, -, L_ini)
  <código de cond → t>
  (ifFalse, t, -, L_fim)
  <A>
  (goto, -, -, L_ini)
  (label, -, -, L_fim)
```

### 6.3 `para`

```
para (inteiro i = i0; cond; passo) { A }
```
```
  <inicialização: i = i0>
  (label, -, -, L_ini)
  <código de cond → t>
  (ifFalse, t, -, L_fim)
  <A>
  (label, -, -, L_passo)
  <código de passo>
  (goto, -, -, L_ini)
  (label, -, -, L_fim)
```
- `L_passo` existe para a futura instrução de continuação, mesmo que a linguagem não a tenha. Não custa nada e facilita a otimização.

### 6.4 Curto-circuito (opcional) **[pendente]**

`a E b` com curto-circuito:
```
  <a → ta>
  (ifFalse, ta, -, L_falso)
  <b → tb>
  (and, ta, tb, r)    -- ou: atribuir tb a r
  (goto, -, -, L_fim)
  (label, -, -, L_falso)
  (=, 0, -, r)  -- valor falso
  (label, -, -, L_fim)
```
Só entra se o grupo decidir por curto-circuito.

### 6.5 Expressões

Cada nó da AST produz um operando. A geração é recursiva:

- `gen(constante)` → a própria constante;
- `gen(variável)` → o nome da variável;
- `gen(a op b)` → gera `gen(a)`, `gen(b)`, cria `t` novo e emite `(op, gen(a), gen(b), t)`; retorna `t`.

---

## 7. Exemplos

### 7.1 Atribuição e condição

Fonte (sintaxe ilustrativa, conforme a gramática da sprint 2):
```
inteiro x
inteiro y
x = 2 + 3 * y
se (x > 10) ENTAO { escreva(x) }
```
TAC:
```
(*, 3, y, t1)
(+, 2, t1, t2)
(=, t2, -, x)
(>, x, 10, t3)
(ifFalse, t3, -, L1)
(write, x, -, -)
(label, -, -, L1)
```

### 7.2 Laço `para`

Fonte:
```
para (inteiro i = 0; i < n; i++) { escreva(i) }
```
TAC:
```
(=, 0, -, i)
(label, -, -, L1)
(<, i, n, t1)
(ifFalse, t1, -, L3)
(write, i, -, -)
(label, -, -, L2)
(+, i, 1, i)
(goto, -, -, L1)
(label, -, -, L3)
```

### 7.3 Função e chamada

Fonte:
```
funcao inteiro soma(inteiro a, inteiro b) { retorne a + b }
x = soma(1, x)
```
TAC:
```
(begin, soma, 2, -)
(+, a, b, t1)
(return, t1, -, -)
(end, soma, -, -)
(param, 1, -, -)
(param, x, -, -)
(call, soma, 2, t2)
(=, t2, -, x)
```

(O exemplo de vetor foi removido daqui porque vetores estão fora de escopo por ora — ver seção 5.6.)

---

## 8. Dependências da análise semântica

A geração de código **assume** que a semântica já:

1. resolveu cada identificador na tabela de símbolos (escopo correto);
2. verificou tipos e inseriu as coerções necessárias (`itor`), se houver;
3. anotou na AST o tipo de cada expressão.

Decisões que precisam ser fechadas antes de implementar:

- **Conjunto de tipos**: `caractere` e `logico` aparecem em `Decisões.md`, mas não na gramática da sprint 2.
- **Coerção `inteiro` / `real`**: implícita ou proibida? Define se `itor` existe.
- **Cadeias**: `cadeia + cadeia` é permitido? Se for, precisa de uma instrução `concat`.
- **Curto-circuito** em `E` e `OU` (seção 6.4).
- **Símbolo de atribuição**: `Decisões.md` diz `:=`, mas a gramática usa `ATRIBUICAO(=)`. Esta TAC usa `=` no código intermediário, qualquer que seja o símbolo da linguagem-fonte.
- **Vetores**: fora do escopo da Sprint 5 por decisão do grupo (seção 5.6). Só entram se houver tempo depois do essencial.

---

## 9. Próximos passos

1. Fechar as decisões da seção 8.
2. Implementar a geração a partir da AST (`src/codigo_intermediario.c`, a confirmar).
3. Teste de referência: para cada exemplo da seção 7, comparar a saída com este documento.
4. Impressão da lista de instruções em modo de depuração (uma linha por instrução, no formato da seção 2).
