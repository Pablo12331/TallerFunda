#ifndef AST_H
#define AST_H

#include <vector>
#include <string>
#include "common_types.h"


//Definicion de las estructuras de cada nodo de cuales se va a componer el AST (INICIO)

typedef struct Node Node; 

//Excepciones: BreakNode y ContinueNode son nodos sin parametros especiales, por ende ocupan la estructa principal para crearse 

struct Node { //Estructura generica y fundamental(Todos los nodos especificos comparten los campos de este)
    NodeType type; //Guarda la etiqueta del nodo
    Node *next; //Puntero hacia el nodo siguiente
    int line; //Campo que guarda el numero de linea donde se encuentra (Util para reportar errores)
    SymbolBasicType calculatedType; //Guarda el tipo resultante de una expresion
};

typedef struct { //OPERACION UNITARIA (EJEM: -5 o !verdadero)
    NodeType type; 
    Node *next; 
    int line; 
    SymbolBasicType calculatedType;
    int op; //Guarda el token del operador
    Node *operand; //Puntero al nodo de la expresion sobre la que actua
} UnaryOpNode;

typedef struct { // INT
    NodeType type;
    Node *next; 
    int line; 
    SymbolBasicType calculatedType; 
    int value; // Guarda el valor del INT
} ConstantIntNode;

typedef struct { //FLOAT
    NodeType type; 
    Node *next; 
    int line; 
    SymbolBasicType calculatedType;
    double value; //Guarda el valor del FLOAT
} ConstantFloatNode;

typedef struct { //BOOL(true o false)
    NodeType type; 
    Node *next; 
    int line; 
    SymbolBasicType calculatedType; 
    int value; //Guarda el valor de 1 o 0 dependiendo de si es true o false
} ConstantBoolNode;

typedef struct { //STRING (CADENA DE TEXTO)
    NodeType type; 
    Node *next; 
    int line; 
    SymbolBasicType calculatedType; 
    char *sval; //Campo que apunta a la cadena de texto
} StringLiteralNode;

typedef struct { //NOMBRE DE UNA VARIABLE O FUNCION
    NodeType type; 
    Node *next; 
    int line; 
    SymbolBasicType calculatedType; 
    char *sval; //Campo que apunta al nombre
} IdentifierNode;

typedef struct { //OPERACION QUE ACTUA SOBRE DOS VARIABLES
    NodeType type;
    Node *next; 
    int line; 
    SymbolBasicType calculatedType; 
    int op; //Guarda el token del operador
    Node *left; //Puntero al nodo de la expresion que esta a la izquierda
    Node *right; //Puntero al nodo de la expresion que esta a la derecha
} BinaryOpNode;

typedef struct { //REPRESENTACION DE UNA ASIGNACION
    NodeType type; 
    Node *next; 
    int line; 
    SymbolBasicType calculatedType; 
    IdentifierNode *identifier; //Puntero al IdentifierNode del lado izquierdo
    Node *expression; //Puntero al nodo de la expresión del lado derecho
} AssignmentNode;

typedef struct { //DECLARACION DE VARIABLE
    NodeType type; 
    Node *next; 
    int line; 
    SymbolBasicType calculatedType; 
    Node *varTypeNode; //Puntero a un IdentifierNode con el tipo
    IdentifierNode *identifier; //Puntero al IdentifierNode con el nombre de la variable
    Node *initialValue; //Puntero a la expresión de inicialización (Ejem: int)
} VarDeclarationNode;

typedef struct { //IF o ELSE IF o ELSE
    NodeType type; 
    Node* next; 
    int line; 
    SymbolBasicType calculatedType;
    Node* condition; //Puntero a la expresion booleana de la condicion
    Node* thenBranch; //Puntero al StatementListNode del bloque then
    Node* elseBranch; //Puntero al statement del bloque else (Tambien puede guardar un IfNode para generar un else if)
} IfNode;

typedef struct { //WHILE
    NodeType type; 
    Node* next; 
    int line; 
    SymbolBasicType calculatedType;
    Node* condition; //Puntero a la expresion booleana de la condicion
    Node* body; //Puntero hacia el cuerpo del while
} WhileNode;

typedef struct { //FOR
    NodeType type; 
    Node* next; 
    int line; 
    SymbolBasicType calculatedType; 
    Node* initialization;
    Node* condition; //Puntero a la expresion booleana de la condicion
    Node* increment; //Putero a la expresion de incrementacion
    Node* body; //Puntero hacia el cuerpo del for
} ForNode;

typedef struct { //PRINT
    NodeType type; 
    Node* next; 
    int line; 
    SymbolBasicType calculatedType;
    Node* expression; //Puntero a la expresion imprimir
} PrintNode;

typedef struct { //READ
    NodeType type; 
    Node* next; 
    int line; 
    SymbolBasicType calculatedType; 
    IdentifierNode* identifier; //Puntero al identificador de leer
} ReadNode;

typedef struct { //ENCABEZADO DE UNA FUNCION
    NodeType type; 
    Node* next; 
    int line; 
    SymbolBasicType calculatedType;
    Node* returnType; //Puntero al nodo del valor que retorna
    IdentifierNode* name; //Puntero al IdentifierNode con el nombre de la funcion
    Node* parameters; //Puntero a la cabeza de una lista enlazada de VarDeclarationNodes que representan los parametros
} FunctionSignatureNode;

