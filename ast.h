#ifndef AST_H
#define AST_H

typedef enum {
    NODE_PROGRAM,
    NODE_DECL,
    NODE_NUM,
    NODE_VAR,
    NODE_RANGE,
    NODE_PRINT,
    NODE_IF,
    NODE_FOR
} NodeType;

typedef struct ASTNode {
    NodeType type;
    union {
        int num;
        char *str;
        struct { struct ASTNode *left, *right; } children;
    };
} ASTNode;

// Funciones para crear nodos
ASTNode *createProgramNode(ASTNode *decls, ASTNode *stmts);
ASTNode *createDeclNode(char *id, char *type, ASTNode *expr);
ASTNode *createNumNode(int num);
ASTNode *createVarNode(char *id);
ASTNode *createRangeNode(ASTNode *start, ASTNode *end);
ASTNode *createPrintNode(ASTNode *expr);
ASTNode *createIfNode(ASTNode *cond, ASTNode *body);
ASTNode *createForNode(char *var, ASTNode *range, ASTNode *body);

#endif