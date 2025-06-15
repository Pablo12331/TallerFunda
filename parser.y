//El parser es el que actua despues del lexer, sus propositos principales son verificar la gramatica y construir el AST
%{
#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "common_types.h"
#include "Ast.h" 
#include "symbol_table.h"
#include "type_checker.h"
#include "code_generator.h"

//Variables y funciones definidas fuera del parser (lex.yy.c)
extern int yylex();
extern int yylineno;
extern char *yytext;
extern FILE *yyin;
void yyerror(const char *s);

//Puntero que guardara la raiz del arbol AST una vez completo todo el proceso del parser
Node* globalAstRoot = nullptr;

//Variable para recordar el tipo de variable en una declaracion multiple
Node* current_declaration_type_node = nullptr;

%}

//"%Code requieres" es un auxiliar para poder decirle al bison que existe una estructura llamada Node y que no tire error en "%union"
%code requires 
{
  typedef struct Node Node;
}

//variables auxiliares para transportar los tipos de valores que tenemos 
%union 
{
  int ival;
  char *sval;
  double fval;
  Node *nodePtr;
}

//Declaracion de los nombres de los tokens que puede devolver el parser
//El valor entre los "<...>" ayudan a bison a saber donde buscar en el apartado de "%union"

%token <ival> INT FLOAT STRING_TYPE VOID
%token <ival> EQ NE LT GT LE GE
%token IF UNLESS ELSE WHILE FOR PRINT READ RETURN BREAK CONTINUE
%token <ival> AND_OP OR_OP NOT_OP
%token BOOL_TYPE
%token PIPE

%token LBRACE RBRACE LPAREN RPAREN SEMICOLON ASSIGN ','

%token <ival> PLUS MINUS MULT DIV
%token PLUS_ASSIGN MINUS_ASSIGN MULT_ASSIGN DIV_ASSIGN
%token <ival> NUMBER
%token <fval> FLOAT_NUMBER
%token <sval> IDENTIFIER
%token <sval> STRING_LITERAL
%token <ival> TRUE_LITERAL FALSE_LITERAL

//"%type" practicamente hace lo mismo que los "%token" pero en esta ocasion es para los no terminales
//El objetivo en este caso es ir construyendo el AST
%type <nodePtr> program statement_list statement function assignment io_statement control_statement type
%type <nodePtr> expression term factor statements_opt function_signature opt_expression param_list params_opt param arg_list args_opt
%type <nodePtr> compound_statement
%type <nodePtr> var_declaration_actual declarator_list_actual declarator_actual for_init

//Es quien le dice el objetivo final al bison asi saber si logro o no el objetivo de si mismo
%start program

//Declara el orden de las operaciones aritmeticas
%right ASSIGN
%left PIPE
%left  OR_OP
%left  AND_OP
%left EQ NE LT GT LE GE
%left PLUS MINUS
%left MULT DIV
%right NOT_OP UMINUS

%%

//Declaracion del objetivo final de bison que es que "statement_list" creado desde el parser, se vuelva el nodo raiz del AST
program:
    statement_list
    { $$ = $1; globalAstRoot = $$; }
    ;

//Construccion del arbol AST, donde se une cada uno de sus nodos, y sus respectivas listas
statement_list:
      statement
      { $$ = createStatementListNode($1, $1 ? $1->line : yylineno); } 
    | statement_list statement
      { $$ = addStatementToList($1, $2); } 
    ;

//Formatos o declaraciones validas que puede tener una instruccion
statement:
      var_declaration_actual SEMICOLON { $$ = $1; }
    | assignment SEMICOLON      { $$ = $1; } 
    | io_statement SEMICOLON    { $$ = $1; } 
    | control_statement       { $$ = $1; }
    | function                { $$ = $1; }
    | RETURN expression SEMICOLON { $$ = createReturnNode($2, yylineno); }
    | RETURN SEMICOLON          { $$ = createReturnNode(nullptr, yylineno); }
    | BREAK SEMICOLON    { $$ = createBreakNode(yylineno); }
    | CONTINUE SEMICOLON { $$ = createContinueNode(yylineno); }
    | SEMICOLON                 { $$ = nullptr; }
    | expression SEMICOLON      { $$ = $1; }
    | compound_statement      { $$ = $1; }
    | expression PIPE PRINT SEMICOLON{ $$ = createPrintNode($1, yylineno); }
    ;

