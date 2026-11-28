# Entrega final

## Compilador Portugol → Python | Sprints 6 e 7

**Data:** 30/11/2026 · **Grupo 18** · Josef Wojtyla Barros de Souza e Zayra Moraes

---

## Resumo

O compilador está completo de ponta a ponta: recebe um programa em Portugol (`.pt`), faz as análises léxica, sintática e semântica, otimiza, gera código de três endereços (TAC) e traduz o TAC para um programa Python executável.

```
arquivo.pt → Flex → Bison (AST) → semântica → otimização da AST → TAC → otimização do TAC → arquivo.py
```

```bash
make
build/portugol programa.pt -o programa.py
python3 programa.py
```

Desde o Ponto de Controle 2 foram entregues:

- ✅ **Simplificação de expressões** na AST: dobragem de constantes e identidades algébricas (issue #51, PR #55)
- ✅ **Remoção de código morto** no TAC: código inalcançável e temporários não usados (issue #50, PR #55)
- ✅ **Código final em Python** a partir do TAC, com o esquema de máquina de estados (`_pc`) para os saltos (PRs #59 e #60)
- ✅ **Testes ponta a ponta**: cada programa válido é compilado, executado e tem a saída comparada (PR #61)
- ✅ Opção `-o` para o arquivo de saída e alvos `make run` e `make debug`
- ✅ Documentação final (README, código final, revisão das páginas do compilador)

---

## Funcionalidades

| Fase | Arquivo | O que faz |
|---|---|---|
| Léxica | `lexer/portugol.l` | palavras-chave, operadores, literais, identificadores com acento, comentários, erro léxico com linha |
| Sintática | `parser/portugol.y` | gramática com precedência, construção da AST, erro sintático com linha |
| Tabela de símbolos | `src/simbolos.c` | escopo global e local, redeclaração, parâmetros, tipo de retorno |
| Semântica | `src/semantica.c` | variável/função não declarada, tipos em atribuição e operadores, condição lógica, número e tipo de argumentos, retorno garantido |
| Otimização da AST | `src/ast.c` (`otimizar_ast`) | dobragem de constantes e identidades (`x + 0`, `x * 1`, `x * 0`...) |
| Código intermediário | `src/codigo_intermediario.c` | quádruplas para expressões, `se`, `enquanto`, `para`, funções e E/S |
| Otimização do TAC | `src/codigo_intermediario.c` (`otimiza_tac`) | remove código inalcançável e temporários mortos |
| Código final | `src/codigo_final.c` | TAC → Python, conversão para `float` em variáveis `real`, `global`, divisão inteira |

**Linguagem suportada:** `inteiro`, `real`, `cadeia`, `logico`; variáveis globais e locais; `+ - * / %`, relacionais, `e`/`ou`/`nao`; `++ -- += -= *= /=`; `se/senao`, `enquanto`, `para`; funções com parâmetros, `retorne` e recursão; `escreva` e `leia`.

**Opções do compilador:** `-o arquivo.py`, `--tac`, `--ast`, `--simbolos`.

---

## Limitações conhecidas

| Limitação | Detalhe |
|---|---|
| **Vetores** | O parser reconhece `inteiro v[10]` e `v[i]`, mas os vetores não entram na AST; não há semântica, TAC nem Python para eles. Foi uma decisão da equipe priorizar o núcleo do compilador. O design das instruções (`=[]` e `[]=`) está em [código intermediário](../compilador/codigo-intermediario.md#56-vetores-fora-do-escopo-por-ora) |
| Curto-circuito | `e` e `ou` avaliam os dois lados |
| `escreva` | aceita uma única expressão por chamada |
| Linha na tabela de símbolos | a coluna `Linha` do `--simbolos` pode aparecer uma linha depois da declaração |
| Nome com tipos diferentes em escopos diferentes | o código final não sabe qual tipo aplicar e não converte para `float` |
| Variável sem inicialização | gera aviso, não erro |

---

## Resultados dos testes

```bash
make clean && make test
```

| Suíte | Pasta | Casos | Resultado |
|---|---|---|---|
| Base: programas válidos | `test/valid/` | 21 | 21 PASS |
| Base: programas com erro | `test/error/` | 10 | 10 PASS |
| Semântica (erro + mensagem) | `test/semantica/` | 7 | 7 PASS |
| Código intermediário | `test/tac/` | 7 | 7 PASS |
| Ponta a ponta (Python executado) | `test/e2e/` | 21 | 21 PASS |
| **Total** | | **66** | **66 PASS, 0 FAIL** |

Os testes de vetor foram retirados das suítes por estarem fora do escopo. O catálogo dos casos está em `TEST_CASES.md`.

### Exemplo de execução

`test/valid/funcao_recursiva.pt` calcula `fibonacci(5)` recursivamente; o Python gerado imprime `5`. O passo a passo (fonte, TAC e Python) está no [README](https://github.com/JosefWojtyla/G18_COMP1_26_2#exemplo-completo) e em [código final](../compilador/codigo-final.md#5-exemplo-antesdepois).

---

## Divisão do trabalho

| Parte | Josef | Zayra |
|---|:-:|:-:|
| Ambiente, Makefile e GitHub Pages | ✅ | ✅ |
| Analisador léxico e gramática | ✅ | ✅ |
| Tabela de símbolos e AST | ✅ | ✅ |
| Análise semântica | ✅ | ✅ |
| Código intermediário (TAC) | ✅ | ✅ |
| Otimização: simplificação de expressões e código morto (#50, #51) | ✅ | |
| Código final em Python (PRs #59, #60) | | ✅ |
| Testes ponta a ponta (PR #61) | ✅ | |
| Documentação dos pontos de controle | ✅ | ✅ |

> Conferir e ajustar a tabela antes da entrega: ela foi montada a partir do histórico de commits e PRs.

---

## Uso de IA

- Revisão de documentação e conferência do código contra o planejamento.
- Material didático de referência para o código intermediário.
- Apoio na redação da documentação final a partir do conteúdo do repositório.

---

**Status:** entregue
