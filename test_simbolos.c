#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "simbolos.h"

int main(){

    printf("Testes da Tabela de Simbolos...\n");

    TabelaSimbolos tabela;
    criarTabelaSimbolos(&tabela);
    assert(tabela.count == 0);

    // 1.teste de inserção simples
    int ok = inserirSimbolo(&tabela, "x", "inteiro", 1, 0);
    assert(ok == 1);
    assert(tabela.count == 1);

    // 2.teste de duplicata
    int duplicado = inserirSimbolo(&tabela, "x", "inteiro", 2, 0);
    assert(duplicado == 0); // não deve permitir
    assert(tabela.count == 1);

    // 3.teste de inserção com o mesmo nome em escopo diferente
    int local = inserirSimbolo(&tabela, "x", "real", 5, 1);
    assert(local == 1);
    assert(tabela.count == 2);

    // 4.teste de busca de sombreamento (qual seria outra forma de dizer isso?)
    Simbolo *s = buscarSimbolo(&tabela, "x");
    assert(s != NULL);
    assert(s->escopo == 1); // achar o local mais recente
    assert(strcmp(s->tipo, "real") == 0);

    // 5.teste de busca de simbolo inexistente
    assert(buscarSimbolo(&tabela, "y") == NULL);
    assert(tabela.count == 2); // qtd de simbolos na tabela, só tem o 'x' do escopo 0 e do escopo 1

    // 6.teste de limpeza da tabela
    limparTabelaSimbolos(&tabela);
    assert(tabela.count == 0);

    assert(buscarSimbolo(&tabela, "x") == NULL);

    // todos os asserts deu true
    printf("Todos os testes passaram com sucesso!\n");

    return 0;
}