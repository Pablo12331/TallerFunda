#include <stdio.h>
#include "ast.h"
#include "parser.tab.h"

extern ASTNode *root;  // Definido en parser.y
extern int yyparse();

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror("Error opening file");
        return 1;
    }

    yyparse();  // Ejecuta el parser (llena 'root' con el AST)
    fclose(yyin);

    if (root) {
        generateCode(root);  // Genera código C++
    }

    return 0;
}

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}