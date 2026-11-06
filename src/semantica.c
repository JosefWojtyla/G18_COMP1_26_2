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
                return s->tipo;
            }
        }

            case NO_CHAMADA: {
                Simbolo *s = buscarSimbolo(tabela, no->dados.chamada.nome)
                if(s == NULL){
                    fprintf(stderr, "Erro semântico: função não declarada.\n");
                    qtd_erros++;
                } else {
                    return s->tipo;
                }
            }

            case NO_OP_BINARIA: {
            const char *t_esq = inferir_tipo(no->dados.op_binaria.esq, tabela);
            const char *t_dir = inferir_tipo(no->dados.op_binaria.dir, tabela);
            char *op = no->dados.op_binaria.op;
            // 1. Operador unário "nao": exige t_dir == "logico", retorna "logico"
            // 2. Operadores lógicos ("e", "ou"): exigem t_esq == "logico" e t_dir == "logico", retornam "logico"
            // 3. Relacionais ("<", ">", "<=", ">=", "==", "!="): retornam "logico"
            // 4. Aritméticos ("+", "-", "*", "/", "%"):
            //    - Se ambos forem "inteiro" -> retorna "inteiro"
            //    - Se um for "real" e outro "inteiro" ou "real" -> retorna "real" (ex: 1 + 2.5 -> "real")
            //    - Caso contrário -> emite erro indicando tipo esperado e encontrado!
            // nao entendi como implementar
            }
        
            default:
                return NULL;
    
    }
}