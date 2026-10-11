# Tabela de Símbolos

## 1. Visão Geral

A Tabela de Símbolos atua como o catálogo central de identificadores durante a compilação. Ela armazena metadados de variáveis e funções (como nome, tipo, linha e escopo), permitindo que o analisador semântico valide declarações duplicadas, cheque tipos e verifique se variáveis foram declaradas antes do uso.

## 2. Estrutura de Dados
O design foi construído em linguagem C utilizando duas estruturas principais:

1. **Struct `Simbolo`:** Representa a ficha cadastral de cada identificador:
    - `nome`: Nome do identificador (alocado dinamicamente).
    - `tipo`: Tipo primitivo (`"inteiro", "real", "cadeia", "lógico"`).
    - `linha_declaracao`: Linha do código-fonte onde foi declarado (para emissão de erros).
    - `escopo`: Nível de visibilidade (0 = global, 1+ = local).
    - `eh_array` e `tamanho_array`: Suporte a vetores unidimensionais.
    - `inicializado` e `valor_padrao`: Controle de atribuição inicial.

2. **Struct `TabelaSimbolos`:** Estrutura gerenciadora:
    - `simbolos[1000]`: Array de ponteiros para os símbolos cadastrados.
    - `count`: Quantidade de símbolos atualmente armazenados.
    - `escopo_atual`: Nível de escopo em análise no momento.

## 3. Decisões de Design & Complexidade
- **Uso de Array de Ponteiros**: Permite inicialização leve e alocação sob demanda via `malloc`. As posições não ocupadas permanecem como `NULL`.
- **Prevalência de Escopo (Shadowing)**: A busca é realizada do final para o início (`count - 1` até `0`), garantindo que a declaração no escopo mais interno/recente seja encontrada primeiro.
- **Complexidade Algorítmica**:
  - **Inserção**: $O(N)$ devido à checagem prévia de duplicata.
  - **Busca**: $O(N)$ percorrendo linearmente os símbolos registrados.
  - **Limpeza**: $O(N)$ para liberação segura de memória (`free`).

## Planejamento para Escopo Hierárquico (Sprint 4)
Para suportar blocos aninhados (`se`, `enquanto`, funções):
- Ao entrar em um novo bloco, o compilador incrementará `escopo_atual`.
- Ao sair do bloco, a tabela implementará uma política de descarte dos símbolos cujo `escopo == escopo_atual`, restaurando o escopo pai (`escopo_atual--`).

## Interface Pública (API)
 `void criarTabelaSimbolos(TabelaSimbolos *tabela)`: Inicializa os campos da tabela.
- `int inserirSimbolo(TabelaSimbolos *tabela, char *nome, char *tipo, int linha, int escopo)`: Valida e cadastra um novo identificador.
- `int verificarDuplicata(TabelaSimbolos *tabela, char *nome, int escopo)`: Checa conflito de nomes no mesmo escopo.
- `Simbolo *buscarSimbolo(TabelaSimbolos *tabela, char *nome)`: Retorna o ponteiro do símbolo visível ou `NULL`.
- `void limparTabelaSimbolos(TabelaSimbolos *tabela)`: Desaloca toda memória dinâmica alocada.

