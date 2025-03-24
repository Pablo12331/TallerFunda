%{
#include "ast.h"
#include "codegen.h"
extern FILE *yyin;
ASTNode *root;
%}

%union {
    int num;
    float fnum;
    char *str;
    ASTNode *node;
}

// Precedencia para operadores
%left '+' '-'
%left '*' '/'

// Tokens
%token T_INT T_FLOAT T_PRINT T_IF T_ELSE T_WHILE T_FOR T_IN
%token T_EQ T_NEQ T_LEQ T_GEQ T_RANGE
%token <num> T_NUM
%token <fnum> T_FNUM
%token <str> T_STRING T_ID

// Tipos de no terminales
%type <node> program declarations statements declaration statement expression range

%%

program:
    declarations statements { $$ = createProgramNode($1, $2); root = $$; }
    ;

declarations:
    /* Vacío */ { $$ = NULL; }
    | declaration declarations { $$ = createDeclList($1, $2); }
    ;

statements:
    /* Vacío */ { $$ = NULL; }
    | statement statements { $$ = createStmtList($1, $2); }
    ;

declaration:
    T_INT T_ID ';' { $$ = createDeclNode($2, "int", NULL); }
    | T_INT T_ID '=' expression ';' { $$ = createDeclNode($2, "int", $4); }
    ;

statement:
    T_PRINT '(' expression ')' ';' { $$ = createPrintNode($3); }
    | T_IF '(' expression ')' '{' statements '}' { $$ = createIfNode($3, $6); }
    | T_FOR T_ID T_IN range '{' statements '}' { $$ = createForNode($2, $4, $6); }
    ;

expression:
    T_NUM { $$ = createNumNode($1); }
    | T_ID { $$ = createVarNode($1); }
    | expression '+' expression { $$ = createBinaryOpNode($1, $3, "+"); }
    | expression '-' expression { $$ = createBinaryOpNode($1, $3, "-"); }
    | expression '*' expression { $$ = createBinaryOpNode($1, $3, "*"); }
    | expression '/' expression { $$ = createBinaryOpNode($1, $3, "/"); }
    | '(' expression ')' { $$ = $2; }
    ;

range:
    expression T_RANGE expression { $$ = createRangeNode($1, $3); }
    ;

%%