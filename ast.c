#include "ast.h"
#include <stdlib.h>
#include <string.h>

ASTNode *createProgramNode(ASTNode *decls, ASTNode *stmts) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_PROGRAM;
    node->children.left = decls;
    node->children.right = stmts;
    return node;
}

ASTNode *createDeclList(ASTNode *decl, ASTNode *decls) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_DECL_LIST;
    node->children.left = decl;
    node->children.right = decls;
    return node;
}

ASTNode *createStmtList(ASTNode *stmt, ASTNode *stmts) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_STMT_LIST;
    node->children.left = stmt;
    node->children.right = stmts;
    return node;
}

// ... Implementa el resto de funciones siguiendo el mismo patrón ...