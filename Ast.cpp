#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <iostream>
#include "Ast.h"
#include "parser.tab.h"
#include "symbol_table.h"

//Implementacion de todas las funciones que estan en Ast.h (INICIO)

//La sentencia malloc(sizeof(VarDeclarationNode)) ayuda a reservar la cantidad exacta de memoria para el tipo de nodo que estamos utilizando
//la sentencia ((Node*)n)->calculatedType = SYM_TYPE_UNCHECKED inicializa al tipo calculado a un estado "no verificado"
//la sentencia if (!n) { fprintf(stderr, "malloc failed\n"); exit(1); } reporta un error por si en algun casual el compilador se queda sin memoria
//la sentancia n->type = NODE_CONSTANT_FLOAT; asigna la etiqueta correcta al nuevo nodo

Node* createConstantIntNode(int value, int line) {
    ConstantIntNode* n = (ConstantIntNode*)malloc(sizeof(ConstantIntNode));
    if (!n) { fprintf(stderr, "malloc failed\n"); exit(1); } 
    n->type = NODE_CONSTANT_INT;
    n->next = nullptr;
    n->line = line;
    ((Node*)n)->calculatedType = SYM_TYPE_UNCHECKED;
    n->value = value;
    return (Node*)n;
}

Node* createConstantFloatNode(double value, int line) {
    ConstantFloatNode* n = (ConstantFloatNode*)malloc(sizeof(ConstantFloatNode));
    if (!n) { fprintf(stderr, "malloc failed\n"); exit(1); }
    n->type = NODE_CONSTANT_FLOAT;
    n->next = nullptr;
    n->line = line;
    ((Node*)n)->calculatedType = SYM_TYPE_UNCHECKED;
    n->value = value;
    return (Node*)n;
}

Node* createConstantBoolNode(int value, int line) {
    ConstantBoolNode* n = (ConstantBoolNode*)malloc(sizeof(ConstantBoolNode));
    if (!n) { fprintf(stderr, "malloc failed\n"); exit(1); }
    n->type = NODE_CONSTANT_BOOL;
    n->next = nullptr;
    n->line = line;
    ((Node*)n)->calculatedType = SYM_TYPE_UNCHECKED;
    n->value = value;
    return (Node*)n;
}

Node* createStringLiteralNode(char* sval, int line) {   
    StringLiteralNode* n = (StringLiteralNode*)malloc(sizeof(StringLiteralNode));
    if (!n) { fprintf(stderr, "malloc failed\n"); exit(1); }
    n->type = NODE_CONSTANT_STRING;
    n->next = nullptr;
    n->line = line;
    ((Node*)n)->calculatedType = SYM_TYPE_UNCHECKED;
    n->sval = sval;
    return (Node*)n;
}

Node* createIdentifierNode(char* sval, int line) {
    IdentifierNode* n = (IdentifierNode*)malloc(sizeof(IdentifierNode));
    if (!n) { fprintf(stderr, "malloc failed\n"); exit(1); }
    n->type = NODE_IDENTIFIER;
    n->next = nullptr;
    n->line = line;
    ((Node*)n)->calculatedType = SYM_TYPE_UNCHECKED;
    n->sval = sval;
    return (Node*)n;
}

Node* createUnaryOpNode(int op, Node* operand, int line) { 
    UnaryOpNode* n = (UnaryOpNode*)malloc(sizeof(UnaryOpNode));
    if (!n) { fprintf(stderr, "malloc failed\n"); exit(1); }
    n->type = NODE_UNARY_OP;
    n->next = nullptr;
    n->line = line;
    ((Node*)n)->calculatedType = SYM_TYPE_UNCHECKED;
    n->op = op;
    n->operand = operand;
    return (Node*)n;
}

Node* createBinaryOpNode(int op, Node* left, Node* right, int line) {
    BinaryOpNode* n = (BinaryOpNode*)malloc(sizeof(BinaryOpNode));
    if (!n) { fprintf(stderr, "malloc failed\n"); exit(1); }
    n->type = NODE_BINARY_OP;
    n->next = nullptr;
    n->line = line;
    ((Node*)n)->calculatedType = SYM_TYPE_UNCHECKED;
    n->op = op;
    n->left = left;
    n->right = right;
    return (Node*)n;
}

