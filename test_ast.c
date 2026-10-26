#include <stdio.h>
#include "ast.h"

int main(void){
    // simula expressão (20 + 10)
    AstNode *n1 = criar_no_inteiro(10, 1);
    AstNode *n2 = criar_no_inteiro(20, 1);
    AstNode *soma = criar_no_op_binaria("+", n1, n2, 1);

    printf("No raiz criado. Tipo: %d, op: %s\n", soma->tipo, soma->dados.op_binaria.op);

    liberar_ast(soma);

    printf("Memoria liberada com sucesso.\n");
    return 0;
}