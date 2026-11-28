# Grupo 18 - Compiladores 1 (2026/2)

Compilador da linguagem **Portugol** (sintaxe do Portugol Studio / UNIVALI) para **Python**, escrito em C com **Flex** e **Bison**.

O compilador passa o programa pelas fases clássicas:

```
arquivo.pt → léxica (Flex) → sintática + AST (Bison) → semântica + tabela de símbolos
           → otimização da AST → código de três endereços (TAC) → otimização do TAC → Python
```

Documentação completa: [GitHub Pages](https://josefwojtyla.github.io/G18_COMP1_26_2/)

---

## O que a linguagem suporta

- Tipos `inteiro`, `real`, `cadeia` e `logico`
- Variáveis globais e locais (escopo por função), com inicialização na declaração
- Expressões aritméticas (`+ - * / %`), relacionais (`== != < > <= >=`) e lógicas (`e`, `ou`, `nao`)
- Atribuições compostas: `++`, `--`, `+=`, `-=`, `*=`, `/=`
- `se ... entao ... senao`, `enquanto ... faca`, `para (...; ...; ...)`
- Funções com parâmetros tipados, `retorne` e recursão
- `escreva(expressao)` (uma expressão por chamada) e `leia(variavel)`
- Comentários `//` e `/* */`

**Não suportado:** vetores. O parser reconhece a sintaxe (`inteiro v[10]`, `v[i]`), mas eles não entram na AST, no TAC nem no Python gerado (decisão da equipe; ver [entrega final](docs/entregas/entrega-final.md)).

---

## Dependências

| Ferramenta | Para quê |
|---|---|
| `flex` | gerar o analisador léxico (`lexer/lex.yy.c`) |
| `bison` | gerar o analisador sintático (`parser/portugol.tab.c`) |
| `gcc` | compilar o compilador |
| `make` | automatizar build e testes |
| `python3` | executar o código gerado e os testes ponta a ponta |

No Ubuntu/Debian (ou WSL):

```bash
sudo apt install flex bison gcc make python3
```

---

## Como compilar o compilador

```bash
make          # gera build/portugol
make clean    # apaga arquivos gerados (build/, lex.yy.c, portugol.tab.*)
```

> Se o build falhar com erros estranhos depois de mudar a gramática, rode `make clean && make` para regenerar o parser com o Bison.

---

## Como compilar um programa Portugol

```bash
build/portugol arquivo.pt -o saida.py   # gera o Python
python3 saida.py                        # executa
```

Sem `-o`, o Python é escrito na saída padrão. Opções de depuração (escritas em `stderr`):

| Opção | Mostra |
|---|---|
| `--tac` | código de três endereços (já otimizado) |
| `--ast` | árvore sintática abstrata |
| `--simbolos` | tabela de símbolos |

Atalhos no Makefile:

```bash
make run   FILE=test/valid/funcao_recursiva.pt   # compila e executa
make debug FILE=test/valid/funcao_recursiva.pt   # mostra TAC, símbolos e o Python
```

Se houver erro léxico, sintático ou semântico, o compilador mostra a mensagem com a linha, termina com código diferente de 0 e não gera Python.

---

## Como rodar os testes

```bash
make test        # todas as suítes
make test-base   # test/valid (devem compilar) e test/error (devem ser rejeitados)
make test-sem    # test/semantica: erro semântico com a mensagem esperada (.msg)
make test-tac    # test/tac: TAC gerado comparado com o .tac esperado
make test-e2e    # test/e2e: compila, executa o Python e compara a saída com o .out
```

Resultado atual: **66 casos, 66 PASS** (31 base, 7 semânticos, 7 TAC, 21 ponta a ponta). O catálogo dos casos está em [`TEST_CASES.md`](TEST_CASES.md).

---

## Exemplo completo

Entrada `test/valid/funcao_recursiva.pt`:

```
programa {
    funcao inteiro fibonacci(inteiro n) {
        se (n <= 1) entao {
            retorne n
        } senao {
            retorne fibonacci(n - 1) + fibonacci(n - 2)
        }
    }

    funcao inicio() {
        inteiro f = fibonacci(5)
        escreva(f)
    }
}
```

Compilando com o TAC visível:

```bash
build/portugol --tac test/valid/funcao_recursiva.pt -o fib.py
```

TAC (`stderr`):

```
(begin, fibonacci, 1, -)
(formal, n, -, -)
(<=, n, 1, t0)
(ifFalse, t0, -, L0)
(return, n, -, -)
(label, -, -, L0)
(-, n, 1, t1)
(param, t1, -, -)
(call, fibonacci, 1, t2)
(-, n, 2, t3)
(param, t3, -, -)
(call, fibonacci, 1, t4)
(+, t2, t4, t5)
(return, t5, -, -)
(label, -, -, L1)
(end, fibonacci, -, -)
(begin, inicio, 0, -)
(param, 5, -, -)
(call, fibonacci, 1, t6)
(=, t6, -, f)
(write, f, -, -)
(end, inicio, -, -)
```

Python gerado em `fib.py` (sem as funções auxiliares `_ler` e `_div` do topo do arquivo):

```python
def fibonacci(n):
    _pc = 0
    while True:
        if _pc == 0:
            t0 = n <= 1
            if not t0:
                _pc = 1
                continue
            return n
        if _pc == 1:
            t1 = n - 1
            t2 = fibonacci(t1)
            t3 = n - 2
            t4 = fibonacci(t3)
            t5 = t2 + t4
            return t5
        if _pc == 2:
            pass
        break

def inicio():
    t6 = fibonacci(5)
    f = t6
    print(f)

inicio()
```

Execução:

```
$ python3 fib.py
5
```

O esquema com `_pc` existe porque Python não tem `goto`; ele está explicado em [código final](docs/compilador/codigo-final.md).

---

## Estrutura do repositório

```
lexer/portugol.l            analisador léxico (Flex)
parser/portugol.y           gramática, construção da AST e main (Bison)
src/ast.c, ast.h            AST e otimização de expressões
src/simbolos.c, simbolos.h  tabela de símbolos com escopos
src/semantica.c             verificações semânticas e inferência de tipos
src/codigo_intermediario.c  geração e otimização do TAC
src/codigo_final.c          tradução do TAC para Python
test/                       suítes de teste (valid, error, semantica, tac, e2e)
docs/                       documentação publicada com MkDocs
```

## Documentação local

```bash
pip install mkdocs-material
mkdocs serve      # http://127.0.0.1:8000
mkdocs build      # gera site/
```

## Estudantes

| Aluno | Matrícula |
| --- | --- |
| Josef Wojtyla Barros de Souza | 251023282 |
| Zayra Moraes | 242015989 |
