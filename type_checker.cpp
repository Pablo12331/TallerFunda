#include <iostream>
#include <string>   
#include <vector>          
#include "type_checker.h"
#include "Ast.h"          
#include "parser.tab.h" 

static int typeErrorCounter = 0; //Contador que guarda la cantidad de errores que hubo en el arbol AST
static void checkTypesInNodeRecursive(Node* node, SymbolBasicType currentFunctionReturnType, bool isInLoop);

//MEtodo para reportar error en la linea correspondiente
void reportTypeError(const std::string& message, int lineNumber) {
    std::cerr << "Error de Tipo (línea " << lineNumber << "): " << message << std::endl;
    typeErrorCounter++;
    exit(1);
}

//Metodo utilizado para tener mayor claridad a la hora de reportar errores
std::string symbolBasicTypeToString(SymbolBasicType type) {
    switch (type) {
        case SYM_TYPE_INT: return "INT";
        case SYM_TYPE_FLOAT: return "FLOAT";
        case SYM_TYPE_STRING: return "STRING";
        case SYM_TYPE_BOOL: return "BOOL";
        case SYM_TYPE_VOID: return "VOID";
        case SYM_TYPE_ERROR: return "ERROR_TYPE";
        case SYM_TYPE_UNCHECKED: return "UNCHECKED";
        case SYM_TYPE_UNKNOWN: default: return "UNKNOWN_TYPE";
    }
}

//Metodo utilizado para tener mayor claridad a la hora de reportar errores
static std::string op_token_to_string(int op) {
    switch (op) {
        case PLUS:   return "+";
        case MINUS:  return "-";
        case MULT:   return "*";
        case DIV:    return "/";
        case EQ:     return "==";
        case NE:     return "!=";
        case LT:     return "<";
        case GT:     return ">";
        case LE:     return "<=";
        case GE:     return ">=";
        case AND_OP: return "&&";
        case OR_OP:  return "||";
        case NOT_OP: return "!";
        default:     return "unknown_op";
    }
}

//Es el encargado de verificar antes de empezar el analisis semantico del arbol AST
bool performTypeChecking(Node* astRoot) {
    typeErrorCounter = 0; //Inicializa el contador en 0
    if (!astRoot) { //Verifica que el arbol no este vacio(nullptr)
        return true; 
    }
    //Inicial el recorrido recursivo del arbol AST
    //currentFunctionReturnType parametro inicializado en SYM_TYPE_UNKNOWN para que se pueda ver si es que hay algun return fuera de una funcion
    checkTypesInNodeRecursive(astRoot, SYM_TYPE_UNKNOWN, false);
    return (typeErrorCounter == 0);
}

