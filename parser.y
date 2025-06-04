%{
#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "Ast.h" 

extern int yylex();
extern int yylineno;
extern char *yytext;
extern FILE *yyin;
void yyerror(const char *s);

Node* globalAstRoot = nullptr;

%}

%code requires {
    typedef struct Node Node;
}

%union {
    int ival;
    char *sval;
    double fval;
    Node *nodePtr;
}


%token <ival> INT FLOAT STRING_TYPE VOID
%token <ival> EQ NE LT GT LE GE
%token IF ELSE WHILE FOR PRINT READ RETURN
%token <ival> AND_OP OR_OP NOT_OP
%token BOOL_TYPE

%token LBRACE RBRACE LPAREN RPAREN SEMICOLON ASSIGN ','

%token <ival> PLUS MINUS MULT DIV
%token <ival> NUMBER
%token <fval> FLOAT_NUMBER
%token <sval> IDENTIFIER
%token <sval> STRING_LITERAL
%token <ival> TRUE_LITERAL FALSE_LITERAL


%type <nodePtr> program statement_list statement function var_declaration assignment io_statement control_statement type
%type <nodePtr> expression term factor statements_opt function_signature opt_expression param_list params_opt param arg_list args_opt


%start program

%right ASSIGN
%left  OR_OP
%left  AND_OP
%left EQ NE LT GT LE GE
%left PLUS MINUS
%left MULT DIV
%right NOT_OP

%%

program:
    statement_list
    { $$ = $1; globalAstRoot = $$; }
    ;

statement_list:
      statement
      { $$ = createStatementListNode($1, $1 ? $1->line : yylineno); } 
    | statement_list statement
      { $$ = addStatementToList($1, $2); } 
    ;

statement:
      var_declaration SEMICOLON { $$ = $1; }
    | assignment SEMICOLON      { $$ = $1; }
    | io_statement SEMICOLON    { $$ = $1; }
    | control_statement       { $$ = $1; }
    | function                { $$ = $1; }
    | RETURN expression SEMICOLON { $$ = createReturnNode($2, yylineno); }
    | RETURN SEMICOLON          { $$ = createReturnNode(nullptr, yylineno); }
    | SEMICOLON                 { $$ = nullptr; }
    | expression SEMICOLON      { $$ = $1; }
    ;

var_declaration:
    type IDENTIFIER
      { $$ = createVarDeclarationNode($1, createIdentifierNode($2, yylineno), nullptr, yylineno); }
    | type IDENTIFIER ASSIGN expression
      { $$ = createVarDeclarationNode($1, createIdentifierNode($2, yylineno), $4, yylineno); }
    ;

type:
      INT   { $$ = createIdentifierNode(strdup("int"), yylineno); ((Node*)$$)->type = NODE_TYPE; }
    | FLOAT { $$ = createIdentifierNode(strdup("float"), yylineno); ((Node*)$$)->type = NODE_TYPE; }
    | STRING_TYPE { $$ = createIdentifierNode(strdup("string"), yylineno); ((Node*)$$)->type = NODE_TYPE; }
    | VOID  { $$ = createIdentifierNode(strdup("void"), yylineno); ((Node*)$$)->type = NODE_TYPE; }
    | BOOL_TYPE { $$ = createIdentifierNode(strdup("bool"), yylineno); ((Node*)$$)->type = NODE_TYPE; }
    ;

assignment:
    IDENTIFIER ASSIGN expression
    { $$ = createAssignmentNode(createIdentifierNode($1, yylineno), $3, yylineno); }
    ;

io_statement:
      PRINT LPAREN expression RPAREN { $$ = createPrintNode($3, yylineno); }
    | READ LPAREN IDENTIFIER RPAREN  { $$ = createReadNode(createIdentifierNode($3, yylineno), yylineno); } // $3 es sval
    ;

control_statement:
      IF LPAREN expression RPAREN LBRACE statement_list RBRACE
        { $$ = createIfNode($3, $6, nullptr, yylineno); }
    | IF LPAREN expression RPAREN LBRACE statement_list RBRACE ELSE LBRACE statement_list RBRACE
        { $$ = createIfNode($3, $6, $10, yylineno); }
    | WHILE LPAREN expression RPAREN LBRACE statement_list RBRACE
        { $$ = createWhileNode($3, $6, yylineno); }
    | FOR LPAREN opt_expression SEMICOLON opt_expression SEMICOLON opt_expression RPAREN LBRACE statement_list RBRACE
      { $$ = createForNode($3, $5, $7, $10, yylineno); }
    ;

