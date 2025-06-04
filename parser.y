%{
#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "Ast.h" 
#include "symbol_table.h"
#include "type_checker.h"

extern int yylex();
extern int yylineno;
extern char *yytext;
extern FILE *yyin;
void yyerror(const char *s);

Node* globalAstRoot = nullptr;

%}

%code requires 
{
  typedef struct Node Node;
}

%union 
{
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
      { 
        Node* id_node_for_ast = createIdentifierNode($2, yylineno); 
        $$ = createVarDeclarationNode($1, id_node_for_ast, nullptr, yylineno);
        if (!c_insert_variable($2, $1, yylineno, $$)) 
        {
          std::string err_msg = "Error Semántico: Re-declaración de variable '" + std::string($2) + "'";
          yyerror(err_msg.c_str());
        } 
      }
    | type IDENTIFIER ASSIGN expression
      { 
        Node* id_node_for_ast = createIdentifierNode($2, yylineno);
        $$ = createVarDeclarationNode($1, id_node_for_ast, $4, yylineno); 
        if (!c_insert_variable($2, $1, yylineno, $$)) 
        {
          std::string err_msg = "Error Semántico: Re-declaración de variable '" + std::string($2) + "'";
          yyerror(err_msg.c_str());
        }
      }
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
    { 
      C_SymbolInfo info;
      if (!c_lookup_symbol($1, &info))
      {
        std::string err_msg = "Error Semántico: Identificador no declarado '" + std::string($1) + "'";
        yyerror(err_msg.c_str());
      } 
      else if (info.kind == NODE_FUNCTION_DEF) 
      {
        std::string err_msg = "Error Semántico: '" + std::string($1) + "' es una función, no se puede asignar.";
        yyerror(err_msg.c_str());
      }

      Node* id_node_for_ast = createIdentifierNode($1, yylineno);
      $$ = createAssignmentNode(id_node_for_ast, $3, yylineno);     
    }
    ;

io_statement:
      PRINT LPAREN expression RPAREN { $$ = createPrintNode($3, yylineno); }
    | READ LPAREN IDENTIFIER RPAREN  
    { 
      C_SymbolInfo info;
      if (!c_lookup_symbol($3, &info)) 
      {
        std::string err_msg = "Error Semántico: Identificador no declarado '" + std::string($3) + "' para READ";
        yyerror(err_msg.c_str());
      } 
      else if (info.kind == NODE_FUNCTION_DEF) 
      {
        std::string err_msg = "Error Semántico: No se puede leer en una función '" + std::string($3) + "'";
        yyerror(err_msg.c_str());
      }
      
      Node* id_node_for_ast = createIdentifierNode($3, yylineno);
      $$ = createReadNode(id_node_for_ast, yylineno);
    }
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
    { 
      if ($1 != nullptr) { 
          $$ = createFunctionDefNode($1, $3, yylineno);
          c_exit_scope();
      } else {
          $$ = nullptr; 
      }
    }
    ;

function_signature:
    type IDENTIFIER LPAREN params_opt RPAREN
    { 
      Node* func_name_node_for_ast = createIdentifierNode($2, yylineno); 
      Node* params_node = $4; // $4 es el nodo 'param_list' (cabeza de lista de VarDeclarationNode para params)
      Node* sig_node = createFunctionSignatureNode($1, func_name_node_for_ast, params_node, yylineno);

      if (!c_insert_function($2, $1, params_node, yylineno, sig_node)) {
        std::string err_msg = "Error Semántico: Re-declaración de función '" + std::string($2) + "'";
        yyerror(err_msg.c_str());
        $$ = nullptr; 
      } else {
        Node* current_param_ast_node = params_node;
        while (current_param_ast_node != nullptr) {
            if (current_param_ast_node->type == NODE_VAR_DECLARATION) {
                VarDeclarationNode* p_decl_node = (VarDeclarationNode*)current_param_ast_node;
                IdentifierNode* p_id_node = p_decl_node->identifier;
                Node* p_type_node = p_decl_node->varTypeNode;  

                if (p_id_node && p_id_node->sval && p_type_node) {
                    if (!c_insert_variable(p_id_node->sval, p_type_node, p_decl_node->line, (Node*)p_decl_node)) {
                         std::string err_msg = "Error Semántico: Re-declaración del parámetro '" + 
                                               std::string(p_id_node->sval) + "' en la firma de la función '" + 
                                               std::string($2) + "'.";
                         yyerror(err_msg.c_str());
                    }
                }
            }
            current_param_ast_node = current_param_ast_node->next;
        }
        $$ = sig_node;
      }
    }
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
    type IDENTIFIER 
    { 
      Node* param_id_node_for_ast = createIdentifierNode($2, yylineno);
      $$ = createVarDeclarationNode($1, param_id_node_for_ast, nullptr, yylineno);
    }
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
    | IDENTIFIER      
    { 
      C_SymbolInfo info;
      if (!c_lookup_symbol($1, &info))
      {
        std::string err_msg = "Error Semántico: Identificador no declarado '" + std::string($1) + "'";
        yyerror(err_msg.c_str());
        $$ = createIdentifierNode($1, yylineno);
      }
      else
      {
        $$ = createIdentifierNode($1, yylineno);
      }
    }
    | STRING_LITERAL  { $$ = createStringLiteralNode($1, yylineno); }
    | LPAREN expression RPAREN { $$ = $2; }
    | assignment      { $$ = $1; }
    | IDENTIFIER LPAREN args_opt RPAREN
    { 
      C_SymbolInfo func_info;
        if (!c_lookup_symbol($1, &func_info)) 
        {
          std::string err_msg = "Error Semántico: Función no declarada '" + std::string($1) + "'";
          yyerror(err_msg.c_str());
        }
        else if (func_info.kind != NODE_FUNCTION_DEF) 
        { 
          std::string err_msg = "Error Semántico: '" + std::string($1) + "' no es una función.";
          yyerror(err_msg.c_str());
        }
        Node* func_call_name_node = createIdentifierNode($1, yylineno); 
        $$ = createFunctionCallNode(func_call_name_node, $3, yylineno);
    }
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
    //exit(1);
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror(argv[1]);
            return 1;
        }
    } else {
        yyin = stdin;
    }

    c_init_symbol_manager();

    int result = yyparse();

    bool sinErroresDeTipo = true;

    if (result == 0 && globalAstRoot != nullptr) {
        std::cerr << "[INFO] Análisis sintáctico completado con éxito." << std::endl;
        
        std::cerr << "[INFO] Iniciando verificación de tipos..." << std::endl;
        sinErroresDeTipo = realizarVerificacionDeTipos(globalAstRoot);
        if (sinErroresDeTipo) {
            std::cerr << "[INFO] Verificación de tipos completada sin errores." << std::endl;
        } else {
            std::cerr << "[INFO] Verificación de tipos completada CON ERRORES" << std::endl;
        }

        if (result == 0) {
            if (sinErroresDeTipo) {
                std::cerr << "[INFO] Raíz del AST capturada. Imprimiendo AST..." << std::endl;
                std::cout << "--- INICIO AST ---" << std::endl;
                printAst(globalAstRoot);
                std::cout << "--- FIN AST ---" << std::endl;
                
            } else {
                std::cerr << "[INFO] No se imprime el AST debido a errores de tipo." << std::endl;
            }
        } else {
            std::cerr << "[INFO] Análisis sintáctico fallido (código " << result << ")." << std::endl;
        }

        std::cerr << "[INFO] Liberando AST..." << std::endl;
        freeAst(globalAstRoot);
        globalAstRoot = nullptr; 
        std::cerr << "[INFO] AST Liberado." << std::endl;

    } else if (result == 0 && globalAstRoot == nullptr) {
         std::cerr << "[INFO] Análisis sintáctico completado con éxito (programa vacío)." << std::endl;
    } else {
        std::cerr << "[INFO] Análisis sintáctico fallido (código " << result << ")." << std::endl;
    }
    c_destroy_symbol_manager();

    if (yyin != stdin) {
        fclose(yyin);
    }
    return result;
}