//Definicion de un bloque de codigo como seria el de un if, while, etc.
compound_statement:
    LBRACE RBRACE
        { $$ = createStatementListNode(nullptr, yylineno); } 
    | LBRACE statement_list RBRACE
        { $$ = $2; }
    ;

//Reglas para poder lograr la declaracion de multivariables del mismo tipo(INICIO)

//Crea un nodo del tipo de la o las variables, y los guarda en la variable global "current_declaration_type_node"
type:
      INT   { $$ = createIdentifierNode(strdup("int"), yylineno); 
              ((Node*)$$)->type = NODE_TYPE; 
              current_declaration_type_node = $$;
            }
    | FLOAT { $$ = createIdentifierNode(strdup("float"), yylineno); 
              ((Node*)$$)->type = NODE_TYPE; 
              current_declaration_type_node = $$;
            }
    | STRING_TYPE { $$ = createIdentifierNode(strdup("string"), yylineno); 
                    ((Node*)$$)->type = NODE_TYPE;
                    current_declaration_type_node = $$; 
                  }
    | VOID  { $$ = createIdentifierNode(strdup("void"), yylineno); 
              ((Node*)$$)->type = NODE_TYPE; 
              current_declaration_type_node = $$;
            }
    | BOOL_TYPE { $$ = createIdentifierNode(strdup("bool"), yylineno); 
                  ((Node*)$$)->type = NODE_TYPE; 
                  current_declaration_type_node = $$;
                }
    ;

//Practicamente es quien ayuda a declarar el tipo de cada una de las variables en la declaracion multiple
//Cuando llega a la ultima, se encarga de dejar la variable global en nullptr para que pueda ser ocupada por otras declaraciones
var_declaration_actual: 
    type 
    declarator_list_actual 
    {
        $$ = $2; 
        current_declaration_type_node = nullptr;
    }
    ;
    
//Es quien maneja las variables separadas por "," en la declaracion de multivariable
declarator_list_actual: 
    declarator_actual
    {
        $$ = createStatementListNode($1, $1 ? $1->line : yylineno);
    }
    | declarator_list_actual ',' declarator_actual
    {
        $$ = addStatementToList($1, $3);
    }
    ;

//Es el que hace el principal trabajo, lee que tipo de variable es (int, string, etc.), crea un VarDeclarationNode 
//y lo registra en la tabla de simbolos
declarator_actual: 
    IDENTIFIER
    {
        Node* id_node_for_ast = createIdentifierNode($1, yylineno);
        $$ = createVarDeclarationNode(current_declaration_type_node, id_node_for_ast, nullptr, yylineno);
        if (!c_insert_variable($1, current_declaration_type_node, yylineno, $$)) 
        {
            yyerror(("Error Semántico: Re-declaración de variable '" + std::string($1) + "'").c_str());
        }
    }
    | IDENTIFIER ASSIGN expression
    {
        Node* id_node_for_ast = createIdentifierNode($1, yylineno);
        $$ = createVarDeclarationNode(current_declaration_type_node, id_node_for_ast, $3, yylineno);
        if (!c_insert_variable($1, current_declaration_type_node, yylineno, $$)) 
        {
            yyerror(("Error Semántico: Re-declaración de variable '" + std::string($1) + "'").c_str());
        }
    }
    ;

//Reglas para poder lograr la declaracion de multivariables del mismo tipo(FINAL)