Node* createAssignmentNode(Node* identifier, Node* expression, int line) {
    AssignmentNode* n = (AssignmentNode*)malloc(sizeof(AssignmentNode));
    if (!n) { fprintf(stderr, "malloc failed\n"); exit(1); }
    n->type = NODE_ASSIGNMENT;
    n->next = nullptr;
    n->line = line;
    ((Node*)n)->calculatedType = SYM_TYPE_UNCHECKED;
    n->identifier = (IdentifierNode*)identifier;
    n->expression = expression;
    return (Node*)n;
}

Node* createVarDeclarationNode(Node* typeNode, Node* idNode, Node* initVal, int line) {
     VarDeclarationNode* n = (VarDeclarationNode*)malloc(sizeof(VarDeclarationNode));
     if (!n) { fprintf(stderr, "malloc failed\n"); exit(1); }
     n->type = NODE_VAR_DECLARATION;
     n->next = nullptr;
     n->line = line;
     ((Node*)n)->calculatedType = SYM_TYPE_UNCHECKED;
     n->varTypeNode = typeNode;
     n->identifier = (IdentifierNode*)idNode;
     n->initialValue = initVal;
     return (Node*)n;
}

Node* createStatementListNode(Node* statement, int line) {
    StatementListNode* n = (StatementListNode*)malloc(sizeof(StatementListNode));
     if (!n) { fprintf(stderr, "malloc failed\n"); exit(1); }
    n->type = NODE_STATEMENT_LIST;
    n->next = nullptr;
    n->line = line;
    ((Node*)n)->calculatedType = SYM_TYPE_UNCHECKED;
    n->statements = new std::vector<Node*>();
    if(statement) n->statements->push_back(statement);
    return (Node*)n;
}

Node* addStatementToList(Node* listNode, Node* statement) {
     if (!listNode) { return createStatementListNode(statement, statement ? statement->line : 0); }
     if (((Node*)listNode)->type != NODE_STATEMENT_LIST) {
         Node* newNode = createStatementListNode(listNode, listNode ? listNode->line : 0);
         if (statement) { ((StatementListNode*)newNode)->statements->push_back(statement); }
         return newNode;
     }
     StatementListNode* list = (StatementListNode*)listNode;
     if (statement) { list->statements->push_back(statement); }
     return listNode;
 }

Node* createFunctionSignatureNode(Node* typeNode, Node* nameNode, Node* params, int line) {
    FunctionSignatureNode* n = (FunctionSignatureNode*) malloc(sizeof(FunctionSignatureNode));
     if (!n) { fprintf(stderr, "malloc failed\n"); exit(1); }
    n->type = NODE_FUNC_SIG;
    n->next = nullptr;
    n->line = line;
    ((Node*)n)->calculatedType = SYM_TYPE_UNCHECKED;
    n->returnType = typeNode;
    n->name = (IdentifierNode*)nameNode;
    n->parameters = params;
    return (Node*)n;
}

Node* createFunctionDefNode(Node* signatureNode, Node* bodyNode, int line) {
    FunctionDefNode* n = (FunctionDefNode*)malloc(sizeof(FunctionDefNode));
    if (!n) { fprintf(stderr, "malloc failed\n"); exit(1); }
    n->type = NODE_FUNCTION_DEF;
    n->next = nullptr;
    n->line = line;
    ((Node*)n)->calculatedType = SYM_TYPE_UNCHECKED;
    n->signature = signatureNode;
    n->body = bodyNode;
    return (Node*)n;
}

Node* createForNode(Node* init, Node* cond, Node* incr, Node* body, int line) {
    ForNode* n = (ForNode*)malloc(sizeof(ForNode));
     if (!n) { fprintf(stderr, "malloc failed\n"); exit(1); }
    n->type = NODE_FOR;
    n->next = nullptr;
    n->line = line;
    ((Node*)n)->calculatedType = SYM_TYPE_UNCHECKED;
    n->initialization = init;
    n->condition = cond;
    n->increment = incr;
    n->body = body;
    return (Node*)n;
}

