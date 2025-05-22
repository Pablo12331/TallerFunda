#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <iostream>
#include "Ast.h"
#include "parser.tab.h"

Node* createConstantIntNode(int value, int line) {
    ConstantIntNode* n = (ConstantIntNode*)malloc(sizeof(ConstantIntNode));
    if (!n) { fprintf(stderr, "malloc failed\n"); exit(1); }
    n->type = NODE_CONSTANT_INT;
    n->next = nullptr;
    n->line = line;
    n->value = value;
    return (Node*)n;
}

Node* createConstantFloatNode(double value, int line) {
    ConstantFloatNode* n = (ConstantFloatNode*)malloc(sizeof(ConstantFloatNode));
    if (!n) { fprintf(stderr, "malloc failed\n"); exit(1); }
    n->type = NODE_CONSTANT_FLOAT;
    n->next = nullptr;
    n->line = line;
    n->value = value;
    return (Node*)n;
}

Node* createConstantBoolNode(int value, int line) {
    ConstantBoolNode* n = (ConstantBoolNode*)malloc(sizeof(ConstantBoolNode));
    if (!n) { fprintf(stderr, "malloc failed\n"); exit(1); }
    n->type = NODE_CONSTANT_BOOL;
    n->next = nullptr;
    n->line = line;
    n->value = value;
    return (Node*)n;
}

Node* createStringLiteralNode(char* sval, int line) {   
    StringLiteralNode* n = (StringLiteralNode*)malloc(sizeof(StringLiteralNode));
    if (!n) { fprintf(stderr, "malloc failed\n"); exit(1); }
    n->type = NODE_CONSTANT_STRING;
    n->next = nullptr;
    n->line = line;
    n->sval = sval;
    return (Node*)n;
}

Node* createIdentifierNode(char* sval, int line) {
    IdentifierNode* n = (IdentifierNode*)malloc(sizeof(IdentifierNode));
    if (!n) { fprintf(stderr, "malloc failed\n"); exit(1); }
    n->type = NODE_IDENTIFIER;
    n->next = nullptr;
    n->line = line;
    n->sval = sval;
    return (Node*)n;
}

Node* createUnaryOpNode(int op, Node* operand, int line) { 
    UnaryOpNode* n = (UnaryOpNode*)malloc(sizeof(UnaryOpNode));
    if (!n) { fprintf(stderr, "malloc failed\n"); exit(1); }
    n->type = NODE_UNARY_OP;
    n->next = nullptr;
    n->line = line;
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
     n->expression = expr;
     return (Node*)n;
}

Node* createReadNode(Node* idNode, int line) {
     ReadNode* n = (ReadNode*)malloc(sizeof(ReadNode));
     if (!n) { fprintf(stderr, "malloc failed\n"); exit(1); }
     n->type = NODE_READ;
     n->next = nullptr;
     n->line = line;
     n->identifier = (IdentifierNode*)idNode;
     return (Node*)n;
}

Node* createReturnNode(Node* retVal, int line) {
     ReturnNode* n = (ReturnNode*)malloc(sizeof(ReturnNode));
     if (!n) { fprintf(stderr, "malloc failed\n"); exit(1); }
     n->type = NODE_RETURN;
     n->next = nullptr;
     n->line = line;
     n->returnValue = retVal;
     return (Node*)n;
}

Node* createFunctionCallNode(Node* nameNode, Node* args, int line) {
     FunctionCallNode* n = (FunctionCallNode*)malloc(sizeof(FunctionCallNode));
     if (!n) { fprintf(stderr, "malloc failed\n"); exit(1); }
     n->type = NODE_FUNCTION_CALL;
     n->next = nullptr;
     n->line = line;
     n->name = (IdentifierNode*)nameNode;
     n->arguments = args;
     return (Node*)n;
}

Node* createParamList(Node* param) { return param; }
Node* addParamToList(Node* listNode, Node* param) { if (!listNode) return param; Node* current = listNode; while (current->next) { current = current->next; } current->next = param; return listNode; }
Node* createArgList(Node* arg) { return arg; }
Node* addArgToList(Node* listNode, Node* arg) { if (!listNode) return arg; Node* current = listNode; while (current->next) { current = current->next; } current->next = arg; return listNode; }


