#ifndef TYPE_CHECKER_H
#define TYPE_CHECKER_H

#include "Ast.h"         
#include "symbol_table.h" 
#include <string>        

//Funcion auxiliar que se especializa en los nodos de expresiones
//Al recibir un puntero de una expresion, de manera recursiva calcula y devuelve el tipo de dato final de la expresion 
SymbolBasicType getAndCheckType(Node* expressionNode);

//Funcion de entrada principal para toda la fase de analisis semantico
//Recibe la raiz del arbol AST y verifica que todo este correcto, si encuentra algun error retorna false y reporta el error
bool performTypeChecking(Node* astRoot); 

#endif 