Node* createIfNode(Node* cond, Node* thenB, Node* elseB, int line) {
    IfNode* n = (IfNode*)malloc(sizeof(IfNode));
    if (!n) { fprintf(stderr, "malloc failed\n"); exit(1); }
    n->type = NODE_IF;
    n->next = nullptr;
    n->line = line;
    ((Node*)n)->calculatedType = SYM_TYPE_UNCHECKED;
    n->condition = cond;
    n->thenBranch = thenB;
    n->elseBranch = elseB;
    return (Node*)n;
}

Node* createWhileNode(Node* cond, Node* body, int line) {
     WhileNode* n = (WhileNode*)malloc(sizeof(WhileNode));
     if (!n) { fprintf(stderr, "malloc failed\n"); exit(1); }
     n->type = NODE_WHILE;
     n->next = nullptr;
     n->line = line;
     ((Node*)n)->calculatedType = SYM_TYPE_UNCHECKED;
     n->condition = cond;
     n->body = body;
     return (Node*)n;
}

Node* createPrintNode(Node* expr, int line) {
     PrintNode* n = (PrintNode*)malloc(sizeof(PrintNode));
     if (!n) { fprintf(stderr, "malloc failed\n"); exit(1); }
     n->type = NODE_PRINT;
     n->next = nullptr;
     n->line = line;
     ((Node*)n)->calculatedType = SYM_TYPE_UNCHECKED;
     n->expression = expr;
     return (Node*)n;
}

Node* createReadNode(Node* idNode, int line) {
     ReadNode* n = (ReadNode*)malloc(sizeof(ReadNode));
     if (!n) { fprintf(stderr, "malloc failed\n"); exit(1); }
     n->type = NODE_READ;
     n->next = nullptr;
     n->line = line;
     ((Node*)n)->calculatedType = SYM_TYPE_UNCHECKED;
     n->identifier = (IdentifierNode*)idNode;
     return (Node*)n;
}

Node* createReturnNode(Node* retVal, int line) {
     ReturnNode* n = (ReturnNode*)malloc(sizeof(ReturnNode));
     if (!n) { fprintf(stderr, "malloc failed\n"); exit(1); }
     n->type = NODE_RETURN;
     n->next = nullptr;
     n->line = line;
     ((Node*)n)->calculatedType = SYM_TYPE_UNCHECKED;
     n->returnValue = retVal;
     return (Node*)n;
}

Node* createFunctionCallNode(Node* nameNode, Node* args, int line) {
     FunctionCallNode* n = (FunctionCallNode*)malloc(sizeof(FunctionCallNode));
     if (!n) { fprintf(stderr, "malloc failed\n"); exit(1); }
     n->type = NODE_FUNCTION_CALL;
     n->next = nullptr;
     n->line = line;
     ((Node*)n)->calculatedType = SYM_TYPE_UNCHECKED;
     n->name = (IdentifierNode*)nameNode;
     n->arguments = args;
     return (Node*)n;
}

Node* createBreakNode(int line) {
    Node* n = (Node*)malloc(sizeof(Node));
    if (!n) { fprintf(stderr, "malloc failed\n"); exit(1); }
    n->type = NODE_BREAK;
    n->next = nullptr;
    n->line = line;
    n->calculatedType = SYM_TYPE_VOID;
    return n;
}

Node* createContinueNode(int line) {
    Node* n = (Node*)malloc(sizeof(Node));
    if (!n) { fprintf(stderr, "malloc failed\n"); exit(1); }
    n->type = NODE_CONTINUE;
    n->next = nullptr;
    n->line = line;
    n->calculatedType = SYM_TYPE_VOID;
    return n;
}

Node* createParamList(Node* param) { 
    if (param) { 
        param->next = nullptr; 
    }
    return param; 
}
Node* addParamToList(Node* listNode, Node* param) { 
    if (!listNode) return param; 
    Node* current = listNode; 
    while (current->next != nullptr) { current = current->next;} 
    current->next = param; 
    param->next = nullptr;
    return listNode; 
}
Node* createArgList(Node* arg) { return arg; }

