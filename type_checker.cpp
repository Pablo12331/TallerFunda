#include <iostream>
#include <string>   
#include <vector>          
#include "type_checker.h"
#include "Ast.h"          
#include "symbol_table.h"
#include "parser.tab.h" 

static int contadorErroresDeTipo = 0;

static void verificarTiposEnNodoRecursivo(Node* nodo, SymbolBasicType tipoRetornoFuncionActual);

void reportarErrorDeTipo(const std::string& mensaje, int numLinea) {
    std::cerr << "Error de Tipo (línea " << numLinea << "): " << mensaje << std::endl;
    contadorErroresDeTipo++;
}

std::string symbolBasicTypeToString(SymbolBasicType type) {
    switch (type) {
        case SYM_TYPE_INT: return "INT";
        case SYM_TYPE_FLOAT: return "FLOAT";
        case SYM_TYPE_STRING: return "STRING";
        case SYM_TYPE_BOOL: return "BOOL";
        case SYM_TYPE_VOID: return "VOID";
        case SYM_TYPE_ERROR: return "ERROR_TYPE";
        case SYM_TYPE_UNKNOWN: default: return "UNKNOWN_TYPE";
    }
}

bool realizarVerificacionDeTipos(Node* raizDelAst) {
    contadorErroresDeTipo = 0;
    if (!raizDelAst) {
        return true; 
    }
    verificarTiposEnNodoRecursivo(raizDelAst, SYM_TYPE_UNKNOWN);
    return (contadorErroresDeTipo == 0);
}