//Calculador de tipos especializado en expresiones(Trabaja de manera recursiva)
//toma una expresion del tipo "int + float" y restorna el resultado del tipo de dato que sale de esa operacion en ese caso "float"
SymbolBasicType getAndCheckType(Node* expressionNode) {
    if (!expressionNode) { //Verifica que no le hayan pasado un puntero vacia
        return SYM_TYPE_UNKNOWN; 
    }

    SymbolBasicType resultingType = SYM_TYPE_UNKNOWN; //Inicializa el tipo de dato en "desconocido"

    switch (expressionNode->type) {
        //En el caso de que sea un simple literal, directamente retornar su valor
        case NODE_CONSTANT_INT: 
            resultingType = SYM_TYPE_INT; //int
            break;
        case NODE_CONSTANT_FLOAT:
            resultingType = SYM_TYPE_FLOAT; //float
            break;
        case NODE_CONSTANT_STRING:
            resultingType = SYM_TYPE_STRING; //string
            break;
        case NODE_CONSTANT_BOOL:
            resultingType = SYM_TYPE_BOOL; //bool
            break;

        case NODE_IDENTIFIER: { //Caso de un identificador (Nombre de una variable)
            IdentifierNode* idNode = (IdentifierNode*)expressionNode; //Guarda el puntero en un puntero auxiliar del mismo tipo
            C_SymbolInfo info;
            if (c_lookup_symbol(idNode->sval, &info)) { //Busca en la tabla de simbolos para ver si es que existe
                if (info.kind == NODE_FUNCTION_DEF) { //Si es que lo encuentra pero su identificador es el de una funcion
                    reportTypeError("El nombre '" + std::string(idNode->sval) + "' es de una funcion, no se puede usar como una variable/valor.", idNode->line);
                    resultingType = SYM_TYPE_ERROR; //Guarda un tipo error, ya que se intento declarar una variable con el mismo nombre de una funcion
                } else { //En caso contrario, guarda un tipo error para luego
                    resultingType = info.type;
                }
            } else {
                 resultingType = SYM_TYPE_ERROR; //En caso de que no lo encuentre en la tabla de simbolos, entonces tambien guarda un tipo error
            }
            break;
        }

        case NODE_BINARY_OP: { //Caso de una operacion con 2 variables
            BinaryOpNode* bn = (BinaryOpNode*)expressionNode; //Guarda el puntero en un puntero auxiliar del mismo tipo

            //Flujo encargado de ver si es que hay division por 0 para reportar el error 
            if (bn->op == DIV) { 
                Node* right_operand = bn->right;
                if (right_operand->type == NODE_CONSTANT_INT && ((ConstantIntNode*)right_operand)->value == 0) { //Para el caso de un int
                    reportTypeError("División por la constante cero.", bn->line);
                    resultingType = SYM_TYPE_ERROR;
                    expressionNode->calculatedType = resultingType; 
                    return resultingType; //Retorna directamente
                }
                if (right_operand->type == NODE_CONSTANT_FLOAT && ((ConstantFloatNode*)right_operand)->value == 0.0) { //Para el caso de un float
                    reportTypeError("División por la constante cero.", bn->line);
                    resultingType = SYM_TYPE_ERROR;
                    expressionNode->calculatedType = resultingType;
                    return resultingType; //Retorna directamente
                }
            }

            SymbolBasicType leftType = getAndCheckType(bn->left); //Toma el tipo de la variable de la izquierda
            SymbolBasicType rightType = getAndCheckType(bn->right); //Toma el tipo de la varible de la derecha

            if (leftType == SYM_TYPE_ERROR || rightType == SYM_TYPE_ERROR) { //Si alguno tiene guardado un tipo error, entonces
                resultingType = SYM_TYPE_ERROR; //Guarda tipo error para luego retornarlo
                break; 
            }
            if (leftType == SYM_TYPE_UNKNOWN || rightType == SYM_TYPE_UNKNOWN) { //Si alguno tiene guardado tipo desconocido, entonces
                reportTypeError("Error!! intento de operar con tipo desconocido en operacion binaria.", bn->line);
                resultingType = SYM_TYPE_ERROR; //Guarda tipo error para luego retornarlo
                break;
            }
            if (leftType == SYM_TYPE_VOID || rightType == SYM_TYPE_VOID) { //Si alguno tiene guardado tipo void, entonces
                reportTypeError("Error!! intento de operar con tipo VOID. No permitido en operacion binaria.", bn->line);
                resultingType = SYM_TYPE_ERROR; //Guarda tipo error para luego retornarlo
                break;
            }
            //Switch que se encarga de dar con el resultado del tipo independientemente de la operacion
            switch (bn->op) {
                case PLUS:
                case MINUS:
                case MULT:
                case DIV:
                    //Esto sirve tanto para el case PLUS(+), MINUS(-), MULT(*) y DIV(/) 
                    //Guarda int si ambos son int
                    if (leftType == SYM_TYPE_INT && rightType == SYM_TYPE_INT) resultingType = SYM_TYPE_INT; 
                    //Guarda float si ambos son float
                    else if (leftType == SYM_TYPE_FLOAT && rightType == SYM_TYPE_FLOAT) resultingType = SYM_TYPE_FLOAT; 
                    //Si es que alguno de los 2 es float, guarda float
                    else if ((leftType == SYM_TYPE_INT && rightType == SYM_TYPE_FLOAT) || 
                             (leftType == SYM_TYPE_FLOAT && rightType == SYM_TYPE_INT)) resultingType = SYM_TYPE_FLOAT; 
                    //Excepcion si el operador es "+" y los 2 son de tipo string, guarda el tipo string (Ayuda a aceptar la concatenacion de strings)
                    else if (bn->op == PLUS && leftType == SYM_TYPE_STRING && rightType == SYM_TYPE_STRING) resultingType = SYM_TYPE_STRING;
                    else {
                        std::string err_msg = "Operador '" + op_token_to_string(bn->op) + "' no se puede aplicar a tipos '" + symbolBasicTypeToString(leftType) + "' y '" + symbolBasicTypeToString(rightType) + "'.";
                        reportTypeError(err_msg, bn->line);
                        resultingType = SYM_TYPE_ERROR; //En cualquier otro caso guarda error
                    }
                    break;
                case EQ:
                case NE:
                    //Esto cubre el case para EQ (==) y NE (!=)
                    if (((leftType == SYM_TYPE_INT || leftType == SYM_TYPE_FLOAT) && (rightType == SYM_TYPE_INT || rightType == SYM_TYPE_FLOAT)) ||
                        (leftType == SYM_TYPE_BOOL && rightType == SYM_TYPE_BOOL) ||
                        (leftType == SYM_TYPE_STRING && rightType == SYM_TYPE_STRING)) {
                        resultingType = SYM_TYPE_BOOL; //Si los valores que se comparan son aceptados, entonces guarda el tipo BOOL
                    } else {
                        std::string err_msg = "No se pueden comparar tipos '" + symbolBasicTypeToString(leftType) + "' y '" + symbolBasicTypeToString(rightType) + "' con operador '" + op_token_to_string(bn->op) + "'.";
                        reportTypeError(err_msg, bn->line);
                        resultingType = SYM_TYPE_ERROR; //Si no son aceptados, guarda tipo error
                    }
                    break;

                case LT:
                case GT:
                case LE:
                case GE:
                    //Esto cubre el case para LT(<), GT(>), LE(<=) y GE(>=)
                    //Si los valores que se estan comparando por su tamaño son float o int, entonces
                    if ((leftType == SYM_TYPE_INT || leftType == SYM_TYPE_FLOAT) &&
                        (rightType == SYM_TYPE_INT || rightType == SYM_TYPE_FLOAT)) resultingType = SYM_TYPE_BOOL; //Guarda el tipo resultado bool
                    else {
                        std::string err_msg = "Operador de comparación '" + op_token_to_string(bn->op) + "' sólo se puede aplicar a tipos numéricos, no a '" + symbolBasicTypeToString(leftType) + "' y '" + symbolBasicTypeToString(rightType) + "'.";
                        reportTypeError(err_msg, bn->line);
                        resultingType = SYM_TYPE_ERROR;// Si no compara un tipo aceptado, guarda error
                    }
                    break;
                case AND_OP:
                case OR_OP:
                    //Esto cubre los case AND_OP(&&) y OR_OP(||)
                    //Si estos operadores estan en medio de un bool
                    if (leftType == SYM_TYPE_BOOL && rightType == SYM_TYPE_BOOL) resultingType = SYM_TYPE_BOOL; //guardan un tipo resultado bool
                    else {
                        std::string err_msg = "Operador lógico '" + op_token_to_string(bn->op) + "' espera operadores booleanos, pero recibió '" + symbolBasicTypeToString(leftType) + "' y '" + symbolBasicTypeToString(rightType) + "'.";
                        reportTypeError(err_msg, bn->line);
                        resultingType = SYM_TYPE_ERROR; //si no estan en medio de 2 bool, entonces guarda error
                    }
                    break;
                default: //Si no entro en ningun case
                    reportTypeError("Operador binario desconocido o no implementado.", bn->line);
                    resultingType = SYM_TYPE_ERROR; //tambien guarda error
                    break;
            }
            break; 
        }

        case NODE_UNARY_OP: { //En el caso de que sea una operacion de una varible 
            UnaryOpNode* un = (UnaryOpNode*)expressionNode; //Guarda en un nodo auxiliar la expresion
            SymbolBasicType operandType = getAndCheckType(un->operand); //Llama recursivamente a getAndCheckType con el nodo del operador(Ejem: !)

            if (operandType == SYM_TYPE_ERROR || operandType == SYM_TYPE_VOID) { //Si el tipo del operador es Error o Void
                resultingType = SYM_TYPE_ERROR; //Guarda el tipo resultado error
                break;
            }
            if (operandType == SYM_TYPE_UNKNOWN) { //Si el tipo del operador es de tipo desconcido
                reportTypeError("Intentando operar con tipo desconocido en operacion unaria.", un->line);
                resultingType= SYM_TYPE_ERROR; //Guarda el tipó resultado error
                break;
            }

            switch (un->op) {
                case NOT_OP: //Para el caso del "!"
                    if (operandType == SYM_TYPE_BOOL) resultingType = SYM_TYPE_BOOL; //Si el operador es de tipo bool guarda el tipo resultado bool
                    else { //Si no es de tipo bool
                        std::string err_msg = "El operador '!' espera un operador de tipo booleano, pero recibió '" + symbolBasicTypeToString(operandType) + "'.";
                        reportTypeError(err_msg, un->line);
                        resultingType = SYM_TYPE_ERROR; //Guarda el tipo resultado error
                    }
                    break;
                case MINUS: //Para el caso del "-"
                    //Si el tipo del operador es int o float
                    if (operandType == SYM_TYPE_INT || operandType == SYM_TYPE_FLOAT) resultingType = operandType; //Guarda el tipo resultado del operador
                    else {//Si no es un tipo que sea aceptado
                        std::string err_msg = "El operador unario menos '-' espera un operador numérico, pero recibió '" + symbolBasicTypeToString(operandType) + "'.";
                        reportTypeError(err_msg, un->line);
                        resultingType = SYM_TYPE_ERROR; //Guarda el tipo resultado error
                    }
                    break;
                default: //Si no entro en ningun case
                    reportTypeError("Operador unario desconocido o no implementado.", un->line);
                    resultingType = SYM_TYPE_ERROR; //Guarda el tipo resultado error 
                    break; 
            }
            break;
        }
        
        case NODE_FUNCTION_CALL: { //En el caso que sea una funcion
            FunctionCallNode* callNode = (FunctionCallNode*)expressionNode; //Guarda en un puntero auxiliar la expresion
            IdentifierNode* funcNameNode = callNode->name; //Guarda el nombre de la funcion 
            C_SymbolInfo basicFuncInfo;
            if (c_lookup_symbol(funcNameNode->sval, &basicFuncInfo)) { //Busca en la tabla de simbolos si es que existe algo llamado como la funcion 
                if (basicFuncInfo.kind != NODE_FUNCTION_DEF) { //Si lo encuentra pero su identificador no es de tipo NODE_FUNCION_DEF(O sea es una variable)
                    reportTypeError("'" + std::string(funcNameNode->sval) + "' no es una función y no se puede llamar.", funcNameNode->line);
                    resultingType = SYM_TYPE_ERROR; //Guarda el tipo resultado error
                } else { //en caso contrario 
                    //Toma la lista de parametros de la funcion
                    const std::vector<ST_Param_CPP>* expectedParams = global_symbol_manager_instance_ptr->getFunctionParamsList(funcNameNode->sval);
                    if (expectedParams) { //Verifica si el puntero guarda un vector valido
                        //Contador de la cantidad de argumentos que fueron posados a al hora de llamar a la funcion
                        int passedArgCount = 0;
                        Node* currentArgAst = callNode->arguments; //Guarda la cantidad de argumentos de la llamada en un puntero auxiliar
                        while(currentArgAst) { //Cuenta la cantidad de argumentos que fueron entregados
                            passedArgCount++;
                            currentArgAst = currentArgAst->next;
                        }

                        //Compara la cantidad de argumentos pasados con la cantidad de argumentos con la que fue declarada la funcion
                        if (passedArgCount != expectedParams->size()) { //Si la cantidad es distinta
                            reportTypeError("Número incorrecto de argumentos para la función '" + std::string(funcNameNode->sval) 
                                            + "'. Se esperan " + std::to_string(expectedParams->size()) + 
                                            ", pero tiene " + std::to_string(passedArgCount) + ".", callNode->line);
                            resultingType = SYM_TYPE_ERROR; //Guarda el tipo resultado error 
                        } else { //En caso de que sea la misma cantidad
                            currentArgAst = callNode->arguments; //Vuelve a pasarle el inicio de la lista enlazada donde estan los parametros
                            bool allArgsCompatible = true; //Marca al inicio como que todos los argumentos son compatibles con su respectivo tipo
                            for (size_t i = 0; i < expectedParams->size(); ++i) { //Itera la misma cantidad de veces que la cantidad de argumentos
                                if (!currentArgAst) { break; } //Verifica de que la lista enlazada de argumentos no sea nullptr
                                
                                //Obtiene el tipo de los argumentos(Ocupado un poco mas adelante para ver si es que alguno tiene un error de tipo)
                                SymbolBasicType passedArgType = getAndCheckType(currentArgAst);

                                SymbolBasicType expectedParamType = (*expectedParams)[i].type; //Guarda el tipo esperado del argumento actual

                                if (passedArgType != SYM_TYPE_ERROR) { //Utiliza passedArgType para verificar que ninguno de los argumentos tenga algun error de tipo
                                    bool compatible = false; //Toma como caso base que la compatibilidad es false

                                    //Si es que el tipo esperado y el entregado son iguales entonces cambia la compatibilidad a true 
                                    if (expectedParamType == passedArgType) compatible = true; 

                                    //Excepcion: si el tipo esperado es de tipo float y el entregado es de tipo int, igual es compatible
                                    //Por ende cambia la compatibilidad a true
                                    else if (expectedParamType == SYM_TYPE_FLOAT && passedArgType == SYM_TYPE_INT) compatible = true; 

                                    if (!compatible) { //Si es que la compatibilidad sigue siendo false
                                        reportTypeError("No hay coincidencia de tipos para el argumento " + std::to_string(i+1) + " en la llamada '" + std::string(funcNameNode->sval) 
                                        + "'. Se esperaba " + symbolBasicTypeToString(expectedParamType) 
                                        + " pero tiene " + symbolBasicTypeToString(passedArgType) + ".", currentArgAst->line);
                                        allArgsCompatible = false; //Cambia el estado de que todo los argumentos son compatibles a false
                                    }
                                } else { //Si es que alguno de los argumentos le tira error entonces
                                    allArgsCompatible = false; //Cambia el estado de que todo los argumentos son compatibles a false 
                                }
                                currentArgAst = currentArgAst->next;
                            }
                            
                            if (allArgsCompatible) { //Si es que todos los argumentos son compatibles
                                resultingType = basicFuncInfo.type; //Guarda el tipo resultado con el tipo de la funcion
                            } else { //Caso contrario
                                resultingType = SYM_TYPE_ERROR; //Guarda el tipo resultado error
                            }
                        }
                    }
                }
            } else { //Si es que en la tabla de simbolos no encuentra nada llamado como el nombre que se nos paso, entonces
                resultingType = SYM_TYPE_ERROR; //Guarda el tipo resultado error
            }
            break;
        }

        default: //Si es que no entra en ningun case
             if (expressionNode->type == NODE_ASSIGNMENT) { //Verifica si es que el tipo de la expresion es NODE_ASSIGNMENT
                 AssignmentNode* assignNode = (AssignmentNode*)expressionNode; //Guarda en un puntero auxiliar la expresion 
                 getAndCheckType(assignNode->expression); //Toma la expresion que se encuentra en la asignacion dada

                 //Guarda el tipo resultado como el tipo del identificador de la asignacion 
                 resultingType = getAndCheckType((Node*)assignNode->identifier); 
            } else { //En caso contrario, lo cataloga como tipo desconocido
                 resultingType = SYM_TYPE_UNKNOWN;  //Guarda el tipo resultado como desconocido
            }
            break;
    }
    expressionNode->calculatedType = resultingType; //Al puntero de la expresion le da como tipo calculado el resultado guardado
    return resultingType;
}