//Regla de las asignaciones(Ejem: "=", "+=", "-=", etc.)
assignment:
    IDENTIFIER ASSIGN expression //"="
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
    | IDENTIFIER PLUS_ASSIGN expression //"+="
    {
        Node* id_node_lhs = createIdentifierNode($1, yylineno);
        Node* id_node_rhs = createIdentifierNode(strdup($1), yylineno);

        Node* binary_op = createBinaryOpNode(PLUS, id_node_rhs, $3, yylineno);

        $$ = createAssignmentNode(id_node_lhs, binary_op, yylineno);
    }

    | IDENTIFIER MINUS_ASSIGN expression //"-="
    {
        Node* id_node_lhs = createIdentifierNode($1, yylineno);
        Node* id_node_rhs = createIdentifierNode(strdup($1), yylineno);
        Node* binary_op = createBinaryOpNode(MINUS, id_node_rhs, $3, yylineno);
        $$ = createAssignmentNode(id_node_lhs, binary_op, yylineno);
    }

    | IDENTIFIER MULT_ASSIGN expression //"*="
    {
        Node* id_node_lhs = createIdentifierNode($1, yylineno);
        Node* id_node_rhs = createIdentifierNode(strdup($1), yylineno);
        Node* binary_op = createBinaryOpNode(MULT, id_node_rhs, $3, yylineno);
        $$ = createAssignmentNode(id_node_lhs, binary_op, yylineno);
    }

    | IDENTIFIER DIV_ASSIGN expression //"/="
    {
        Node* id_node_lhs = createIdentifierNode($1, yylineno);
        Node* id_node_rhs = createIdentifierNode(strdup($1), yylineno);
        Node* binary_op = createBinaryOpNode(DIV, id_node_rhs, $3, yylineno);
        $$ = createAssignmentNode(id_node_lhs, binary_op, yylineno);
    }
    ;
//Manejo de las funciones de entradas y salidas ("print" y "read")
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

//Reglas de los controles de flujo("if", "else if", "while", etc.)
control_statement:
      IF LPAREN expression RPAREN LBRACE statement_list RBRACE //"if(){};"
      { $$ = createIfNode($3, $6, nullptr, yylineno); }
    | UNLESS LPAREN expression RPAREN LBRACE statement_list RBRACE //"unless(){}"
      {
        Node* negated_condition = createUnaryOpNode(NOT_OP, $3, yylineno);
        $$ = createIfNode(negated_condition, $6, nullptr, yylineno);
      }
    | IF LPAREN expression RPAREN LBRACE statement_list RBRACE ELSE statement //"else if(){};" o "else{};"
    { $$ = createIfNode($3, $6, $9, yylineno); }
    | WHILE LPAREN expression RPAREN LBRACE statement_list RBRACE //"while(){};"
    { $$ = createWhileNode($3, $6, yylineno); }
    | FOR LPAREN for_init SEMICOLON opt_expression SEMICOLON opt_expression RPAREN LBRACE statement_list RBRACE //"for(){};"
    { $$ = createForNode($3, $5, $7, $10, yylineno); }
    ;

//Regla que nos permite utilizar o inicializar la variable utilizada por el "for" en su primer argumento
for_init:
      opt_expression
      {
          $$ = $1;
      }
    | type IDENTIFIER
      {
          Node* id_node = createIdentifierNode($2, yylineno);
          $$ = createVarDeclarationNode($1, id_node, nullptr, yylineno);
          if (!c_insert_variable($2, $1, yylineno, $$)) {
              yyerror(("Error Semántico: Re-declaración de variable '" + std::string($2) + "' en for.").c_str());
          }
      }
    | type IDENTIFIER ASSIGN expression
      {
          Node* id_node = createIdentifierNode($2, yylineno);
          $$ = createVarDeclarationNode($1, id_node, $4, yylineno);
          if (!c_insert_variable($2, $1, yylineno, $$)) {
              yyerror(("Error Semántico: Re-declaración de variable '" + std::string($2) + "' en for.").c_str());
          }
      }
    ;

//Regla para lugares donde una expresion es opcional
opt_expression:
      %empty      { $$ = nullptr; }
    | expression  { $$ = $1; }
    ;

//Une la function_signature con el compound_statement del cuerpo para crear el nodo final FunctionDefNode
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