SymbolBasicType obtenerYVerificarTipo(Node* nodoExpresion) {
    if (!nodoExpresion) {
        return SYM_TYPE_UNKNOWN; 
    }

    SymbolBasicType tipoResultante = SYM_TYPE_UNKNOWN;

    switch (nodoExpresion->type) {
        case NODE_CONSTANT_INT:
            tipoResultante = SYM_TYPE_INT;
            break;
        case NODE_CONSTANT_FLOAT:
            tipoResultante = SYM_TYPE_FLOAT;
            break;
        case NODE_CONSTANT_STRING:
            tipoResultante = SYM_TYPE_STRING;
            break;
        case NODE_CONSTANT_BOOL:
            tipoResultante = SYM_TYPE_BOOL;
            break;

        case NODE_IDENTIFIER: {
            IdentifierNode* idNode = (IdentifierNode*)nodoExpresion;
            C_SymbolInfo info;
            if (c_lookup_symbol(idNode->sval, &info)) {
                if (info.kind == NODE_FUNCTION_DEF) {
                    reportarErrorDeTipo("El nombre de función '" + std::string(idNode->sval) + "' no se puede usar como una variable/valor.", idNode->line);
                    tipoResultante = SYM_TYPE_ERROR;
                } else {
                    tipoResultante = info.type;
                }
            } else {
                 tipoResultante = SYM_TYPE_ERROR; 
            }
            break;
        }

        case NODE_BINARY_OP: {
            BinaryOpNode* bn = (BinaryOpNode*)nodoExpresion;
            SymbolBasicType tipoIzq = obtenerYVerificarTipo(bn->left);
            SymbolBasicType tipoDer = obtenerYVerificarTipo(bn->right);

            if (tipoIzq == SYM_TYPE_ERROR || tipoDer == SYM_TYPE_ERROR) {
                tipoResultante = SYM_TYPE_ERROR; 
                break; 
            }
            if (tipoIzq == SYM_TYPE_UNKNOWN || tipoDer == SYM_TYPE_UNKNOWN) {
                 reportarErrorDeTipo("Operando con tipo desconocido en operación binaria.", bn->line);
                 tipoResultante = SYM_TYPE_ERROR;
                 break;
            }
            if (tipoIzq == SYM_TYPE_VOID || tipoDer == SYM_TYPE_VOID) {
                 reportarErrorDeTipo("Operando de tipo VOID no permitido en operación binaria.", bn->line);
                 tipoResultante = SYM_TYPE_ERROR;
                 break;
            }


            switch (bn->op) {
                case PLUS:
                case MINUS:
                case MULT:
                case DIV:
                    if (tipoIzq == SYM_TYPE_INT && tipoDer == SYM_TYPE_INT) tipoResultante = SYM_TYPE_INT;
                    else if (tipoIzq == SYM_TYPE_FLOAT && tipoDer == SYM_TYPE_FLOAT) tipoResultante = SYM_TYPE_FLOAT;
                    else if ((tipoIzq == SYM_TYPE_INT && tipoDer == SYM_TYPE_FLOAT) ||
                             (tipoIzq == SYM_TYPE_FLOAT && tipoDer == SYM_TYPE_INT)) tipoResultante = SYM_TYPE_FLOAT;
                    else if (bn->op == PLUS && tipoIzq == SYM_TYPE_STRING && tipoDer == SYM_TYPE_STRING) tipoResultante = SYM_TYPE_STRING; // Concatenación
                    else {
                        reportarErrorDeTipo("Tipos incompatibles para operación aritmética.", bn->line);
                        tipoResultante = SYM_TYPE_ERROR;
                    }
                    break;
                case EQ:
                case NE:
                    if (((tipoIzq == SYM_TYPE_INT || tipoIzq == SYM_TYPE_FLOAT) && (tipoDer == SYM_TYPE_INT || tipoDer == SYM_TYPE_FLOAT)) ||
                        (tipoIzq == SYM_TYPE_BOOL && tipoDer == SYM_TYPE_BOOL) ||
                        (tipoIzq == SYM_TYPE_STRING && tipoDer == SYM_TYPE_STRING)) {
                        tipoResultante = SYM_TYPE_BOOL;
                    } else {
                        reportarErrorDeTipo("Tipos incompatibles para operación de igualdad/desigualdad.", bn->line);
                        tipoResultante = SYM_TYPE_ERROR;
                    }
                    break;
                case LT:
                case GT:
                case LE:
                case GE:
                    if ((tipoIzq == SYM_TYPE_INT || tipoIzq == SYM_TYPE_FLOAT) &&
                        (tipoDer == SYM_TYPE_INT || tipoDer == SYM_TYPE_FLOAT)) tipoResultante = SYM_TYPE_BOOL;
                    else {
                        reportarErrorDeTipo("Tipos incompatibles para operación de comparación ordenada.", bn->line);
                        tipoResultante = SYM_TYPE_ERROR;
                    }
                    break;
                case AND_OP:
                case OR_OP:
                    if (tipoIzq == SYM_TYPE_BOOL && tipoDer == SYM_TYPE_BOOL) tipoResultante = SYM_TYPE_BOOL;
                    else {
                        reportarErrorDeTipo("Operandos para operación lógica deben ser booleanos.", bn->line);
                        tipoResultante = SYM_TYPE_ERROR;
                    }
                    break;
                default:
                    reportarErrorDeTipo("Operador binario desconocido o no implementado en verificador de tipos.", bn->line);
                    tipoResultante = SYM_TYPE_ERROR;
                    break;
            }
            break; 
        }

        case NODE_UNARY_OP: {
            UnaryOpNode* un = (UnaryOpNode*)nodoExpresion;
            SymbolBasicType tipoOp = obtenerYVerificarTipo(un->operand);

            if (tipoOp == SYM_TYPE_ERROR) {
                tipoResultante = SYM_TYPE_ERROR; 
                break;
            }
             if (tipoOp == SYM_TYPE_UNKNOWN || tipoOp == SYM_TYPE_VOID) {
                 reportarErrorDeTipo("Operando con tipo VOID o desconocido en operación unaria.", un->line);
                 tipoResultante = SYM_TYPE_ERROR;
                 break;
            }

            switch (un->op) {
                case NOT_OP:
                    if (tipoOp == SYM_TYPE_BOOL) tipoResultante = SYM_TYPE_BOOL;
                    else {
                        reportarErrorDeTipo("Operando para NOT debe ser booleano.", un->line);
                        tipoResultante = SYM_TYPE_ERROR;
                    }
                    break;
                case MINUS:
                    if (tipoOp == SYM_TYPE_INT) tipoResultante = SYM_TYPE_INT;
                    else if (tipoOp == SYM_TYPE_FLOAT) tipoResultante = SYM_TYPE_FLOAT;
                    else {
                        reportarErrorDeTipo("Operando para menos unario debe ser numérico.", un->line);
                        tipoResultante = SYM_TYPE_ERROR;
                    }
                    break;
                default:
                    reportarErrorDeTipo("Operador unario desconocido o no implementado.", un->line);
                    tipoResultante = SYM_TYPE_ERROR;
                    break;
            }
            break;
        }
        
        case NODE_FUNCTION_CALL: {
            FunctionCallNode* callNode = (FunctionCallNode*)nodoExpresion;
            IdentifierNode* funcNameNode = callNode->name;
             C_SymbolInfo basicFuncInfo;
            if (c_lookup_symbol(funcNameNode->sval, &basicFuncInfo)) {
                if (basicFuncInfo.kind != NODE_FUNCTION_DEF) {
                    reportarErrorDeTipo("'" + std::string(funcNameNode->sval) + "' no es una función, no se puede llamar.", funcNameNode->line);
                    tipoResultante = SYM_TYPE_ERROR;
                } else {
                    if (global_symbol_manager_instance_ptr) {
                        std::optional<SymbolTableEntry_CPP> funcEntryOpt = 
                            global_symbol_manager_instance_ptr->lookupSymbol(funcNameNode->sval);

                        if (funcEntryOpt && funcEntryOpt->kind == NODE_FUNCTION_DEF) {
                            const std::vector<ST_Param_CPP>& paramsEsperados = funcEntryOpt->params_list;
                            int numArgsPasados = 0;
                            Node* currentArgAst = callNode->arguments;
                            while(currentArgAst) {
                                numArgsPasados++;
                                currentArgAst = currentArgAst->next;
                            }
                            if (numArgsPasados != paramsEsperados.size()) {
                                reportarErrorDeTipo("Número incorrecto de argumentos para la función '" + 
                                                    std::string(funcNameNode->sval) + "'. Se esperaban " + 
                                                    std::to_string(paramsEsperados.size()) + " pero se pasaron " + 
                                                    std::to_string(numArgsPasados) + ".", callNode->line);
                                tipoResultante = SYM_TYPE_ERROR;
                            } else {
                                currentArgAst = callNode->arguments;
                                bool todosLosArgsCompatibles = true;
                                for (size_t i = 0; i < paramsEsperados.size(); ++i) {
                                    if (!currentArgAst) { break; }
                                    
                                    SymbolBasicType tipoArgPasado = obtenerYVerificarTipo(currentArgAst);
                                    SymbolBasicType tipoParamEsperado = paramsEsperados[i].type;

                                    if (tipoArgPasado != SYM_TYPE_ERROR) { 
                                        bool compatible = false;
                                        if (tipoParamEsperado == tipoArgPasado) compatible = true;
                                        else if (tipoParamEsperado == SYM_TYPE_FLOAT && tipoArgPasado == SYM_TYPE_INT) compatible = true;

                                        if (!compatible) {
                                            reportarErrorDeTipo("Tipo de argumento " + std::to_string(i+1) + 
                                                                " (" + symbolBasicTypeToString(tipoArgPasado) + 
                                                                ") incompatible con el parámetro esperado (" + 
                                                                symbolBasicTypeToString(tipoParamEsperado) + 
                                                                ") para la función '" + std::string(funcNameNode->sval) + "'.",
                                                                currentArgAst->line);
                                            todosLosArgsCompatibles = false;
                                        }
                                    } else {
                                        todosLosArgsCompatibles = false; 
                                    }
                                    currentArgAst = currentArgAst->next;
                                }

                                if (todosLosArgsCompatibles) {
                                    tipoResultante = funcEntryOpt->type;
                                } else {
                                    tipoResultante = SYM_TYPE_ERROR;
                                }
                            }
                        } else { 
                            reportarErrorDeTipo("Error interno: No se pudo obtener la información detallada de la función '" + std::string(funcNameNode->sval) + "'.", funcNameNode->line);
                            tipoResultante = SYM_TYPE_ERROR;
                        }
                    } else { 
                         reportarErrorDeTipo("Error crítico: SymbolManager no disponible para verificar llamada a función.", funcNameNode->line);
                         tipoResultante = SYM_TYPE_ERROR;
                    }
                }
            } else { 
                tipoResultante = SYM_TYPE_ERROR;
            }
            break;
        }


        default:
            tipoResultante = SYM_TYPE_UNKNOWN; 
            break;
    }

    return tipoResultante;
}