Node* addArgToList(Node* listNode, Node* arg) {
     if (!listNode) return arg; 
     Node* current = listNode; 
     while (current->next) { 
        current = current->next; 
    } 
    current->next = arg; 
    return listNode; 
}

void freeAst(Node* node) {
    // fprintf(stderr, "[DEBUG] Liberando nodo de tipo: %d en línea %d\n", node ? node->type : -1, node ? node->line : -1);
    
    if (!node) {
        return;
    }

    switch (node->type) {
        case NODE_UNARY_OP:
            freeAst(((UnaryOpNode*)node)->operand);
            break;
        case NODE_BINARY_OP:
            freeAst(((BinaryOpNode*)node)->left);
            freeAst(((BinaryOpNode*)node)->right);
            break;
        case NODE_ASSIGNMENT:
            freeAst((Node*)((AssignmentNode*)node)->identifier);
            freeAst(((AssignmentNode*)node)->expression);
            break;
        case NODE_VAR_DECLARATION:
            freeAst(((VarDeclarationNode*)node)->varTypeNode);
            freeAst((Node*)((VarDeclarationNode*)node)->identifier);
            freeAst(((VarDeclarationNode*)node)->initialValue);
            break;
        case NODE_FUNCTION_DEF:
            freeAst(((FunctionDefNode*)node)->signature);
            freeAst(((FunctionDefNode*)node)->body);
            break;
        case NODE_IF:
            freeAst(((IfNode*)node)->condition);
            freeAst(((IfNode*)node)->thenBranch);
            freeAst(((IfNode*)node)->elseBranch);
            break;
        case NODE_WHILE:
            freeAst(((WhileNode*)node)->condition);
            freeAst(((WhileNode*)node)->body);
            break;
        case NODE_FOR:
            freeAst(((ForNode*)node)->initialization);
            freeAst(((ForNode*)node)->condition);
            freeAst(((ForNode*)node)->increment);
            freeAst(((ForNode*)node)->body);
            break;
        case NODE_PRINT:
            freeAst(((PrintNode*)node)->expression);
            break;
        case NODE_READ:
            freeAst((Node*)((ReadNode*)node)->identifier);
            break;
        case NODE_RETURN:
            freeAst(((ReturnNode*)node)->returnValue);
            break;
        case NODE_FUNC_SIG:
            freeAst(((FunctionSignatureNode*)node)->returnType);
            freeAst((Node*)((FunctionSignatureNode*)node)->name);
            freeAst(((FunctionSignatureNode*)node)->parameters);
            break;
        case NODE_FUNCTION_CALL:
            freeAst((Node*)((FunctionCallNode*)node)->name);
            freeAst(((FunctionCallNode*)node)->arguments); 
            break;
        case NODE_STATEMENT_LIST: {
            StatementListNode* list = (StatementListNode*)node;
            if (list->statements) {
                for (Node* stmt : *list->statements) {
                    freeAst(stmt);
                }
                delete list->statements; 
            }
            break;
        }
        case NODE_IDENTIFIER:
        case NODE_TYPE:
            if (((IdentifierNode*)node)->sval) {
                free(((IdentifierNode*)node)->sval);
            }
            break;
        case NODE_CONSTANT_STRING:
            if (((StringLiteralNode*)node)->sval) {
                free(((StringLiteralNode*)node)->sval);
            }
            break;
        // Los casos base no tienen hijos ni memoria dinámica propia.
        case NODE_CONSTANT_INT:
        case NODE_CONSTANT_FLOAT:
        case NODE_CONSTANT_BOOL:
        case NODE_BREAK:
        case NODE_CONTINUE:
        default:
            break;
    }

    // Liberar la lista de nodos hermanos (usado para parámetros y argumentos)
    if (node->next) {
        freeAst(node->next);
    }

    // Finalmente, liberar el nodo actual
    free(node);
}

