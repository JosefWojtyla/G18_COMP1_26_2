#include "simbolos.h"
#include <stdlib.h>
#include <string.h>

void criarTabelaSimbolos(TabelaSimbolos *tabela){

    if(tabela == NULL)
        return;
    
    tabela->count = 0;
    tabela->escopo_atual = 0;

    for(int i = 0; i < 1000; i++){
        tabela->simbolos[i] = NULL;
    }
}

int verificarDuplicata(TabelaSimbolos *tabela, char *nome, int escopo){

}

int inserirSimbolo(TabelaSimbolos *tabela, char *nome, char *tipo, int linha, int escopo){

    if(tabela->count >= 1000)
        return 0;

    if(verificarDuplicata(tabela, nome, tipo, linha, escopo))
        return 0;

    Simbolo *novo = (Simbolo *) malloc(sizeof(Simbolo));
    
    if(novo == NULL)
        return 0;

    novo->linha_declaracao = linha;
    novo->escopo = escopo;
    novo->nome = strdup(nome);
    novo->tipo = strdup(tipo);
    novo->eh_array = 0;
    novo->tamanho_array = 0; //nao deve ser zero, ne?]
    novo->inicializado = 0;

    tabela->simbolos[tabela->count] = novo;
    tabela->count++;

    return 1; // passou por todos os casos e inseriu com sucesso

}