static void verificarTiposEnNodoRecursivo(Node* nodo, SymbolBasicType tipoRetornoFuncionActual) {
    if (!nodo) {
        return;
    }

    switch (nodo->type) {
        case NODE_STATEMENT_LIST: {
            StatementListNode* list = (StatementListNode*)nodo;
            if (list->statements) {
                for (Node* stmt : *(list->statements)) {
                    verificarTiposEnNodoRecursivo(stmt, tipoRetornoFuncionActual); 
                }
            }
            break;
        }
        case NODE_VAR_DECLARATION: {
            VarDeclarationNode* decl = (VarDeclarationNode*)nodo;

            if (decl->initialValue) {
                SymbolBasicType tipoDeclarado = SYM_TYPE_UNKNOWN;
                C_SymbolInfo varInfo;
                if (decl->identifier && decl->identifier->type == NODE_IDENTIFIER) {
                    if (c_lookup_symbol(((IdentifierNode*)(decl->identifier))->sval, &varInfo)) {
                        tipoDeclarado = varInfo.type;
                    }
                }
                
                SymbolBasicType tipoInicializacion = obtenerYVerificarTipo(decl->initialValue);

                if (tipoDeclarado != SYM_TYPE_ERROR && tipoDeclarado != SYM_TYPE_UNKNOWN && 
                    tipoInicializacion != SYM_TYPE_ERROR && tipoInicializacion != SYM_TYPE_UNKNOWN) {
                    bool compatible = false;
                    if (tipoDeclarado == tipoInicializacion) compatible = true;
                    else if (tipoDeclarado == SYM_TYPE_FLOAT && tipoInicializacion == SYM_TYPE_INT) compatible = true; 

                    if (!compatible) {
                        reportarErrorDeTipo("Tipo de la expresión de inicialización incompatible con el tipo declarado de la variable '" + 
                                            std::string(((IdentifierNode*)decl->identifier)->sval) + "'.", decl->line);
                    }
                }
            }
            break;
        }
        case NODE_ASSIGNMENT: {
            AssignmentNode* assignNode = (AssignmentNode*)nodo;
            SymbolBasicType tipoLHS = obtenerYVerificarTipo((Node*)assignNode->identifier); 
            SymbolBasicType tipoRHS = obtenerYVerificarTipo(assignNode->expression);

            if (tipoLHS != SYM_TYPE_ERROR && tipoRHS != SYM_TYPE_ERROR) {
                bool compatible = false;
                if (tipoLHS == tipoRHS) compatible = true;
                else if (tipoLHS == SYM_TYPE_FLOAT && tipoRHS == SYM_TYPE_INT) compatible = true;

                if (!compatible) {
                    reportarErrorDeTipo("Tipos incompatibles en la asignación. No se puede asignar tipo " + 
                    symbolBasicTypeToString(tipoRHS) + " a tipo " + 
                    symbolBasicTypeToString(tipoLHS) + ".", 
                    assignNode->line);
                }
            }
            break;
        }
        case NODE_PRINT: {
            PrintNode* printNode = (PrintNode*)nodo;
            SymbolBasicType exprType = obtenerYVerificarTipo(printNode->expression);
            if (exprType == SYM_TYPE_VOID || exprType == SYM_TYPE_ERROR) { 
                reportarErrorDeTipo("Expresión no imprimible (VOID o con errores previos) en PRINT.", printNode->line);
            }
            break;
        }
        case NODE_IF: {
            IfNode* ifNode = (IfNode*)nodo;
            SymbolBasicType condType = obtenerYVerificarTipo(ifNode->condition);
            if (condType != SYM_TYPE_BOOL && condType != SYM_TYPE_ERROR) {
                reportarErrorDeTipo("La condición del IF debe ser de tipo booleano.", ifNode->condition->line);
            }
            verificarTiposEnNodoRecursivo(ifNode->thenBranch, tipoRetornoFuncionActual);
            if (ifNode->elseBranch) {
                verificarTiposEnNodoRecursivo(ifNode->elseBranch, tipoRetornoFuncionActual);
            }
            break;
        }
        case NODE_WHILE: {
            WhileNode* whileNode = (WhileNode*)nodo;
            SymbolBasicType condType = obtenerYVerificarTipo(whileNode->condition);
            if (condType != SYM_TYPE_BOOL && condType != SYM_TYPE_ERROR) {
                reportarErrorDeTipo("La condición del WHILE debe ser de tipo booleano.", whileNode->condition->line);
            }
            verificarTiposEnNodoRecursivo(whileNode->body, tipoRetornoFuncionActual);
            break;
        }
        case NODE_FOR: { 
            ForNode* forNode = (ForNode*)nodo;
            if (forNode->initialization) obtenerYVerificarTipo(forNode->initialization); 
            if (forNode->condition) {
                SymbolBasicType condType = obtenerYVerificarTipo(forNode->condition);
                 if (condType != SYM_TYPE_BOOL && condType != SYM_TYPE_ERROR && condType != SYM_TYPE_UNKNOWN /*empty expr*/) {
                    reportarErrorDeTipo("La condición del FOR debe ser de tipo booleano.", forNode->condition->line);
                }
            }
            if (forNode->increment) obtenerYVerificarTipo(forNode->increment);
            verificarTiposEnNodoRecursivo(forNode->body, tipoRetornoFuncionActual);
            break;
        }
        case NODE_RETURN: {
            ReturnNode* returnNode = (ReturnNode*)nodo;
            SymbolBasicType tipoExpresionRetornada = SYM_TYPE_VOID; 
            if (returnNode->returnValue) {
                tipoExpresionRetornada = obtenerYVerificarTipo(returnNode->returnValue);
            }
            if (tipoRetornoFuncionActual == SYM_TYPE_UNKNOWN) { 
                if (returnNode->returnValue || tipoExpresionRetornada != SYM_TYPE_VOID) {
                     reportarErrorDeTipo("Sentencia 'return' no permitida fuera de una función.", returnNode->line);
                }
            } else if (tipoRetornoFuncionActual == SYM_TYPE_VOID) {
                if (returnNode->returnValue && tipoExpresionRetornada != SYM_TYPE_VOID) { 
                    reportarErrorDeTipo("Una función VOID no puede retornar un valor.", returnNode->line);
                }
            } else {
                if (!returnNode->returnValue) { 
                    reportarErrorDeTipo("Función no-VOID debe retornar un valor.", returnNode->line);
                } else if (tipoExpresionRetornada != SYM_TYPE_ERROR) { 
                    bool compatible = false;
                    if (tipoRetornoFuncionActual == tipoExpresionRetornada) compatible = true;
                    else if (tipoRetornoFuncionActual == SYM_TYPE_FLOAT && tipoExpresionRetornada == SYM_TYPE_INT) compatible = true; 

                    if (!compatible) {
                        reportarErrorDeTipo("Tipo de valor de retorno (" + symbolBasicTypeToString(tipoExpresionRetornada) +
                                            ") incompatible con el tipo de retorno declarado de la función (" +
                                            symbolBasicTypeToString(tipoRetornoFuncionActual) + ").", returnNode->line);
                    }
                }
            }
            break;
        }
        case NODE_FUNCTION_DEF: {
            FunctionDefNode* funcDefNode = (FunctionDefNode*)nodo;
            SymbolBasicType tipoRetornoEsperadoParaEsteCuerpo = SYM_TYPE_UNKNOWN;

            if (funcDefNode->signature && 
                ((FunctionSignatureNode*)funcDefNode->signature)->returnType) {
                tipoRetornoEsperadoParaEsteCuerpo = 
                    c_ast_node_to_symbol_basic_type(((FunctionSignatureNode*)funcDefNode->signature)->returnType);
            } else {
                reportarErrorDeTipo("Definición de función incompleta, no se pudo determinar el tipo de retorno.", funcDefNode->line);
            }
            
            if (funcDefNode->body) {
                verificarTiposEnNodoRecursivo(funcDefNode->body, tipoRetornoEsperadoParaEsteCuerpo);
            }
            break;
        }
        case NODE_FUNCTION_CALL:
            obtenerYVerificarTipo(nodo);
            break;
        case NODE_BINARY_OP:
        case NODE_UNARY_OP:
            obtenerYVerificarTipo(nodo);
            break;
        case NODE_CONSTANT_INT:
        case NODE_CONSTANT_FLOAT:
        case NODE_CONSTANT_STRING:
        case NODE_CONSTANT_BOOL:
        case NODE_IDENTIFIER:
        case NODE_TYPE:
        case NODE_FUNC_SIG:
            break;

        default:
            break;
    }
}