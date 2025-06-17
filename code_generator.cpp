//Logica de la traduccion del AST del codigo a codigo C++
#include "code_generator.h"
#include "common_types.h"
#include "symbol_table.h" 
#include "parser.tab.h" 
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

//Metodo utilizado para traducir a C++ las expresiones
static void generate_expression_code(Node* node, std::stringstream& out);

//Metodo utilizado para traducir a C++ las declaraciones
static void generate_statement_code(Node* node, std::stringstream& out, int indent_level);

//Metodo utilizado para traducir a C++ las funciones
static void generate_function_definition_code(Node* node, std::stringstream& out);

//Metodo utilizado para ver el nivel de identacion a utilizar
static std::string indent(int level) {
    return std::string(level * 4, ' '); 
}

//Combierte los tipos enum a su respectivo tipo en C++
static std::string map_type_to_cpp(SymbolBasicType type) {
    switch (type) {
        case SYM_TYPE_INT:    return "int";
        case SYM_TYPE_FLOAT:  return "float";
        case SYM_TYPE_BOOL:   return "bool";
        case SYM_TYPE_STRING: return "std::string";
        case SYM_TYPE_VOID:   return "void";
        default:              return "/* tipo desconocido */";
    }
}

//El principal generador de codigo
void generate_code(Node* ast_root, const std::string& output_filename) {
    if (!ast_root) { //Verifica que haya un AST el cual traducir
        std::cerr << "[CodeGenerator] AST vacío, no se genera código." << std::endl;
        return;
    }

    //Declaracion de 2 buffers con stringstream que son como "archivos de texto en memoria"
    std::stringstream functions_buffer; //utilizada a la hora de encontrarse con una funcion la cual traducir
    std::stringstream main_buffer; //Utilizada para traducir todo lo correspondiente al main

    //Verifica que el tipo de el puntero de el AST sea un NODE_STATEMENT_LIST
    if (ast_root->type == NODE_STATEMENT_LIST) {
        StatementListNode* list = (StatementListNode*)ast_root; //Guarda en un puntero auxiliar la raiz del AST
        if (list->statements) { //Verifica que el puntero hacia los demas punteros (Demas nodos del AST) no este vacia
            for (Node* stmt : *(list->statements)) { //Explora cada uno de los punteros (Nodos del arbol AST)
                if (stmt && stmt->type == NODE_FUNCTION_DEF) {  //Separa los punteros del tipo funcion
                    generate_function_definition_code(stmt, functions_buffer); 
                } else { //De las demas punteros que esten fuera de cualquier funcion
                    generate_statement_code(stmt, main_buffer, 1);
                }
            }
        }
    }

    //EL ensamblaje del codigo en el orden correcto
    std::stringstream final_code;
    final_code << "// Código generado por mi compilador" << std::endl;
    //Respectivos includes
    final_code << "#include <iostream>" << std::endl;
    final_code << "#include <string>" << std::endl;
    final_code << "#include <vector>" << std::endl;
    final_code << std::endl;

    //Declaracion de las funciones y sus bloques de codigo
    final_code << functions_buffer.str() << std::endl;

    final_code << "int main() {" << std::endl;
    
    //Todo lo que debe ir dentro del main
    final_code << main_buffer.str();

    final_code << indent(1) << "return 0;" << std::endl;
    final_code << "}" << std::endl;

    std::ofstream output_file(output_filename);

    //Verifica que el archivo se haya generado correctamente
    if (!output_file.is_open()) { 
        std::cerr << "[CodeGenerator] Error: No se pudo abrir el archivo de salida " << output_filename << std::endl; //Si no reporta el error
        return;
    }
    output_file << final_code.str();
    output_file.close();

    //Reporta que el codigo se haya traducido y generado exitosamente
    std::cout << "[CodeGenerator] Código generado exitosamente en " << output_filename << std::endl;
}