void freeAst(Node* node) {
    if (!node) return;

    switch (node->type) {
        case NODE_BINARY_OP: freeAst(((BinaryOpNode*)node)->left); freeAst(((BinaryOpNode*)node)->right); break;
        case NODE_UNARY_OP: freeAst(((UnaryOpNode*)node)->operand); break;
        case NODE_ASSIGNMENT: freeAst((Node*)((AssignmentNode*)node)->identifier); freeAst(((AssignmentNode*)node)->expression); break;
        case NODE_IDENTIFIER: if(((IdentifierNode*)node)->sval) free(((IdentifierNode*)node)->sval); break;
        case NODE_TYPE: if(((IdentifierNode*)node)->sval) free(((IdentifierNode*)node)->sval); break;
        case NODE_VAR_DECLARATION: freeAst(((VarDeclarationNode*)node)->varTypeNode); freeAst((Node*)((VarDeclarationNode*)node)->identifier); freeAst(((VarDeclarationNode*)node)->initialValue); break;
        case NODE_FUNC_SIG: freeAst(((FunctionSignatureNode*)node)->returnType); freeAst((Node*)((FunctionSignatureNode*)node)->name); freeAst(((FunctionSignatureNode*)node)->parameters); break;
        case NODE_FUNCTION_DEF: freeAst(((FunctionDefNode*)node)->signature); freeAst(((FunctionDefNode*)node)->body); break;
        case NODE_FOR: freeAst(((ForNode*)node)->initialization); freeAst(((ForNode*)node)->condition); freeAst(((ForNode*)node)->increment); freeAst(((ForNode*)node)->body); break;
        case NODE_IF: freeAst(((IfNode*)node)->condition); freeAst(((IfNode*)node)->thenBranch); freeAst(((IfNode*)node)->elseBranch); break;
        case NODE_WHILE: freeAst(((WhileNode*)node)->condition); freeAst(((WhileNode*)node)->body); break;
        case NODE_PRINT: freeAst(((PrintNode*)node)->expression); break;
        case NODE_READ: freeAst((Node*)((ReadNode*)node)->identifier); break;
        case NODE_RETURN: freeAst(((ReturnNode*)node)->returnValue); break;
        case NODE_FUNCTION_CALL: freeAst((Node*)((FunctionCallNode*)node)->name); freeAst(((FunctionCallNode*)node)->arguments); break;
        case NODE_STATEMENT_LIST: { StatementListNode* list = (StatementListNode*)node; if (list->statements) { for (Node* stmt : *list->statements) { freeAst(stmt); } delete list->statements;} } break;
        case NODE_CONSTANT_INT: break;
        case NODE_CONSTANT_FLOAT: break;
        case NODE_CONSTANT_BOOL: break;
        case NODE_CONSTANT_STRING: if(((StringLiteralNode*)node)->sval) free(((StringLiteralNode*)node)->sval); break;
        default: break;
    }

    
    // Libera la cadena enlazada por 'next' DESPUÉS de liberar los hijos específicos
    freeAst(node->next);

    // Libera el nodo actual
    free(node);
}