opt_expression:
      %empty      { $$ = nullptr; }
    | expression  { $$ = $1; }
    ;

function:
    function_signature LBRACE statements_opt RBRACE
    { $$ = createFunctionDefNode($1, $3, yylineno); }
    ;

function_signature:
    type IDENTIFIER LPAREN params_opt RPAREN
    { $$ = createFunctionSignatureNode($1, createIdentifierNode($2, yylineno), $4, yylineno); }
    ;

params_opt:
      %empty { $$ = nullptr; }
    | param_list { $$ = $1; }
    ;

param_list:
      param { $$ = createParamList($1); }
    | param_list ',' param { $$ = addParamToList($1, $3); }
    ;

param:
    type IDENTIFIER { $$ = createVarDeclarationNode($1, createIdentifierNode($2, yylineno), nullptr, yylineno); }
    ;

statements_opt:
      %empty { $$ = nullptr; }
    | statement_list { $$ = $1; }
    ;

expression:
      term                  { $$ = $1; }
    | expression PLUS term  { $$ = createBinaryOpNode(PLUS, $1, $3, yylineno); }
    | expression MINUS term { $$ = createBinaryOpNode(MINUS, $1, $3, yylineno); }
    | expression EQ term    { $$ = createBinaryOpNode(EQ, $1, $3, yylineno); }
    | expression NE term    { $$ = createBinaryOpNode(NE, $1, $3, yylineno); }
    | expression LT term    { $$ = createBinaryOpNode(LT, $1, $3, yylineno); }
    | expression GT term    { $$ = createBinaryOpNode(GT, $1, $3, yylineno); }
    | expression LE term    { $$ = createBinaryOpNode(LE, $1, $3, yylineno); }
    | expression GE term    { $$ = createBinaryOpNode(GE, $1, $3, yylineno); }
    | expression AND_OP expression { $$ = createBinaryOpNode(AND_OP, $1, $3, yylineno); } 
    | expression OR_OP expression  { $$ = createBinaryOpNode(OR_OP, $1, $3, yylineno); }
    ;

term:
      factor           { $$ = $1; }
    | term MULT factor { $$ = createBinaryOpNode(MULT, $1, $3, yylineno); }
    | term DIV factor  { $$ = createBinaryOpNode(DIV, $1, $3, yylineno); }
    ;

factor:
      NUMBER          { $$ = createConstantIntNode($1, yylineno); }
    | FLOAT_NUMBER    { $$ = createConstantFloatNode($1, yylineno); }
    | TRUE_LITERAL    { $$ = createConstantBoolNode($1, yylineno); }
    | FALSE_LITERAL   { $$ = createConstantBoolNode($1, yylineno); }
    | IDENTIFIER      { $$ = createIdentifierNode($1, yylineno); }
    | STRING_LITERAL  { $$ = createStringLiteralNode($1, yylineno); }
    | LPAREN expression RPAREN { $$ = $2; }
    | assignment      { $$ = $1; }
    | IDENTIFIER LPAREN args_opt RPAREN{ $$ = createFunctionCallNode(createIdentifierNode($1, yylineno), $3, yylineno); }
    | NOT_OP factor   { $$ = createUnaryOpNode(NOT_OP, $2, yylineno); }
    ;

args_opt:
      %empty { $$ = nullptr; }
    | arg_list { $$ = $1; }
    ;

arg_list:
      expression { $$ = createArgList($1); }
    | arg_list ',' expression { $$ = addArgToList($1, $3); } 
    ;

%%



void yyerror(const char *s) {
    std::cerr << "Error de Sintaxis: " << s << " cerca de la línea " << yylineno << std::endl;
    exit(1);
}

int main() {
    int result = yyparse();

    if (result == 0) {
        std::cerr << "[INFO] Análisis sintáctico completado con éxito." << std::endl;
        if (globalAstRoot) {
             std::cerr << "[INFO] Raíz del AST capturada. Imprimiendo AST..." << std::endl;
             std::cout << "--- INICIO AST ---" << std::endl;
             printAst(globalAstRoot);
             std::cout << "--- FIN AST ---" << std::endl;
             std::cerr << "[INFO] Liberando AST..." << std::endl;
             freeAst(globalAstRoot);
             std::cerr << "[INFO] AST Liberado." << std::endl;
        } else {
             std::cerr << "[ERROR] Raíz del AST no capturada." << std::endl;
        }
    } else {
        std::cerr << "[INFO] Análisis sintáctico fallido (código " << result << ")." << std::endl;
    }
    if (yyin != stdin) {
        fclose(yyin);
    }
    return result;
}