//Todo lo que se escriba en este metodo sera puesto en functions_buffer
static void generate_function_definition_code(Node* node, std::stringstream& out) {
    if (!node || node->type != NODE_FUNCTION_DEF) return; //Verifica que el puntero no este vacio y que sea del tipo adecuado para esta funcion

    FunctionDefNode* func_def = (FunctionDefNode*)node; //Guarda en un puntero auxiliar el puntero otorgado
    FunctionSignatureNode* sig = (FunctionSignatureNode*)func_def->signature; //Guarda el puntero de la firma de la funcion

    //Verifica que el tipo que debe retornar la funcion sea valida, toma su enum correspondiente utilizando c_ast_node_to_symbol_basic_type
    //Luego lo traduce al tipo que deberia ser en C++ utilizando map_type_to_cpp 
    out << map_type_to_cpp(c_ast_node_to_symbol_basic_type(sig->returnType));

    //Escribe el nombre de la funcion seguido de abrir parentesis para poner los respectivos parametros(Si es que los tiene)
    out << " " << sig->name->sval << "(";

    //Verifica si es que el puntero de parametros no sea nullptr
    if (sig->parameters) {
        Node* current_param = sig->parameters; //Utiliza un puntero auxiliar para guardar el puntero de los parametros 
        bool first_param = true; //Variable auxiliar bool iniciada en true  
        while (current_param) {
            if (!first_param) { //Variable auxiliar bool, utilizada para que en el primer parametro de la funcion no se escriba una coma antes
                out << ", ";
            }

            VarDeclarationNode* param_decl = (VarDeclarationNode*)current_param; //Toma el primer parametro

            //Verifica que el tipo del parametro sea valido, toma su enum correspondiente utilizando c_ast_node_to_symbol_basic_type
            //Luego lo traduce al tipo que deberia ser en C++ utilizando map_type_to_cpp 
            out << map_type_to_cpp(c_ast_node_to_symbol_basic_type(param_decl->varTypeNode));
            out << " " << param_decl->identifier->sval;
            
            first_param = false; //Cambia la variable auxiliar a false para que pueda escribir la coma que indica la separacion de los parametros
            current_param = current_param->next; //Toma el siguiente parametro en la funcion
        }
    }
    out << ") {" << std::endl; //Cierra el parentesis y abre llaves para traducir todo lo haya dentro de el cuerpo de la funcion

    //Llama al metodo generate_statement_code pasandole el puntero al cuerpo de la funcion, y el nivel de identacion en 1 
    generate_statement_code(func_def->body, out, 1);

    out << "}" << std::endl << std::endl; //Cierra llaves
}
 
