#ifndef CODIGO_FINAL_H
#define CODIGO_FINAL_H

#include <stdio.h>
#include "codigo_intermediario.h"

// Traduz a lista de instruções TAC (já otimizada) para um programa Python
// e escreve em 'saida'. 'lista' vem de obtem_tac().
void gerar_codigo_final(Instrucao *lista, FILE *saida);

#endif
