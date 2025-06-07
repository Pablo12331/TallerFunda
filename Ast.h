#ifndef AST_H
#define AST_H

#include <vector>
#include <string>
#include "common_types.h"

typedef struct Node Node;

struct Node {
    NodeType type;
    Node *next;
    int line;
    SymbolBasicType tipoCalculado;
};

typedef struct {
    NodeType type; 
    Node *next; 
    int line; 
    SymbolBasicType tipoCalculado; // Corregido
    int op;
    Node *operand;
} UnaryOpNode;

typedef struct {
    NodeType type;
    Node *next; 
    int line; 
    SymbolBasicType tipoCalculado; 
    int value;
} ConstantIntNode;

typedef struct {
    NodeType type; 
    Node *next; 
    int line; 
    SymbolBasicType tipoCalculado; // Corregido
    double value;
} ConstantFloatNode;

typedef struct {
    NodeType type; 
    Node *next; 
    int line; 
    SymbolBasicType tipoCalculado; // Corregido
    int value; // 0 para false, 1 para true
} ConstantBoolNode;

typedef struct {
    NodeType type; 
    Node *next; 
    int line; 
    SymbolBasicType tipoCalculado; // Corregido
    char *sval;
} StringLiteralNode;

typedef struct {
    NodeType type; 
    Node *next; 
    int line; 
    SymbolBasicType tipoCalculado; // Corregido
    char *sval;
} IdentifierNode;

typedef struct {
    NodeType type;
    Node *next; 
    int line; 
    SymbolBasicType tipoCalculado; // Corregido
    int op;
    Node *left;
    Node *right;
} BinaryOpNode;

typedef struct {
    NodeType type; 
    Node *next; 
    int line; 
    SymbolBasicType tipoCalculado; // Corregido
    IdentifierNode *identifier;
    Node *expression;
} AssignmentNode;

typedef struct {
    NodeType type; 
    Node *next; 
    int line; 
    SymbolBasicType tipoCalculado; // Corregido
    Node *varTypeNode;
    IdentifierNode *identifier;
    Node *initialValue;
} VarDeclarationNode;

typedef struct {
    NodeType type; 
    Node* next; 
    int line; 
    SymbolBasicType tipoCalculado; // Corregido
    Node* condition;
    Node* thenBranch;
    Node* elseBranch;
} IfNode;

typedef struct {
    NodeType type; 
    Node* next; 
    int line; 
    SymbolBasicType tipoCalculado; // Corregido
    Node* condition;
    Node* body;
} WhileNode;

typedef struct {
    NodeType type; 
    Node* next; 
    int line; 
    SymbolBasicType tipoCalculado; // Corregido
    Node* initialization;
    Node* condition;
    Node* increment;
    Node* body;
} ForNode;

typedef struct {
    NodeType type; 
    Node* next; 
    int line; 
    SymbolBasicType tipoCalculado; // Corregido
    Node* expression;
} PrintNode;

typedef struct {
    NodeType type; 
    Node* next; 
    int line; 
    SymbolBasicType tipoCalculado; // Corregido
    IdentifierNode* identifier;
} ReadNode;

typedef struct {
    NodeType type; 
    Node* next; 
    int line; 
    SymbolBasicType tipoCalculado; // Corregido
    Node* returnType;
    IdentifierNode* name;
    Node* parameters;
} FunctionSignatureNode;

typedef struct {
    NodeType type; 
    Node* next; 
    int line; 
    SymbolBasicType tipoCalculado; // Corregido
    Node* signature;
    Node* body;
} FunctionDefNode;

typedef struct {
    NodeType type; 
    Node* next; 
    int line; 
    SymbolBasicType tipoCalculado; // Corregido
    IdentifierNode* name;
    Node* arguments;
} FunctionCallNode;

typedef struct {
    NodeType type; 
    Node* next; 
    int line; 
    SymbolBasicType tipoCalculado; // Corregido
    Node* returnValue;
} ReturnNode;

typedef struct {
    NodeType type; 
    Node *next; 
    int line; 
    SymbolBasicType tipoCalculado; // Corregido
    std::vector<Node*>* statements;
} StatementListNode;

Node* createConstantIntNode(int value, int line);
Node* createConstantFloatNode(double value, int line);
Node* createConstantBoolNode(int value, int line);
Node* createStringLiteralNode(char* sval, int line);
Node* createIdentifierNode(char* sval, int line);
Node* createUnaryOpNode(int op, Node* operand, int line);
Node* createBinaryOpNode(int op, Node* left, Node* right, int line);
Node* createAssignmentNode(Node* identifier, Node* expression, int line);
Node* createStatementListNode(Node* statement, int line);
Node* addStatementToList(Node* listNode, Node* statement);
Node* createFunctionDefNode(Node* signature, Node* body, int line);
Node* createVarDeclarationNode(Node* typeNode, Node* idNode, Node* initVal, int line);
Node* createFunctionSignatureNode(Node* typeNode, Node* nameNode, Node* params, int line);
Node* createForNode(Node* init, Node* cond, Node* incr, Node* body, int line);
Node* createIfNode(Node* cond, Node* thenB, Node* elseB, int line);
Node* createWhileNode(Node* cond, Node* body, int line);
Node* createPrintNode(Node* expr, int line);
Node* createReadNode(Node* idNode, int line);
Node* createReturnNode(Node* retVal, int line);
Node* createFunctionCallNode(Node* nameNode, Node* args, int line);
Node* createParamList(Node* param);
Node* addParamToList(Node* listNode, Node* param);
Node* createArgList(Node* arg);
Node* addArgToList(Node* listNode, Node* arg);

void freeAst(Node* node);
void printAst(Node* node, int indent = 0);  

#endif