//Casi todo lo que se escriba en este metodo sera puesto en main_buffer
//Traductor de todos los nodos de tipo sentencia a C++ 
static void generate_statement_code(Node* node, std::stringstream& out, int indent_level) {
    if (!node) return; //Verificacion de que el puntero entregado sea distinto de nullptr
    while (node) {
        switch (node->type) { //Toma el tipo del puntero
            case NODE_STATEMENT_LIST: { //Si es un NODE_STATEMENT_LIST
                StatementListNode* list = (StatementListNode*)node; //Guarda en un puntero auxiliar el puntero otorgado
                if (list->statements) { //Verifica que el puntero sea distinto de nullptr 
                    for (Node* stmt : *(list->statements)) { //Explora todo lo que haya sido declarado en ese bloque de codigo
                        //Se llama recursivamente a generate_statement_code para poder explorar cada parte dentro del bloque de codigo
                        generate_statement_code(stmt, out, indent_level);
                    }
                }
                break;
            }

            case NODE_VAR_DECLARATION: {
                out << indent(indent_level);
                VarDeclarationNode* decl_node = (VarDeclarationNode*)node;

                out << map_type_to_cpp(c_ast_node_to_symbol_basic_type(decl_node->varTypeNode));
                out << " " << decl_node->identifier->sval;

                if (decl_node->initialValue) {
                    out << " = ";
                    generate_expression_code(decl_node->initialValue, out);
                }
                out << ";" << std::endl;
                break;
            }

            case NODE_PRINT: { //Si es un NODE_PRINT
                out << indent(indent_level);
                PrintNode* print_node = (PrintNode*)node; //Guarda en un puntero auxiliar el puntero otorgado
                out << "std::cout << ";
                //Busca y imprime el valor correspondiente a la expresion dentro del print utilizando generate_expression_code
                generate_expression_code(print_node->expression, out);
                out << " << std::endl;" << std::endl;
                break;
            }

            case NODE_IF: { //Si es un NODE_IF
                out << indent(indent_level); //Genera la respectiva identacion para el bloque de codigo del if
                IfNode* current_if = (IfNode*)node; //Guarda en un puntero auxiliar el puntero otorgado

                while (true) {
                    out << "if( ";
                    //Busca y imprime la condicion correspondiente del if utilizando generate_expression_code
                    generate_expression_code(current_if->condition, out);
                    out << " ){" << std::endl;

                    //Se llama recursivamente a generate_statement_code para poder explorar cada parte dentro del bloque de codigo del if
                    generate_statement_code(current_if->thenBranch, out, indent_level + 1);

                    out << indent(indent_level) << "}"; //Coloca la respectiva identacion de la llave final del if

                    //Toma el puntero hacia una sentencia else o else if conectada al if actual
                    Node* else_branch = current_if->elseBranch;

                    //Si es que el puntero no es nullptr y el puntero es de tipo NODE_IF
                    if (else_branch && else_branch->type == NODE_IF) {
                        out << " else "; //Entonces escribe solo "else" para que se pueda generar despues el "if" del "else if"
                        current_if = (IfNode*)else_branch; //Cambia el puntero del "if" actual al siguiente "if" del "else if"
                    } else if (else_branch) { //Si el puntero es solo distinto de nullptr
                        out << " else {" << std::endl; //Entonces va a escribir un "else" solamente
                        //Se llama recursivamente a generate_statement_code para poder explorar cada parte dentro del bloque de codigo del else
                        generate_statement_code(else_branch, out, indent_level + 1); 
                        out << indent(indent_level) << "}"; //Coloca la respectiva identacion de la llave final del else
                        break;
                    } else { //Si es que no hay "else" 
                        break;
                    }
                }
                out << std::endl;
                break;
            }
            case NODE_ASSIGNMENT: { //Si es un NODE_ASSIGNMENT
                out << indent(indent_level); //Coloca su respectiva identacion
                AssignmentNode* assign_node = (AssignmentNode*)node; //Guarda en un puntero auxiliar el puntero otorgado
                
                //Busca y imprime el nombre de la variable correspondiente a la asignacion utilizando generate_expression_code
                generate_expression_code((Node*)assign_node->identifier, out);
                
                out << " = ";
                
                //Busca y imprime el valor correspondiente a la expresion utilizando generate_expression_code
                generate_expression_code(assign_node->expression, out);
                
                out << ";" << std::endl;
                break;
            }
            case NODE_BREAK: { //Si es un NODE_BREAK
                //Coloca la respectivq identacion y escribe su "traduccion"
                out << indent(indent_level) << "break;" << std::endl;
                break;
            }
            case NODE_CONTINUE: { //Si es un NODE_CONTINUE
                //Coloca la respectivq identacion y escribe su "traduccion"
                out << indent(indent_level) << "continue;" << std::endl;
                break;
            }
            case NODE_WHILE: { //Si es un NODE_WHILE
                out << indent(indent_level); //Coloca la respectivq identacion del while  
                WhileNode* while_node = (WhileNode*)node; //Guarda en un puntero auxiliar el puntero otorgado
                
                out << "while ";
                //Busca y imprime la condicion correspondiente del while utilizando generate_expression_code
                generate_expression_code(while_node->condition, out);
                out << " {" << std::endl;

                //Se llama recursivamente a generate_statement_code para poder explorar cada parte dentro del bloque de codigo del while
                generate_statement_code(while_node->body, out, indent_level + 1);

                out << indent(indent_level) << "}" << std::endl; //Coloca la respectiva identacion de la llave final del while
                break;
            }
            case NODE_FOR: { //Si es un
                out << indent(indent_level); //Coloca la respectivq identacion del for
                ForNode* for_node = (ForNode*)node; //Guarda en un puntero auxiliar el puntero otorgado
                out << "for (";
                
                //Verifica que el puntero al apartado del for donde va la variable con el numero de partida sea distinto de nullptr
                if (for_node->initialization) { 
                    //Si es que el apartado donde va el numero de partida del for es una declaracion de variable
                    if (for_node->initialization->type == NODE_VAR_DECLARATION) {
                        //Guarda en un puntero auxiliar el puntero de la declaracion
                        VarDeclarationNode* decl_node = (VarDeclarationNode*)for_node->initialization;
                        IdentifierNode* id_node = decl_node->identifier;//Toma el puntero del identificador de la declaracion
                        C_SymbolInfo info;
                        //Verifica si es que en la tabla de simbolos existe alguna variable declarada con ese nombre
                        if (c_lookup_symbol(id_node->sval, &info)) {
                            //Si la hay toma el tipo en su forma enum y la traduce con map_type_to_cpp
                            out << map_type_to_cpp(info.type) << " " << id_node->sval;
                        }
                        if (decl_node->initialValue) {//Si esque el puntero hacia el valor de la declaracion es distinto de nullptr
                            out << " = ";
                            //Busca y imprime el valor correspondiente de la variable declarada utilizando generate_expression_code
                            generate_expression_code(decl_node->initialValue, out);
                        }
                    } else {
                        //Busca y imprime el valor correspondiente a la expresion de partida del for utilizando generate_expression_code
                        generate_expression_code(for_node->initialization, out);
                    }
                }
                out << "; ";

                if (for_node->condition) {
                    //Busca y imprime la condicion correspondiente del for utilizando generate_expression_code
                    generate_expression_code(for_node->condition, out);
                }
                out << "; ";

                if (for_node->increment) {
                    //Busca y imprime la expresion correspondiente del incremento dentro del for utilizando generate_expression_code
                    generate_expression_code(for_node->increment, out);
                }
                out << ") {" << std::endl;

                generate_statement_code(for_node->body, out, indent_level + 1);

                out << indent(indent_level) << "}" << std::endl; //Coloca la respectiva identacion de la llave final del for
                break;
            }

            case NODE_READ: { //Si es un NODE_READ
                out << indent(indent_level); //Coloca la respectiva identacion 
                ReadNode* read_node = (ReadNode*)node; //Guarda en un puntero auxiliar el puntero otorgado
                
                //Coloca el nombre de la variable utilizada para guardar lo escrito por teclado que esta en el parametro del read
                out << "std::cin >> " << read_node->identifier->sval << ";" << std::endl;
                
                break;
            }
            case NODE_RETURN: { //Si es un NODE_RETURN
                out << indent(indent_level); //Coloca la respectiva identacion 
                ReturnNode* return_node = (ReturnNode*)node; //Guarda en un puntero auxiliar el puntero otorgado
                out << "return";
                if (return_node->returnValue) { //Si es que el return tiene que retornar algun valor
                    out << " ";
                    //Busca y imprime el valor correspondiente a la expresion que debe retornar utilizando generate_expression_code
                    generate_expression_code(return_node->returnValue, out); 
                }

                out << ";" << std::endl;
                break;
            }
            case NODE_FUNCTION_CALL: { //Si es un NODE_FUNCTION_CALL
                out << indent(indent_level); //Coloca la respectiva identacion
                //Busca y imprime la llamada correspondiente a la funcion utilizando generate_expression_code
                generate_expression_code(node, out); 
                out << ";" << std::endl;
                break;
            }
            default: //Si es que la sentencia no ha sido implementada paratraducir, lo reporta
                out << indent(indent_level) << "// Sentencia (tipo " << node->type << ") no implementada." << std::endl;
                break;
        }
        node = node->next;
    }
}

