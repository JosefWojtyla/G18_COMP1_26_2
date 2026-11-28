# Código final

**Status:** implementado na Sprint 6 (`src/codigo_final.c`, PRs #59 e #60) e validado por `make test-e2e` (21 programas compilados, executados e comparados com a saída esperada).

## 1. Alvo: Python

O compilador gera um arquivo Python 3 a partir do código de três endereços (TAC) já otimizado:

```
AST → TAC → otimização do TAC → gerar_codigo_final(tac, saida) → arquivo.py
```

A equipe escolheu Python em vez de assembly porque:

- os tipos do Portugol têm equivalente direto (`inteiro → int`, `real → float`, `cadeia → str`, `logico → bool`);
- o resultado pode ser executado em qualquer máquina com `python3`, o que permite testes ponta a ponta automáticos;
- a tradução fica perto do TAC (uma quádrupla vira, em geral, uma linha de Python), o que facilita conferir a saída.

O problema principal é que Python **não tem `goto`**, e o TAC é feito de rótulos e saltos. A solução é o esquema do contador `_pc` (seção 4).

## 2. Estrutura do arquivo gerado

1. Cabeçalho e duas funções auxiliares fixas:
    - `_ler()`: lê uma linha e tenta converter para `int`, depois `float`, senão devolve texto. Usada quando o tipo do destino do `leia` não é conhecido.
    - `_div(a, b)`: divisão inteira (`//`) se os dois lados forem `int`, divisão real (`/`) caso contrário. Reproduz a semântica do Portugol, em que `7 / 2` entre inteiros dá `3`.
2. Uma `def` para cada função do TAC (cada par `begin ... end`), inclusive `inicio`.
3. Instruções do TAC fora de qualquer função: inicialização das variáveis globais.
4. A chamada `inicio()`.

## 3. Tabela de tradução TAC → Python

| TAC | Python | Observação |
|---|---|---|
| `(+, a, b, r)` (e `-`, `*`, `%`) | `r = a + b` | |
| `(/, a, b, r)` | `r = _div(a, b)` | inteira ou real conforme os operandos |
| `(==, a, b, r)` (e `!=`, `<`, `>`, `<=`, `>=`) | `r = a == b` | |
| `(and, a, b, r)` / `(or, a, b, r)` | `r = a and b` / `r = a or b` | |
| `(not, a, -, r)` | `r = not a` | |
| `(=, a, -, x)` | `x = a` | se `x` é `real`: `x = float(a)` |
| `(write, a, -, -)` | `print(a)` | |
| `(read, -, -, x)` | `x = int(input())` | `float(input())`, `input()` ou `_ler()` conforme o tipo de `x` |
| `(begin, f, n, -)` + `(formal, p, -, -)` | `def f(p1, p2, ...):` | |
| `(end, f, -, -)` | fim da `def` | |
| `(param, v, -, -)` | (nada) | o valor é guardado até o `call` |
| `(call, f, n, r)` | `r = f(v1, ..., vn)` | usa os `n` últimos `param` |
| `(return, v, -, -)` | `return v` | se a função é `real`: `return float(v)` |
| `(label, -, -, L)` | `if _pc == k:` | início de um bloco |
| `(goto, -, -, L)` | `_pc = k` + `continue` | |
| `(ifFalse, c, -, L)` | `if not c: _pc = k; continue` | |
| `(ifTrue, c, -, L)` | `if c: _pc = k; continue` | |

Outros detalhes da tradução:

- **Tipos.** O TAC não guarda tipos, então o gerador consulta a tabela de símbolos. Só o `real` precisa de cuidado: em Python, `real r = 7` guardaria o inteiro `7`, e `r / 2` daria `3`. Por isso atribuições, parâmetros e retornos `real` passam por `float(...)`.
- **Globais.** Se uma função atribui a uma variável global, o gerador escreve `global x` no início da `def`; sem isso o Python criaria uma variável local com o mesmo nome.
- **Nomes reservados.** Um identificador igual a uma palavra reservada do Python (`print` não é, mas `def`, `class`, `in`... são) ganha um `_` no fim.
- **Constantes reais.** `2.0` é escrito como `2.0`, não `2`, para não virar `int` no Python.

## 4. O esquema do `_pc`

Uma função **sem** rótulos nem saltos é traduzida em sequência, linha a linha.

Uma função **com** saltos vira uma máquina de estados:

- O corpo é dividido em **blocos**: o bloco 0 é o trecho antes do primeiro `label`; o bloco `k` começa no `k`-ésimo `label`.
- A variável `_pc` ("program counter") guarda o número do bloco a executar.
- Tudo fica dentro de um `while True:`. Cada bloco é um `if _pc == k:`.
- Saltar para o rótulo do bloco `k` é `_pc = k` seguido de `continue` (volta ao topo do laço).
- No fim de um bloco que não termina em `goto`/`return`, o gerador escreve `_pc = k + 1`. Como os testes são `if` seguidos (não `elif`), o bloco seguinte executa na mesma volta do laço, imitando a "queda" de um bloco no outro.
- Depois do último bloco vem `break`, que encerra a função.

```python
_pc = 0
while True:
    if _pc == 0:
        ...           # bloco 0
        _pc = 1       # cai no bloco 1
    if _pc == 1:
        ...
        if not t0:
            _pc = 3   # goto L(bloco 3)
            continue
        ...
    ...
    break
```

## 5. Exemplo antes/depois

Fonte (`test/valid/teste_07_enquanto_se.pt`):

```
programa {
    funcao inicio() {
        inteiro x = 0
        enquanto(x < 10) faca {
            se(x == 5) entao {
                escreva(x)
            }
            x = x + 1
        }
    }
}
```

TAC (`build/portugol --tac ...`):

```
(begin, inicio, 0, -)
(=, 0, -, x)
(label, -, -, L0)        ← bloco 1
(<, x, 10, t0)
(ifFalse, t0, -, L1)
(==, x, 5, t1)
(ifFalse, t1, -, L2)
(write, x, -, -)
(label, -, -, L2)        ← bloco 2
(+, x, 1, t2)
(=, t2, -, x)
(goto, -, -, L0)
(label, -, -, L1)        ← bloco 3
(end, inicio, -, -)
```

Python gerado:

```python
def inicio():
    _pc = 0
    while True:
        if _pc == 0:
            x = 0
            _pc = 1
        if _pc == 1:
            t0 = x < 10
            if not t0:
                _pc = 3
                continue
            t1 = x == 5
            if not t1:
                _pc = 2
                continue
            print(x)
            _pc = 2
        if _pc == 2:
            t2 = x + 1
            x = t2
            _pc = 1
            continue
        if _pc == 3:
            pass
        break

inicio()
```

Saída da execução: `5`.

## 6. Testes

`make test-e2e` compila cada programa de `test/valid/` que tem um `.out` em `test/e2e/`, executa o Python (com o `.in` como entrada, se existir) e compara a saída. Resultado atual: **21 PASS, 0 FAIL**.

## 7. Limitações

- Vetores não são traduzidos (não existem na AST nem no TAC).
- `escreva` aceita uma única expressão, e cada `print` termina a linha.
- Se o mesmo nome for declarado com tipos diferentes em escopos diferentes, o gerador não sabe qual é qual e não aplica a conversão para `float`.
