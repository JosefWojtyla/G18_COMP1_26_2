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
    
    if(tabela == NULL || nome == NULL){
        return 1;
    }
    // um simbolo só é duplicado se ele tiver o mesmo nome e tiver no mesmo escopo do que está na tabela
    for(int i = 0; i < tabela->count; i++){
        if(strcmp(tabela->simbolos[i]->nome, nome) == 0 && tabela->simbolos[i]->escopo == escopo){
            return 1;
        }
    }

    return 0;
}

int inserirSimbolo(TabelaSimbolos *tabela, char *nome, char *tipo, int linha, int escopo){

    if(tabela->count >= 1000)
        return 0;

    if(verificarDuplicata(tabela, nome, escopo))
        return 0;

    Simbolo *novo = (Simbolo *) malloc(sizeof(Simbolo));
    
    if(novo == NULL)
        return 0;

    novo->nome = strdup(nome);
    novo->tipo = strdup(tipo);
    novo->eh_array = 0;
    novo->tamanho_array = 0; 
    novo->linha_declaracao = linha;
    novo->escopo = escopo;
    novo->inicializado = 0;
    novo->valor_padrao = NULL;

    tabela->simbolos[tabela->count] = novo;
    tabela->count++;

    return 1; // passou por todos os casos e inseriu com sucesso

}

Simbolo *buscarSimbolo(TabelaSimbolos *tabela, char *nome){

    
}