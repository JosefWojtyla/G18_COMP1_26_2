#include "simbolos.h"
#include "stdlib.h"

void criarTabelaSimbolos(TabelaSimbolos *tabela){

    if(tabela == NULL)
        return;
    
    tabela->count = 0;
    tabela->escopo_atual = 0;

    for(int i = 0; i < 1000; i++){
        tabela->simbolos[i] = NULL;
    }
}

int inserirSimbolo(TabelaSimbolos *tabela, char *nome, char *tipo, int linha, int escopo){

    if(tabela->simbolos[1000] != NULL || tabela->simbolos-> == escopo)
        return;

    

}