void printAst(Node* node, int indent) {
    if (!node) {
        // std::cout << std::string(indent * 2, ' ') << "(null)" << std::endl; // Opcional: mostrar nulos
        return;
    }

    // Imprimir indentación
    std::cout << std::string(indent * 2, ' '); // 2 espacios por nivel

    // Imprimir información básica del nodo
    printf("[%d:L%d] ", node->type, node->line); // Imprime tipo enum y línea

    // Imprimir detalles según el tipo de nodo
    switch (node->type) {
        case NODE_CONSTANT_INT:
            printf("INT_CONST(%d)\n", ((ConstantIntNode*)node)->value);
            break;
        case NODE_CONSTANT_FLOAT:
            printf("FLOAT_CONST(%f)\n", ((ConstantFloatNode*)node)->value);
            break;
        case NODE_CONSTANT_BOOL:
            printf("BOOL_CONST(%s)\n", ((ConstantBoolNode*)node)->value ? "true" : "false");
            break;
        case NODE_CONSTANT_STRING:
            printf("STR_CONST(\"%s\")\n", ((StringLiteralNode*)node)->sval);
            break;
        case NODE_IDENTIFIER:
            printf("ID(\"%s\")\n", ((IdentifierNode*)node)->sval);
            break;
        case NODE_TYPE:
            printf("TYPE(\"%s\")\n", ((IdentifierNode*)node)->sval);
            break;
        case NODE_UNARY_OP:
            printf("UNARY_OP(%d)\n", ((UnaryOpNode*)node)->op);
            printAst(((UnaryOpNode*)node)->operand, indent + 1);
            break;
        case NODE_BINARY_OP:
            printf("BINARY_OP(%d)\n", ((BinaryOpNode*)node)->op); 
            printAst(((BinaryOpNode*)node)->left, indent + 1);
            printAst(((BinaryOpNode*)node)->right, indent + 1);
            break;
        case NODE_ASSIGNMENT:
            printf("ASSIGN:\n");
            printAst((Node*)((AssignmentNode*)node)->identifier, indent + 1);
            printAst(((AssignmentNode*)node)->expression, indent + 1);
            break;
         case NODE_VAR_DECLARATION:
             printf("VAR_DECL:\n");
             printAst(((VarDeclarationNode*)node)->varTypeNode, indent + 1);
             printAst((Node*)((VarDeclarationNode*)node)->identifier, indent + 1);
             if (((VarDeclarationNode*)node)->initialValue) {
                 std::cout << std::string((indent + 1) * 2, ' ') << "INIT:\n";
                 printAst(((VarDeclarationNode*)node)->initialValue, indent + 1);
             }
             break;
        case NODE_STATEMENT_LIST: {
            printf("STMT_LIST:\n");
            StatementListNode* list = (StatementListNode*)node;
            if (list->statements) {
                for (Node* stmt : *list->statements) {
                    printAst(stmt, indent + 1);
                }
            }
            break;
        }
        case NODE_FUNC_SIG: {
             printf("FUNC_SIG:\n");
             FunctionSignatureNode* sig = (FunctionSignatureNode*)node;
             std::cout << std::string((indent + 1) * 2, ' ') << "RET_TYPE:\n";
             printAst(sig->returnType, indent + 2);
             std::cout << std::string((indent + 1) * 2, ' ') << "NAME:\n";
             printAst((Node*)sig->name, indent + 2);
             std::cout << std::string((indent + 1) * 2, ' ') << "PARAMS:\n";
             printAst(sig->parameters, indent + 2); // Imprime lista de parámetros
             break;
        }
        case NODE_FUNCTION_DEF:
             printf("FUNC_DEF:\n");
             printAst(((FunctionDefNode*)node)->signature, indent + 1);
             std::cout << std::string((indent + 1) * 2, ' ') << "BODY:\n";
             printAst(((FunctionDefNode*)node)->body, indent + 1);
             break;
         case NODE_FOR:
             printf("FOR:\n");
             std::cout << std::string((indent + 1) * 2, ' ') << "INIT:\n";
             printAst(((ForNode*)node)->initialization, indent + 2);
             std::cout << std::string((indent + 1) * 2, ' ') << "COND:\n";
             printAst(((ForNode*)node)->condition, indent + 2);
             std::cout << std::string((indent + 1) * 2, ' ') << "INCR:\n";
             printAst(((ForNode*)node)->increment, indent + 2);
             std::cout << std::string((indent + 1) * 2, ' ') << "BODY:\n";
             printAst(((ForNode*)node)->body, indent + 2);
             break;
         case NODE_IF:
             printf("IF:\n");
             std::cout << std::string((indent + 1) * 2, ' ') << "COND:\n";
             printAst(((IfNode*)node)->condition, indent + 2);
             std::cout << std::string((indent + 1) * 2, ' ') << "THEN:\n";
             printAst(((IfNode*)node)->thenBranch, indent + 2);
             if (((IfNode*)node)->elseBranch) {
                 std::cout << std::string((indent + 1) * 2, ' ') << "ELSE:\n";
                 printAst(((IfNode*)node)->elseBranch, indent + 2);
             }
             break;
         case NODE_WHILE:
             printf("WHILE:\n");
             std::cout << std::string((indent + 1) * 2, ' ') << "COND:\n";
             printAst(((WhileNode*)node)->condition, indent + 2);
             std::cout << std::string((indent + 1) * 2, ' ') << "BODY:\n";
             printAst(((WhileNode*)node)->body, indent + 2);
             break;
         case NODE_PRINT:
             printf("PRINT:\n");
             printAst(((PrintNode*)node)->expression, indent + 1);
             break;
         case NODE_READ:
             printf("READ:\n");
             printAst((Node*)((ReadNode*)node)->identifier, indent + 1);
             break;
         case NODE_RETURN:
             printf("RETURN:\n");
             printAst(((ReturnNode*)node)->returnValue, indent + 1);
             break;
         case NODE_FUNCTION_CALL:
              printf("CALL:\n");
              printAst((Node*)((FunctionCallNode*)node)->name, indent + 1);
              std::cout << std::string((indent + 1) * 2, ' ') << "ARGS:\n";
              printAst(((FunctionCallNode*)node)->arguments, indent + 2); // Imprime lista args
              break;

        default:
            printf("Nodo Desconocido (%d)\n", node->type);
            break;
    }

    // Imprimir el siguiente nodo en listas enlazadas (params/args)
    // OJO: Esto puede imprimir dos veces si el nodo padre ya recorrió la lista
    // Es mejor manejar el recorrido de ->next DENTRO del caso del nodo padre (FUNC_SIG, FUNC_CALL)
    // printAst(node->next, indent); // <-- COMENTAR O QUITAR si causa doble impresión
}