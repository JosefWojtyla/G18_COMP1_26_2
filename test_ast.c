#include <stdio.h>
#include "ast.h"

int main(void){
    // simula expressão (20 + 10)
    AstNode *n1 = criar_no_inteiro(10, 1);
    AstNode *n2 = criar_no_inteiro(20, 1);
    AstNode *soma = criar_no_op_binaria('+', n1, n2, 1);

    printf("Nó raiz criado. Tipo: %d, op: %c\n", soma->tipo, soma->dados.criar_no_op_binaria.op);

    liberar_ast(soma);

    printf("Memoria liberada com sucesso.\n");
    return 0;
}