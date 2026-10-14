#ifndef SIMBOLO_H
#define SIMBOLO_H

typedef struct{
    char *nome; // nome do identificador (ID)
    char *tipo; // inteiro, real, cadeia, logico
    int eh_array; // 1 se é vetor, 0 caso contrário
    int tamanho_array; //tamanho se for um vetor
    int linha_declaracao; // linha onde foi declarado
    int escopo; //0 = global, 1+ = local (nivel)
    int inicializado; // 1 se foi inicializado, 0 c.c.
    char *valor_padrao; // valor padrão se não inicializado 
} Simbolo;

typedef struct{
    Simbolo *simbolos[1000]; // array de simbolos
    int count; // numero de simbolos
    int escopo_atual; // nivel de escopo atual
} TabelaSimbolos;


void criarTabelaSimbolos(TabelaSimbolos *tabela);
int inserirSimbolo(TabelaSimbolos *tabela, char *nome, char *tipo, int linha, int escopo);
Simbolo *buscarSimbolo(TabelaSimbolos *tabela, char *nome);
int verificarDuplicata(TabelaSimbolos *tabela, char *nome, int escopo);
void limparTabelaSimbolos(TabelaSimbolos *tabela);

#endif