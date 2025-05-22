#ifndef AST_H
#define AST_H

typedef enum {
    NODE_PROGRAM,
    NODE_DECL_LIST,
    NODE_STMT_LIST,
    NODE_DECL,
    NODE_NUM,
    NODE_VAR,
    NODE_BIN_OP,
    NODE_PRINT,
    NODE_IF,
    NODE_FOR,
    NODE_RANGE
} NodeType;

typedef struct ASTNode {
    NodeType type;
    union {
        int num;
        float fnum;
        char *str;
        struct { struct ASTNode *left, *right; } children;
    };
} ASTNode;

ASTNode *createProgramNode(ASTNode *decls, ASTNode *stmts);
ASTNode *createDeclList(ASTNode *decl, ASTNode *decls);
ASTNode *createStmtList(ASTNode *stmt, ASTNode *stmts);
ASTNode *createDeclNode(char *id, char *type, ASTNode *expr);
ASTNode *createNumNode(int num);
ASTNode *createVarNode(char *id);
ASTNode *createBinaryOpNode(ASTNode *left, ASTNode *right, char *op);
ASTNode *createPrintNode(ASTNode *expr);
ASTNode *createIfNode(ASTNode *cond, ASTNode *body);
ASTNode *createForNode(char *var, ASTNode *range, ASTNode *body);

#endif
