#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "simbolos.h"

extern int qtd_erros;

const char *inferir_tipo(AstNode *no, TabelaSimbolos *tabela){
    if(no == NULL) return NULL;

    switch(no->tipo){
        case NO_INTEIRO: return "inteiro";
        case NO_REAL:    return "real";
        case NO_CADEIA:  return "cadeia";
        case NO_LOGICO:  return "logico";
        case NO_VARIAVEL: {
            Simbolo *s = buscarSimbolo(tabela, no->dados.variavel.nome);
            if(s == NULL){
                fprintf(stderr, "Erro semantico na linha %d: Variavel '%s' nao foi declarada.\n", no->linha, no->dados.variavel.nome);
                qtd_erros++;
                return NULL;
            }

            if(!s->inicializado){
                fprintf(stderr, "Aviso na linha %d: Variavel '%s' usada sem ser inicializada.\n", no->linha, no->dados.variavel.nome);
            }
            return s->tipo;
        }

            case NO_CHAMADA: {
                Simbolo *s = buscarSimbolo(tabela, no->dados.chamada.nome);
                if(s == NULL){
                    fprintf(stderr, "Erro semântico: função não declarada.\n");
                    qtd_erros++;
                    return NULL;
                } else {
                    return s->tipo;
                }
            }

            case NO_OP_BINARIA: {
            const char *t_esq = inferir_tipo(no->dados.op_binaria.esq, tabela);
            const char *t_dir = inferir_tipo(no->dados.op_binaria.dir, tabela);
            char *op = no->dados.op_binaria.op;
            
            if(strcmp(op, "nao") == 0){
                if(t_dir == NULL) return NULL;

                if(strcmp(t_dir, "logico") != 0){
                    fprintf(stderr, "Erro semantico na linha %d: Operador 'nao' espera 'logico', mas recebeu '%s'.\n", no->linha, t_dir);
                    qtd_erros++;

                }
                return "logico";
            }
            if (t_esq == NULL || t_dir == NULL) return NULL;

            if(strcmp(op, "e") == 0 || strcmp(op, "ou") == 0){
                if(strcmp(t_esq, "logico") != 0 || strcmp(t_dir, "logico") != 0){
                    fprintf(stderr, "Erro semantico na linha %d: Operador '%s' espera tipo 'logico', mas encontrou '%s' e '%s'.\n", no->linha, op, t_esq, t_dir);
                    qtd_erros++;
                }
                return "logico";
            }

            if (strcmp(op, "<") == 0 || strcmp(op, ">") == 0 || strcmp(op, "<=") == 0 || strcmp(op, ">=") == 0 || strcmp(op, "==") == 0 || strcmp(op, "!=") == 0){
                return "logico";
            }

            if(strcmp(t_esq, "inteiro") == 0 && strcmp(t_dir, "inteiro") == 0){
                return "inteiro";
            }

            if((strcmp(t_esq, "inteiro") == 0 || strcmp(t_esq, "real") == 0) && (strcmp(t_dir, "inteiro") == 0 || strcmp(t_dir, "real") == 0)){
                return "real";
            }

                fprintf(stderr, "Erro semantico na linha %d: Operacao '%s' esperava tipos numericos ('inteiro' ou 'real'), mas encontrou '%s' e '%s'.\n", no->linha, op, t_esq, t_dir);
                qtd_erros++;
                return NULL;
            }
        
            default:
                return NULL;
    
    }
}

void verificar_atribuicao(const char *tipo_var, AstNode *expressao, int linha, TabelaSimbolos *tabela){
    const char *tipo_expr = inferir_tipo(expressao, tabela);

    if(tipo_var == NULL || tipo_expr == NULL) return;

    if(strcmp(tipo_var, tipo_expr) == 0) return;

    if(strcmp(tipo_var, "real") == 0 && strcmp(tipo_expr, "inteiro") == 0) return;

    fprintf(stderr, "Erro semantico na linha %d: Tipo incompativel na atribuicao. Esperado '%s', mas encontrado '%s'.\n",
    linha, tipo_var, tipo_expr);
    qtd_erros++;
}

void verificar_condicao(AstNode *condicao, int linha, TabelaSimbolos *tabela){
    const char *tipo_cond = inferir_tipo(condicao, tabela);
    
    if(tipo_cond == NULL) return;

    if(strcmp(tipo_cond, "logico") != 0){
        fprintf(stderr, "Erro semantico na linha %d: Condicao invalida. Esperado 'logico', mas encontrado '%s'.\n",
        linha, tipo_cond);
        qtd_erros++;
    }
}