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

// ... (implementaciones similares para otras funciones) ...