//Traduce un sub-árbol de expresion del AST a su cadena de texto C++ equivalente
static void generate_expression_code(Node* node, std::stringstream& out) {
    if (!node) return;

    switch (node->type) {
        case NODE_CONSTANT_STRING: { //Si es un NODE_CONSTANT_STRING
            StringLiteralNode* str_node = (StringLiteralNode*)node; //Guarda en un puntero auxiliar el puntero otorgado
            out << "\"" << str_node->sval << "\""; //Escribe la cadena de texto correspondiente del string
            break;
        }
        case NODE_CONSTANT_INT: { //Si es un NODE_CONSTANT_INT
            out << ((ConstantIntNode*)node)->value; //Escribe el valor del int
            break;
        }

        case NODE_CONSTANT_FLOAT: { //Si es un NODE_CONSTANT_FLOAT
            out << ((ConstantFloatNode*)node)->value; //Escribe el valor del float
            break;
        }

        case NODE_CONSTANT_BOOL: { //Si es un NODE_CONSTANT_BOOL
            out << (((ConstantBoolNode*)node)->value ? "true" : "false"); //Escribe el valor del bool(true o false)
            break;
        }

        case NODE_IDENTIFIER: { //Si es un NODE_IDENTIFIER
            out << ((IdentifierNode*)node)->sval; //Escribe el nombre del identificador
            break;
        }

        case NODE_BINARY_OP: { //Si es un NODE_BINARY_OP
            BinaryOpNode* op_node = (BinaryOpNode*)node; //Guarda en un puntero auxiliar el puntero otorgado
            out << "(";
            //Se llama recursivamente a generate_expression_code para buscar y escribir 
            //el valor o el nombre de la expresion de la izquierda correspondiente en la operacion
            generate_expression_code(op_node->left, out);

            // Mapea el token del operador a su string en C++
            switch(op_node->op) {
                case PLUS:   out << " + "; break;
                case MINUS:  out << " - "; break;
                case MULT:   out << " * "; break;
                case DIV:    out << " / "; break;
                case EQ:     out << " == "; break;
                case NE:     out << " != "; break;
                case LT:     out << " < "; break;
                case GT:     out << " > "; break;
                case LE:     out << " <= "; break;
                case GE:     out << " >= "; break;
                case AND_OP: out << " && "; break;
                case OR_OP:  out << " || "; break;
                default:     out << " /* op desconocido */ "; 
                break;
            }

            //Se llama recursivamente a generate_expression_code para buscar y escribir 
            //el valor o el nombre de la expresion de la derecha correspondiente en la operacion
            generate_expression_code(op_node->right, out);
            out << ")";
            break;
        }

        case NODE_ASSIGNMENT: { //Si es un NODE_ASSIGNMENT
            AssignmentNode* assign_node = (AssignmentNode*)node; //Guarda en un puntero auxiliar el puntero otorgado
            out << "(";
            //Se llama recursivamente a generate_expression_code para buscar y escribir el nombre de la variable correspondiente a la asignacion
            generate_expression_code((Node*)assign_node->identifier, out);
            out << " = ";
            //Se llama recursivamente a generate_expression_code para buscar y escribir 
            //el valor o el nombre de la expresion de la derecha correspondiente a la asignacion 
            generate_expression_code(assign_node->expression, out);
            out << ")";
            break;
        }
        case NODE_FUNCTION_CALL: { //Si es un NODE_FUNCTION_CALL
            FunctionCallNode* call_node = (FunctionCallNode*)node; //Guarda en un puntero auxiliar el puntero otorgado
            out << call_node->name->sval << "("; //Escribe el nombre de la funcion que ha sido llamada

            if (call_node->arguments) { //Verifica que el puntero hacia los argumentos del llamado de la funcion sea distinto de nullptr
                Node* current_arg = call_node->arguments; //Guarda en un puntero auxiliar los argumentos del llamado de la funcion
                bool first_arg = true; //Variable bool auxiliar inicializada en true
                while (current_arg) { //Mientras el puntero al argumento sea distinto de nullptr
                    if (!first_arg) { //Si la variable es false, escribe la coma que separa los argumentos de la llamada de la funcion
                        out << ", ";
                    }
                    //Se llama recursivamente a generate_expression_code para buscar y escribir 
                    //el valor o el nombre de la expresion correspondiente al argumento en el llamado de la funcion 
                    generate_expression_code(current_arg, out);
                    first_arg = false; //Cambia el valor a false para poder separar los argumentos siguientes con coma
                    current_arg = current_arg->next; //Pasa al siguiente argumento
                }
            }
            out << ")";
            break;
        }
        case NODE_UNARY_OP: { //Si es un NODE_UNARY_OP
            UnaryOpNode* un_node = (UnaryOpNode*)node; //Guarda en un puntero auxiliar el puntero otorgado
            out << "("; 
            switch(un_node->op) { //Mapea la operacion unaria al string en C++ correspondiente
                case NOT_OP: out << "!"; break;
                case MINUS:  out << "-"; break;
                default:     out << "/* op_unario_desconocido */"; break;
            }
            //Se llama recursivamente a generate_expression_code para buscar y escribir 
            //el valor o el nombre de la expresion correspondiente sobre el que actua el operador unario
            generate_expression_code(un_node->operand, out);
            out << ")";
            break;
        }

        default:
            //Si es que la expresion no ha sido implementada para traducir, lo reporta
            out << "/* expr (tipo " << node->type << ") no implementada */";
            break;
    }
}