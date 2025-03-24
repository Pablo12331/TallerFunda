%{
#include "ast.h"
#include "codegen.h"
extern FILE *yyin;
ASTNode *root;  // Nodo raíz del AST (usado en main.c)
%}

%union {
    int num;
    float fnum;
    char *str;
    ASTNode *node;
}

// Tokens (definidos en lexer.l)
%token T_INT T_FLOAT T_STRING
%token T_IF T_ELSE T_WHILE T_FOR T_IN T_PRINT T_READ
%token T_EQ T_NEQ T_LEQ T_GEQ T_RANGE
%token <num> T_NUM
%token <fnum> T_FNUM
%token <str> T_STRING_LIT T_ID

// No terminales
%type <node> program declaration statement expression range

%%

program:
    declaration* statement* { $$ = createProgramNode($1, $2); root = $$; }
    ;

declaration:
    T_INT T_ID ';' { $$ = createDeclNode($2, "int", NULL); }
    | T_INT T_ID '=' expression ';' { $$ = createDeclNode($2, "int", $4); }
    ;

statement:
    T_PRINT '(' expression ')' ';' { $$ = createPrintNode($3); }
    | T_IF '(' expression ')' '{' statement* '}' { $$ = createIfNode($3, $6); }
    | T_FOR T_ID T_IN range '{' statement* '}' { $$ = createForNode($2, $4, $6); }
    ;

expression:
    T_NUM { $$ = createNumNode($1); }
    | T_ID { $$ = createVarNode($1); }
    | expression T_RANGE expression { $$ = createRangeNode($1, $3); }
    ;

range:
    expression T_RANGE expression { $$ = createRangeNode($1, $3); }
    ;

%%