void printAst(Node* node, int indent) {
    if (!node) {
        std::cout << std::string(indent * 2, ' ') << "(null)" << std::endl;
        return;
    }

    std::cout << std::string(indent * 2, ' ');

    std::cout << "[" << node->type << ":L" << node->line << "] ";

    std::string CalcTypeStr = "";
    if (node->calculatedType != SYM_TYPE_UNCHECKED) { 
        CalcTypeStr = " [TipoAST: " + symbolBasicTypeToString(node->calculatedType) + "]";
    }


    switch (node->type) {
        case NODE_CONSTANT_INT:
            std::cout << "INT_CONST(" << ((ConstantIntNode*)node)->value << ")" << CalcTypeStr << std::endl;
            break;
        case NODE_CONSTANT_FLOAT:
            std::cout << "FLOAT_CONST(" << ((ConstantFloatNode*)node)->value << ")" << CalcTypeStr << std::endl;
            break;
        case NODE_CONSTANT_BOOL:
            std::cout << "BOOL_CONST(" << (((ConstantBoolNode*)node)->value ? "true" : "false") << ")" << CalcTypeStr << std::endl;
            break;
        case NODE_CONSTANT_STRING:
            std::cout << "STR_CONST(\"" << ((StringLiteralNode*)node)->sval << "\")" << CalcTypeStr << std::endl;
            break;
        case NODE_IDENTIFIER:
            std::cout << "ID(\"" << ((IdentifierNode*)node)->sval << "\")" << CalcTypeStr << std::endl;
            break;
        case NODE_TYPE: 
            std::cout << "TYPE(" << ((IdentifierNode*)node)->sval << ")" << std::endl;
            break;
        case NODE_UNARY_OP:
            
            std::cout << "UNARY_OP(" << ((UnaryOpNode*)node)->op << ")" << CalcTypeStr << std::endl;
            printAst(((UnaryOpNode*)node)->operand, indent + 1);
            break;
        case NODE_BINARY_OP:
            std::cout << "BINARY_OP(" << ((BinaryOpNode*)node)->op << ")" << CalcTypeStr << std::endl;
            printAst(((BinaryOpNode*)node)->left, indent + 1);
            printAst(((BinaryOpNode*)node)->right, indent + 1);
            break;
        case NODE_ASSIGNMENT: 
            std::cout << "ASSIGN:" << CalcTypeStr << std::endl;
            printAst((Node*)((AssignmentNode*)node)->identifier, indent + 1); 
            printAst(((AssignmentNode*)node)->expression, indent + 1);
            break;
        case NODE_FUNCTION_CALL:
            std::cout << "CALL:" << CalcTypeStr << std::endl;
            { 
                FunctionCallNode* call = (FunctionCallNode*)node;
                printAst((Node*)call->name, indent + 1); 
                std::cout << std::string((indent + 1) * 2, ' ') << "ARGS:" << std::endl;
                Node* current_arg = call->arguments;
                if (!current_arg) {
                    std::cout << std::string((indent + 2) * 2, ' ') << "(no args)" << std::endl;
                }
                while (current_arg) {
                    printAst(current_arg, indent + 2); 
                    current_arg = current_arg->next;
                }
            }
            break;
        case NODE_VAR_DECLARATION:
            std::cout << "VAR_DECL:" << std::endl;
            printAst(((VarDeclarationNode*)node)->varTypeNode, indent + 1);
            printAst((Node*)((VarDeclarationNode*)node)->identifier, indent + 1); 
            if (((VarDeclarationNode*)node)->initialValue) {
                std::cout << std::string((indent + 1) * 2, ' ') << "INIT:" << std::endl;
                printAst(((VarDeclarationNode*)node)->initialValue, indent + 2); 
            }
            break;
        case NODE_STATEMENT_LIST: {
            std::cout << "STMT_LIST:" << std::endl;
            StatementListNode* list = (StatementListNode*)node;
            if (list->statements && !list->statements->empty()) {
                for (Node* stmt : *list->statements) {
                    printAst(stmt, indent + 1);
                }
            } else {
                std::cout << std::string((indent + 1) * 2, ' ') << "(empty)" << std::endl;
            }
            break;
        }
        case NODE_FUNC_SIG: {
            std::cout << "FUNC_SIG:" << std::endl;
            FunctionSignatureNode* sig = (FunctionSignatureNode*)node;
            std::cout << std::string((indent + 1) * 2, ' ') << "RET_TYPE:" << std::endl;
            printAst(sig->returnType, indent + 2);
            std::cout << std::string((indent + 1) * 2, ' ') << "NAME:" << std::endl;
            printAst((Node*)sig->name, indent + 2);
            std::cout << std::string((indent + 1) * 2, ' ') << "PARAMS:" << std::endl;
            Node* current_param = sig->parameters;
            if (!current_param) {
                std::cout << std::string((indent + 2) * 2, ' ') << "(no params)" << std::endl;
            }
            while (current_param) { 
                printAst(current_param, indent + 2);
                current_param = current_param->next;
            }
             break;
        }
        case NODE_FUNCTION_DEF:
            std::cout << "FUNC_DEF:" << std::endl;
            printAst(((FunctionDefNode*)node)->signature, indent + 1);
            std::cout << std::string((indent + 1) * 2, ' ') << "BODY:" << std::endl;
            printAst(((FunctionDefNode*)node)->body, indent + 2); 
            break;
        case NODE_FOR:
            std::cout << "FOR:" << std::endl;
            std::cout << std::string((indent + 1) * 2, ' ') << "INIT:" << std::endl;
            printAst(((ForNode*)node)->initialization, indent + 2);
            std::cout << std::string((indent + 1) * 2, ' ') << "COND:" << std::endl;
            printAst(((ForNode*)node)->condition, indent + 2); 
            std::cout << std::string((indent + 1) * 2, ' ') << "INCR:" << std::endl;
            printAst(((ForNode*)node)->increment, indent + 2);
            std::cout << std::string((indent + 1) * 2, ' ') << "BODY:" << std::endl;
            printAst(((ForNode*)node)->body, indent + 2);
            break;
        case NODE_IF:
            std::cout << "IF:" << std::endl;
            std::cout << std::string((indent + 1) * 2, ' ') << "COND:" << std::endl;
            printAst(((IfNode*)node)->condition, indent + 2); 
            std::cout << std::string((indent + 1) * 2, ' ') << "THEN:" << std::endl;
            printAst(((IfNode*)node)->thenBranch, indent + 2);
            if (((IfNode*)node)->elseBranch) {
                std::cout << std::string((indent + 1) * 2, ' ') << "ELSE:" << std::endl;
                printAst(((IfNode*)node)->elseBranch, indent + 2);
            }
            break;
        case NODE_WHILE:
            std::cout << "WHILE:" << std::endl;
            std::cout << std::string((indent + 1) * 2, ' ') << "COND:" << std::endl;
            printAst(((WhileNode*)node)->condition, indent + 2); 
            std::cout << std::string((indent + 1) * 2, ' ') << "BODY:" << std::endl;
            printAst(((WhileNode*)node)->body, indent + 2);
            break;
        case NODE_PRINT:
            std::cout << "PRINT:" << std::endl;
            printAst(((PrintNode*)node)->expression, indent + 1); 
            break;
        case NODE_READ:
            std::cout << "READ:" << std::endl;
            printAst((Node*)((ReadNode*)node)->identifier, indent + 1);
            break;
        case NODE_RETURN:
            std::cout << "RETURN:" << std::endl;
            printAst(((ReturnNode*)node)->returnValue, indent + 1);
            break;
        case NODE_BREAK:
            std::cout << "BREAK_STMT" << std::endl;
            break;
        case NODE_CONTINUE:
            std::cout << "CONTINUE_STMT" << std::endl;
            break;
        default:
            std::cout << "Nodo Desconocido (" << node->type << ")" << std::endl;
            break;
    }
}
Node* duplicateTypeNode(Node* original) {
    if (!original || original->type != NODE_TYPE) {
        return nullptr;
    }
    IdentifierNode* original_id = (IdentifierNode*)original;

    Node* new_node = createIdentifierNode(strdup(original_id->sval), original->line);

    new_node->type = NODE_TYPE;
    
    return new_node;
}
//Implementacion de todas las funciones que estan en Ast.h (FINAL)