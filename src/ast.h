#ifndef AST_H
#define AST_H

typedef enum{
    NO_INTEIRO,
    NO_REAL,
    NO_CADEIA,
    NO_LOGICO,
    NO_OP_BINARIA,
    NO_SE,
    NO_ENQUANTO,
    NO_PARA,
    NO_RETORNE,
    NO_ATRIBUICAO,
} TipoNo;

typedef struct AstNode AstNode;

struct AstNode{
    TipoNo tipo; // tag do polimorfismo
    int linha; // rastramento de erros semanticos
    
    // reune os dados das subclasses
    union{
        // inteiro
        struct{
            int valor;
        } inteiro;
        // real
        struct{
            float valor;
        } real;
        // cadeia
        struct{
            char *valor;
        } cadeia;
        // logico
        struct{
            int valor; // 1 pra true e 0 pra false???
        } logico;
        
        // operação
        struct{
            char op; // + | - | * | / |
            AstNode *esq; // subarvore pra esq
            AstNode *dir; // pra dir 
        } op_binaria 

        // se - senao
        struct{
            AstNode *condicao;
            AstNode *entao;
            AstNode *senao; // podendo ser NULL
        } comando_se

        // enquanto
        struct{
            AstNode *condicao;
            AstNode *entao;
        } comando_enquanto

        // para
        struct{
            AstNode *inicializa;
            AstNode *condicao;
            AstNode *entao;
            AstNode *op_inicializa;
        } comando_se

        // retorne
        struct{
            // ??
        } comando_retorne

        // funcao
        struct{
            // ??
        } comando_funcao

        // não to lembrando de todos...
    } dados;
};

// "construtor"
AstNode *criar_no_inteiro(int valor, int linha);
AstNode *criar_no_real(float valor, int linha);
AstNode *criar_no_cadeia(char *valor, int linha);
AstNode *criar_no_logico(int valor, int linha);
AstNode *criar_no_op_binaria(char op, AstNode *esq, AstNode *dir, int linha);
AstNode *criar_no_se(AstNode *condicao, AstNode *entao, AstNode *senao, int linha);
AstNode *criar_no_enquanto(AstNode *condicao, AstNode *entao, int linha);
AstNode *criar_no_para(AstNode *inicializa, AstNode *condicao, AstNode *entao, AstNode *op_inicializa, int linha);

// free na memoria
void liber_ast(AstNode *no);

#endif