//Metodo que se encarga de recorrer todo el arbol AST de manera recursiva buscando algun error en la semantica y reportandolo
static void checkTypesInNodeRecursive(Node* node, SymbolBasicType currentFunctionReturnType, bool isInLoop) {
    if (!node) { //Verifica que el puntero que fue otorgado no sea nullptr
        return;
    }

    switch (node->type) { //Toma el tipo del puntero
        case NODE_STATEMENT_LIST: { //Si es un NODE_STATEMENT_LIST (Bloque de codigo o una secuencia de sentencias)
            StatementListNode* list = (StatementListNode*)node; //Guarda en un puntero auxiliar el puntero que fue otorgado
            if (list->statements) { //Si la lista enlzada de declaraciones no es nula
                for (Node* stmt : *(list->statements)) { //Explora cada una de las declaraciones
                    //Y se llama recursivamente a checkTypesInNodeRecursive para ver si es que encuentra algun error semantico
                    checkTypesInNodeRecursive(stmt, currentFunctionReturnType, isInLoop);
                }
            }
            break;
        }

        case NODE_VAR_DECLARATION: {
            VarDeclarationNode* decl = (VarDeclarationNode*)node; //Si es un NODE_VAR_DECLARATION
            if (decl->initialValue) { //Si es que fue asignado con un valor
                SymbolBasicType declaredType = c_ast_node_to_symbol_basic_type(decl->varTypeNode); //Busca si es que existe y si esta bien el tipo
                SymbolBasicType initExpressionType = getAndCheckType(decl->initialValue);

                if (declaredType != SYM_TYPE_ERROR && initExpressionType != SYM_TYPE_ERROR) {
                    bool compatible = (declaredType == initExpressionType) || 
                                      (declaredType == SYM_TYPE_FLOAT && initExpressionType == SYM_TYPE_INT);
                    if (!compatible) {
                        reportTypeError("Tipo de inicializador '" + symbolBasicTypeToString(initExpressionType) + 
                                        "' no es compatible con el tipo de variable declarada '" + 
                                        symbolBasicTypeToString(declaredType) + "'.", decl->line);
                    }
                }
            }
            c_insert_variable(decl->identifier->sval, decl->varTypeNode, decl->line, (Node*)decl);
            break;
        }
        case NODE_ASSIGNMENT: { //Si es un NODE_ASSIGNMENT(Ejem: x = "Hola")
            AssignmentNode* assignNode = (AssignmentNode*)node;//Guarda en un puntero auxiliar el puntero otorgado

            //Recibe el tipo de la variable utilizando getAndCheckType
            SymbolBasicType lhsType = getAndCheckType((Node*)assignNode->identifier); 

            //Recibe el tipo del valor al que esta igualada la variable utilizando getAndCheckType
            SymbolBasicType rhsType = getAndCheckType(assignNode->expression); 
            if (lhsType != SYM_TYPE_ERROR && rhsType != SYM_TYPE_ERROR) {//Verifica de que ninguno haya retornado un tipo error
                bool compatible = false; //Toma como caso base el que sean incompatibles

                if (lhsType == rhsType) compatible = true; //Si ambos tienen el mismo tipo cambia el valor de compatible a true
                //Excepcion: Si el tipo de la variable es Float y el tipo del valor al que esta igualada la variable es int
                else if (lhsType == SYM_TYPE_FLOAT && rhsType == SYM_TYPE_INT) compatible = true; //Entonces cambia la variable compatible a true
                if (!compatible) { //Si es que la variable compatible no cambia, muestra el error por consola
                    reportTypeError("Tipos incompatibles en la asignación. No se puede asignar el tipo. " 
                                    + symbolBasicTypeToString(rhsType) + " a '" 
                                    + symbolBasicTypeToString(lhsType) + "'.", assignNode->line);
                }
            }
            break;
        }
        case NODE_PRINT: { //Si es un NODE_PRINT
            PrintNode* printNode = (PrintNode*)node; //Guarda en un puntero auxiliar el puntero otorgado 

            //Recibe el tipo de la expresion que tiene en su parametro utilizando getAndCheckType
            SymbolBasicType exprType = getAndCheckType(printNode->expression); 
            if (exprType == SYM_TYPE_VOID || exprType == SYM_TYPE_ERROR) { //Si el tipo de la expresion es VOID o Error
                reportTypeError("Expresión no imprimible (es nula o tiene errores).", printNode->line);//entonces muestra el error por consola
            }
            break;
        }
        case NODE_READ: { //Si es un NODE_READ
            ReadNode* readNode = (ReadNode*)node; //Guarda en un puntero auxiliar el puntero otorgado 
            //Le otorga el tipo compatible a la variable colocada en donde guardara lo recibido por teclado el read
            getAndCheckType((Node*)readNode->identifier);  
            break;
        }
        case NODE_IF: { //Si es un NODE_IF
            IfNode* ifNode = (IfNode*)node; //Guarda en un puntero auxiliar el puntero otorgado 

            //Reibe el tipo de la condicion (bool si es que no hay ningun error) utilizando getAndCheckType 
            SymbolBasicType condType = getAndCheckType(ifNode->condition); 

            //Si es que el tipo es diferente a bool y error, entonces muestra el error por pantalla
            if (condType != SYM_TYPE_BOOL && condType != SYM_TYPE_ERROR) {  
                reportTypeError("La condición del IF debe ser de tipo BOOL, pero se obtuvo '" 
                                + symbolBasicTypeToString(condType) + "'.", ifNode->condition->line);
            }
            //Llama recursivamente a checkTypesInNodeRecursive para que tome el bloque de codigo que hay dentro del if
            checkTypesInNodeRecursive(ifNode->thenBranch, currentFunctionReturnType, isInLoop);
            if (ifNode->elseBranch) { //Verifica si es que hay un else o un if else conectado a este if
                checkTypesInNodeRecursive(ifNode->elseBranch, currentFunctionReturnType, isInLoop); //Verifica condicion o bloque de codigo, dependiendo
            }
            break;
        }
        case NODE_WHILE: { //Si es un NODE_WHILE 
            WhileNode* whileNode = (WhileNode*)node; //Guarda en un puntero auxiliar el puntero otorgado

            //Reibe el tipo de la condicion (bool si es que no hay ningun error) utilizando getAndCheckType 
            SymbolBasicType condType = getAndCheckType(whileNode->condition); 

             //Si es que el tipo es diferente a bool y error, entonces muestra el error por pantalla
            if (condType != SYM_TYPE_BOOL && condType != SYM_TYPE_ERROR) { 
                reportTypeError("La condición del WHILE debe ser de tipo BOOL, pero se obtuvo '" 
                                + symbolBasicTypeToString(condType) + "'.", whileNode->condition->line);
            }
            //Llama recursivamente a checkTypesInNodeRecursive para que tome el bloque de codigo que hay dentro del While
            checkTypesInNodeRecursive(whileNode->body, currentFunctionReturnType, true);
            break;
        }
        case NODE_FOR: { //Si es un NODE_FOR
            ForNode* forNode = (ForNode*)node; //Guarda en un puntero auxiliar el puntero otorgado
            //Ve si es que en el apartado donde a la variable se le otorga un valor haya sido inicializada
            if (forNode->initialization) { 
                 if (forNode->initialization->type == NODE_VAR_DECLARATION) { //Si es de tipo NODE_VAR_DECLARATION (Ejem: int x)
                    //Llama recursivamente a checkTypesInNodeRecursive y le otorga el nodo que contiene la declaracion
                    //Tambien le pasa "isInLoop" con true, ya que estamos en un for y es un loop
                    checkTypesInNodeRecursive(forNode->initialization, currentFunctionReturnType, true); 
                } else {
                    //Si es que no es de tipo declaracion, entonces le busca el valor al apartado de la inicializacion
                    getAndCheckType(forNode->initialization); //Utilizando getAndCheckType
                }
            }
            if (forNode->condition) {
                //Reibe el tipo de la condicion (bool si es que no hay ningun error) utilizando getAndCheckType 
                SymbolBasicType condType = getAndCheckType(forNode->condition);

                //Si es que el tipo es diferente a bool, error o desconocido, entonces muestra el error por pantalla
                if (condType != SYM_TYPE_BOOL && condType != SYM_TYPE_ERROR && condType != SYM_TYPE_UNKNOWN) {
                    reportTypeError("La condición del bucle FOR debe ser del tipo BOOL, pero se obtuvo '" 
                                    + symbolBasicTypeToString(condType) + "'.", forNode->condition->line);
                }
            }
            //Le da un tipo al apartado de incrementacion del for utilizando getAndCheckType
            if (forNode->increment) getAndCheckType(forNode->increment); 
            //Llama recursivamente a checkTypesInNodeRecursive para que tome el bloque de codigo que hay dentro del for
            checkTypesInNodeRecursive(forNode->body, currentFunctionReturnType, true);
            break;
        }
        case NODE_RETURN: { //Si es un NODE_RETURN
            ReturnNode* returnNode = (ReturnNode*)node; //Guarda en un puntero auxiliar el puntero otorgado
            SymbolBasicType returnedExprType = SYM_TYPE_VOID; //Le pone como valor predeterminado VOID al return 
            if (returnNode->returnValue) { //Si es que retorna un valor (!nullptr)
                returnedExprType = getAndCheckType(returnNode->returnValue); //Verifica y guarda el valor de retorno, utilizando getAndCheckType
            }
            //Si es que el tipo es desconocido probablemente estamos fuera de una funcion
            if (currentFunctionReturnType == SYM_TYPE_UNKNOWN) { 
                //Y si tiene un valor de retorno, entonces muestra error por consola(No se puede retornar fuera de una funcion)
                if (returnNode->returnValue) { 
                     reportTypeError("'Declaración 'return' no permitido fuera de una función.", returnNode->line);
                }
            } else if (currentFunctionReturnType == SYM_TYPE_VOID) { //Si es que la funcion es de tipo void
                //Y el return tiene valor entonces muestra error por consola(Una funcion de tipo void no puede retornar)
                if (returnNode->returnValue) { 
                    reportTypeError("Una función VOID no puede devolver un valor.", returnNode->line);
                }
            } else { //Y en cualquier otra ocasion donde tengamos que retornar un valor
                if (!returnNode->returnValue) { //Si es que el return no tiene un valor muestra un error por consola
                    reportTypeError("La función no es de tipo VOID debe devolver un valor.", returnNode->line);
                } else if (returnedExprType != SYM_TYPE_ERROR) { //Verifica que la expresion de retorno no haya sido marcada como error
                    bool compatible = false; //Marca como base que la compatibilidad es false
                    //Si el tipo con el que la funcion fue declarada y el tipo de la expresion que es retornada son iguales
                    if (currentFunctionReturnType == returnedExprType) compatible = true; //Entonces cambia compatible a true
                    //Excepcion: Si el tipo con el que la funcion fue declarada es Float
                    //y el tipo de la expresion que es retornada es igual a int entonces cambia compatible a true
                    else if (currentFunctionReturnType == SYM_TYPE_FLOAT && returnedExprType == SYM_TYPE_INT) compatible = true; 
                    if (!compatible) { //en el caso de que la variable compatible no haya cambiado, entonces muestra el error por consola
                        reportTypeError("El tipo de valor de retorno (" + symbolBasicTypeToString(returnedExprType) 
                                        + ") es incompatible con el tipo de retorno de la función declarada (" 
                                        + symbolBasicTypeToString(currentFunctionReturnType) + ").", returnNode->line);
                    }
                }
            }
            break;
        }
        case NODE_FUNCTION_DEF: {
            FunctionDefNode* funcDefNode = (FunctionDefNode*)node;
            SymbolBasicType expectedReturnTypeForBody = c_ast_node_to_symbol_basic_type(((FunctionSignatureNode*)funcDefNode->signature)->returnType);
            
            c_enter_scope();
            
            if (((FunctionSignatureNode*)funcDefNode->signature)->parameters) {
                Node* current_param_ast_node = ((FunctionSignatureNode*)funcDefNode->signature)->parameters;
                while (current_param_ast_node != nullptr) {
                    if (current_param_ast_node->type == NODE_VAR_DECLARATION) {
                        VarDeclarationNode* p_decl_node = (VarDeclarationNode*)current_param_ast_node;
                        c_insert_variable(p_decl_node->identifier->sval, p_decl_node->varTypeNode, p_decl_node->line, (Node*)p_decl_node);
                    }
                    current_param_ast_node = current_param_ast_node->next;
                }
            }
            if (funcDefNode->body) {
                checkTypesInNodeRecursive(funcDefNode->body, expectedReturnTypeForBody, false);
            }
            c_exit_scope();
            break;
        }
        case NODE_BREAK: { //Si es un NODE_BREAK
            if (!isInLoop) { //Verifica si es que no esta en un bucle, si es asi muestra error por consola
                reportTypeError("La declaración 'break' solo se puede utilizar dentro de un bucle (while o for).", node->line);
            }
            break;
        }
        case NODE_CONTINUE: { //Si es un NODE_CONTINUE
            if (!isInLoop) {//Verifica si es que no esta en un bucle, si es asi muestra error por consola
                reportTypeError("La instrucción 'continue' solo se puede utilizar dentro de un bucle (while o for).", node->line);
            }
            break;
        }
        case NODE_FUNCTION_CALL://Si es un NODE_FUNCTION_CALL
            getAndCheckType(node); //Llama a getAndCheckType ya que es la llamada de una funcion
            break;
        
        default:
            //Y por defecto si no entra en ningun case 
            getAndCheckType(node); //Llama a getAndCheckType
            break;
    }
}