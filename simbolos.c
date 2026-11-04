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

    if(tabela == NULL) 
        return 0;

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

    if(tabela == NULL || nome == NULL)
        return NULL;
    // na tabela de simbolos, variaveis com o mesmo nome e com escopo diferentes, são guardadas numa ordem:
    // primeiro vem a global e depois vem as locais.
    // primeiro devemos buscar a de escopo mais atual
    // somente rodar um for de trás pra frente
    for(int i = tabela->count - 1; i >= 0; i--){
        if(strcmp(tabela->simbolos[i]->nome, nome) == 0){
            // retornar o simbolo encontrado
            return tabela->simbolos[i];
        }
    }
    
    return NULL;
}

void limparTabelaSimbolos(TabelaSimbolos *tabela){

    if(tabela == NULL)
        return;

    for(int i = 0; i < tabela->count; i++){
        if(tabela->simbolos[i] != NULL){
            free(tabela->simbolos[i]->nome);
            free(tabela->simbolos[i]->tipo);
            free(tabela->simbolos[i]);

            tabela->simbolos[i] = NULL;
        }
    }

    tabela->count = 0; // ? ou deveria decrementar dentro do for? mas iria atingir a logica do for
    tabela->escopo_atual = 0;
}


