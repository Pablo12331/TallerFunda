#include "codegen.h"
#include <stdio.h>

void generateCode(ASTNode *node) {
    if (!node) return;

    switch (node->type) {
        case NODE_PROGRAM:
            printf("#include <iostream>\nusing namespace std;\nint main() {\n");
            generateCode(node->children.left);  // Declaraciones
            generateCode(node->children.right); // Sentencias
            printf("return 0;\n}\n");
            break;
        case NODE_DECL:
            printf("int %s", node->str);
            if (node->children.right) {
                printf(" = ");
                generateCode(node->children.right);
            }
            printf(";\n");
            break;
        // ... (otros casos) ...
    }
}