//Regla que se encarga de registrar la funcion, permitiendo identificar si es que ya existe, y la recursivida
function_signature:
    type IDENTIFIER LPAREN params_opt RPAREN
    { 
      Node* func_name_node_for_ast = createIdentifierNode($2, yylineno); 
      Node* params_node = $4;
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

//Regla que permite la ausencia de parametros en una funcion
params_opt:
      %empty { $$ = nullptr; }
    | param_list { $$ = $1; }
    ;

//Definicion para un parametro de una funcion
param:
    type IDENTIFIER 
    { 
      Node* param_id_node_for_ast = createIdentifierNode($2, yylineno);
      $$ = createVarDeclarationNode($1, param_id_node_for_ast, nullptr, yylineno);
    }
    ;

//construye una lista de parametros de una funcion
param_list:
      param { $$ = createParamList($1); }
    | param_list ',' param { $$ = addParamToList($1, $3); }
    ;
//Permite que las funciones no tengan nada en su cuerpo(o sea dentro de sus {})
statements_opt:
      %empty { $$ = nullptr; }
    | statement_list { $$ = $1; }
    ;

//Regla que agrupa los factores de alta procedencia en una expresion matematica
term:
      factor           { $$ = $1; }
    | term MULT factor { $$ = createBinaryOpNode(MULT, $1, $3, yylineno); }
    | term DIV factor  { $$ = createBinaryOpNode(DIV, $1, $3, yylineno); }
    ;

//Regla que agrupa los operadores de baja procedencia y los terminos que se crearon en "term"
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
    | expression PIPE factor
      {
          Node* left_expr = $1;
          Node* right_func_factor = $3;
          Node* new_call_node = nullptr;

          if (right_func_factor->type == NODE_FUNCTION_CALL) {
              FunctionCallNode* call_node = (FunctionCallNode*)right_func_factor;
              left_expr->next = call_node->arguments; 
              call_node->arguments = left_expr;

              new_call_node = right_func_factor;

          } else if (right_func_factor->type == NODE_IDENTIFIER) {
              Node* arg_list = createArgList(left_expr);
              new_call_node = createFunctionCallNode(right_func_factor, arg_list, yylineno);

          } else {
              yyerror("Error de Sintaxis: El operador '|>' debe ser seguido por una llamada a función.");
              $$ = nullptr; 
          }
          
          $$ = new_call_node;
      }
    ;

//Regla que representa las unidades atomicas en una expresion matematica
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
    | MINUS factor %prec UMINUS { $$ = createUnaryOpNode(MINUS, $2, yylineno); }
    ;

//Regla que permite que una regla de argumentos pueda ser vacia
args_opt:
      %empty { $$ = nullptr; }
    | arg_list { $$ = $1; }
    ;

//Regla que construye recursivamente una lista de nodos enlazados de expresiones
arg_list:
      expression { $$ = createArgList($1); }
    | arg_list ',' expression { $$ = addArgToList($1, $3); } 
    ;

%%

//Es la funcion principal para el manejo de errores de syntax
void yyerror(const char *s) {
    std::cerr << "Error de Sintaxis: " << s << " cerca de la línea " << yylineno << std::endl;
    //exit(1);(Si se descomenta esta linea, el proceso de analisis parara al primer error, cosa que puede ser incomodo
    //ya que si hay mas de un error en el codigo, tendria que ir arreglandose uno por uno si no te das cuenta)
}

//El punto de entrada, es el que llama a las demas reglas y funciones
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
        std::cerr << "[INFO] Analisis sintactico completado con exito." << std::endl;
        
        std::cerr << "[INFO] Iniciando verificacion de tipos..." << std::endl;
        sinErroresDeTipo = performTypeChecking(globalAstRoot);
        if (sinErroresDeTipo) {
            std::cerr << "[INFO] Verificacion de tipos completada sin errores." << std::endl;
        } else {
            std::cerr << "[INFO] Verificacion de tipos completada CON ERRORES" << std::endl;
        }

        if (globalAstRoot != nullptr) {
            std::cerr << "[INFO] Raiz del AST capturada. Imprimiendo AST..." << std::endl;
            if (!sinErroresDeTipo) {
                std::cerr << "[INFO] Nota: Se detectaron errores de tipo. El AST podria mostrar nodos marcados como ERROR_TYPE." << std::endl;
            }else{
                generate_code(globalAstRoot, "salida.cpp");
            }
            std::cout << "--- INICIO AST ---" << std::endl;
            printAst(globalAstRoot);
            std::cout << "--- FIN AST ---" << std::endl;

        } else {
            std::cerr << "[INFO] Analisis sintactico fallido (codigo " << result << ")." << std::endl;
        }

        std::cerr << "[INFO] Liberando AST..." << std::endl;
        freeAst(globalAstRoot);
        globalAstRoot = nullptr; 
        std::cerr << "[INFO] AST Liberado." << std::endl;

    } else if (result == 0 && globalAstRoot == nullptr) {
         std::cerr << "[INFO] Analisis sintactico completado con exito (programa vacio)." << std::endl;
    } else {
        std::cerr << "[INFO] Analisis sintactico fallido (codigo " << result << ")." << std::endl;
    }
    c_destroy_symbol_manager();

    if (yyin != stdin) {
        fclose(yyin);
    }
    return result;
}