typedef struct { //DEFINICION COMPLETA DE UNA FUNCION
    NodeType type; 
    Node* next; 
    int line; 
    SymbolBasicType calculatedType; 
    Node* signature; //Puntero hacia la firma de la funcion
    Node* body; //Puntero al cuerpo de la funcion
} FunctionDefNode;

typedef struct { //LLAMADA DE UNA FUNCION EN UNA EXPRESION
    NodeType type; 
    Node* next; 
    int line; 
    SymbolBasicType calculatedType; 
    IdentifierNode* name; //Puntero al IdentifierNode con el nombre de la funcion
    Node* arguments; //Puntero a la cabeza de una lista enlazada de nodos de expresion que son los argumentos
} FunctionCallNode;

typedef struct { //RETURN
    NodeType type; 
    Node* next; 
    int line; 
    SymbolBasicType calculatedType; 
    Node* returnValue; //Puntero al valor que retorna
} ReturnNode;

typedef struct { //NODO CONTENEDOR
    NodeType type; 
    Node *next; 
    int line; 
    SymbolBasicType calculatedType;
    std::vector<Node*>* statements; //Un std::vector que contiene punteros a cada una de las sentencias en el bloque
} StatementListNode;

//Definicion de las estructuras de cada nodo de cuales se va a componer el AST (FINAL)

//Declaracion de cada una de las funciones para crear la creacion de cada respectiva estructura del nodo(INICIO)

Node* createConstantIntNode(int value, int line); //Declaracion para el nodo que contiene la estructura para el INT
Node* createConstantFloatNode(double value, int line); //Declaracion para el nodo que contiene la estructura para el FLOAT
Node* createConstantBoolNode(int value, int line); //Declaracion para el nodo que contiene la estructura para variables de tipo BOOL
Node* createStringLiteralNode(char* sval, int line); //Declaracion para el nodo que contiene la estructura para los STRINGS
Node* createIdentifierNode(char* sval, int line); //Declaracion para el nodo que contiene la estructura para guardar el nombre de una VARIABLE o FUNCION
Node* createUnaryOpNode(int op, Node* operand, int line); //Declaracion para el nodo que contiene la estructura para guardar la OPERACION UNARIA
Node* createBinaryOpNode(int op, Node* left, Node* right, int line); //Declaracion para el nodo que contiene la estructura para guardar la OPERACION SOBRE DOS VARIABLES
Node* createAssignmentNode(Node* identifier, Node* expression, int line); //Declaracion para el nodo que contiene la estructura para la REPRESENTACION DE UNA ASIGNACION
Node* createStatementListNode(Node* statement, int line); //Declaracion para el nodo CONTENEDOR
Node* createFunctionDefNode(Node* signature, Node* body, int line); //Declaracion para el nodo que contiene la estructura COMPLETA DE UNA FUNCION
Node* createVarDeclarationNode(Node* typeNode, Node* idNode, Node* initVal, int line); //Declaracion para el nodo que contiene la estructura para la DECLARACION DE VARIABLES
Node* createFunctionSignatureNode(Node* typeNode, Node* nameNode, Node* params, int line); //Declaracion para el nodo que contiene la estructura para el ENCABEZADO DE UNA FUNCION
Node* createForNode(Node* init, Node* cond, Node* incr, Node* body, int line); //Declaracion para el nodo que contiene la estructura del FOR
Node* createIfNode(Node* cond, Node* thenB, Node* elseB, int line); //Declaracion para el nodo que contiene la estructura del IF
Node* createWhileNode(Node* cond, Node* body, int line); //Declaracion para el nodo que contiene la estructura del WHILE
Node* createPrintNode(Node* expr, int line); //Declaracion para el nodo que contiene la estructura del PRINT
Node* createReadNode(Node* idNode, int line); //Declaracion para el nodo que contiene la estructura del READ
Node* createReturnNode(Node* retVal, int line); //Declaracion para el nodo que contiene la estructura del RETURN
Node* createFunctionCallNode(Node* nameNode, Node* args, int line); //Declaracion para el nodo que contiene la estructura para la LLAMADA DE UNA FUNCION EN UNA EXPRESION
Node* createBreakNode(int line); //Declaracion para el nodo que contiene la estructura del BREAK
Node* createContinueNode(int line); //Declaracion para el nodo que contiene la estructura del CONTINUE

//Declaracion de cada una de las funciones para crear la creacion de cada respectiva estructura del nodo(FINAL)

Node* addParamToList(Node* listNode, Node* param); //Funcion auxiliar para añadir un nuevo nodo a la ParamList
Node* createParamList(Node* param); //Funcion para crear un ParamList

Node* addArgToList(Node* listNode, Node* arg); //Funcion auxiliar para añadir un nuevo nodo a la Arglist
Node* createArgList(Node* arg); //Funcion para crear un ArgList

Node* addStatementToList(Node* listNode, Node* statement); //funcion auxiliar para añadir un nuevo nodo a la StatementList

Node* duplicateTypeNode(Node* original); //Crear duplicacion de nodo de tipo para evitar la doble liberacion en la declaracion de multivariable

void freeAst(Node* node); //Funcion para liberar el AST
void printAst(Node* node, int indent = 0); //Funcion para mostrar por consola el AST creado


#endif