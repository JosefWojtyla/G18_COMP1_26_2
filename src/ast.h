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
            int valor; // 1 = true e 0 = false
        } logico;
        
        // operação
        struct{
            char *op; // + , - , * , / 
            AstNode *esq; // subarvore pra esq
            AstNode *dir; // pra dir 
        } op_binaria;

        // se - senao
        struct{
            AstNode *condicao;
            AstNode *entao;
            AstNode *senao; // podendo ser NULL
        } se;

        // enquanto
        struct{
            AstNode *condicao;
            AstNode *entao;
        } enquanto;

        // para
        struct{
            AstNode *inicializa;
            AstNode *condicao;
            AstNode *entao;
            AstNode *passo;
        } para;

        // retorne
        struct{
            AstNode *expressao; // valor a ser retornado
        } retorne;

        // funcao
        struct{
            char *nome; // nome de identificação da função
            char *tipo_retorno; // se é "inteiro", "real"...
            AstNode *parametros; // lista de parametros
            AstNode *corpo; // bloco de comando da função
        } funcao;

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
void liberar_ast(AstNode *no);

#endif