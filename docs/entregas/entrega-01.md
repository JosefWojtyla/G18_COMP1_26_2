# Ponto de controle 1

## Compilador Portugol Studio UNIVALI → Python | Sprint 2



### Número de Sprints Realizadas
**2 sprints completas:**

- **S1 (01-14/09)** 
- **S2 (15-28/09)** 

---

### Objetivos S1 e S2 

**S1 - Alcançados:**

- ✅ Ambiente configurado (Flex, Bison, GCC)
- ✅ Escopo documentado (versão UNIVALI)
- ✅ Repositório GitHub estruturado
- ✅ Arquivos `.l` e `.y` iniciais
- ✅ GitPages para documentação

**S2 - Alcançados:**

- ✅ Expressões regulares completas (Flex)
- ✅ Gramática sintática funcional (Bison)
- ✅ 11 casos de teste (válidos e erros)
- ✅ Makefile automatizado
- ✅ Documentação técnica

---

### Principais Entregas Implementadas

- Analisador Léxico (Flex)
```
- Palavras-chave (programa, funcao, inicio, se, entao, senao, enquanto, etc)
- Tipos primitivos (inteiro, real, cadeia, logico)
- Operadores aritméticos (+, -, *, /, %)
- Operadores relacionais (<, >, <=, >=, ==, !=)
- Operadores lógicos (e, ou, nao)
- Operadores incremento/decremento (++, --, +=, -=, *=, /=)
- Identificadores com acentos (português)
- Números inteiros e reais
- Strings entre aspas duplas
- Comentários // e /* */
- Detecção de erros com número da linha
```

#### Analisador Sintático (Bison)
```
- Estrutura programa { funcao inicio() { ... } }
- Declaração de variáveis (inteiro x, real y, etc)
- Atribuição simples (x = 5)
- Atribuição com operadores (x += 2, x++, etc)
- Comandos I/O (escreva(), leia())
- Condicional se/entao/senao (com aninhamento)
- Repetição enquanto
- Expressões aritméticas, lógicas e relacionais
- Precedência de operadores configurada
- Mensagens de erro sintático claras
```

#### Infraestrutura
```
- Makefile 
- 11 casos de teste automatizados

- Documentação em GitPages
```

---

### Principais Dificuldades e Soluções

| Dificuldade | Solução |
|-------------|---------|
| Compatibilidade com variantes Portugol | Documentar explicitamente a versão UNIVALI e manter escopo versionado |
| Ordem de regras Flex (operadores compostos) | Colocar `>=`, `<=` antes de `>`, `<` |
| Conflitos shift/reduce no Bison | Configurar precedência de operadores com `%left` e `%right` |
| Sincronização Flex ↔ Bison | Usar `#include "portugol.tab.h"` no `.l` para tokens |

---

### O Que Ainda Falta (Sprint 3+)

**Para P2 (Sprint 4):**

- Análise semântica (tabela de símbolos, verificação de tipos)
- Funções definidas pelo usuário
- Vetores unidimensionais
- Comando `para` (for)
- Geração de código intermediário

**Para Entrega Final (Sprint 5):**

- Geração de Python executável
- Otimizações básicas
- Testes integrados ponta-a-ponta

---



### Execução


**Execução:** `make test` → relatório automático

---

###  Resumo Objetivo do Progresso

Implementamos com sucesso as **fases de análise léxica e sintática** de um compilador Portugol → Python. O léxico reconhece todos os tokens (palavras-chave, operadores, literais, comentários). O parser valida estrutura do programa, declarações, comandos de I/O, condicionais e repetições. 11 casos de teste automatizados garantem qualidade. Documentação completa em GitPages.

---


## Métricas P1

| Métrica | Valor |
|---------|-------|
| Linhas Flex | ~100 |
| Linhas Bison | ~150 |
| Tokens suportados | 40+ |
| Regras gramaticais | 15+ |
| Casos de teste | 11 |
| Documentação |  Completa até o momento |
| Código testável |  Sim |

> Métricas colhidas com o auxilio da IA

---

**Data:** 23/09/2026  
**Status:**  Pronto para apresentação  
**Próxima entrega:** P2 (Sprint 4, ~4 semanas)