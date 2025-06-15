//Archivo creado para evitar la dependencia circular entre el AST y la tabla de simbolos
#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

// Definición de todos los tipos de nodos que pueden existir en el lenguaje(Etiquetas que nos ayudana saber que informacion estamos guardando)
typedef enum {
    NODE_CONSTANT_INT, NODE_CONSTANT_FLOAT, NODE_CONSTANT_STRING, NODE_CONSTANT_BOOL,
    NODE_IDENTIFIER, NODE_BINARY_OP, NODE_UNARY_OP,
    NODE_ASSIGNMENT, NODE_VAR_DECLARATION,
    NODE_IF, NODE_WHILE, NODE_FOR,
    NODE_BREAK, NODE_CONTINUE,
    NODE_PRINT, NODE_READ,
    NODE_FUNCTION_DEF, NODE_FUNCTION_CALL, NODE_FUNC_SIG,
    NODE_RETURN,
    NODE_STATEMENT_LIST,
    NODE_TYPE,
    NODE_PARAM_LIST, NODE_ARG_LIST,
    NODE_PLACEHOLDER
} NodeType;

// Definición de los tipos de datos fundamentales y especiales del lenguaje
typedef enum {
    SYM_TYPE_INT,
    SYM_TYPE_FLOAT,
    SYM_TYPE_STRING,
    SYM_TYPE_VOID,
    SYM_TYPE_BOOL,
    SYM_TYPE_UNKNOWN,
    SYM_TYPE_ERROR,
    SYM_TYPE_UNCHECKED
} SymbolBasicType;

#endif // COMMON_TYPES_H