#ifndef TYPE_CHECKER_H
#define TYPE_CHECKER_H

#include "Ast.h"         
#include "symbol_table.h" 
#include <string>        


bool realizarVerificacionDeTipos(Node* raizDelAst);

SymbolBasicType obtenerYVerificarTipo(Node* nodoExpresion);


#endif 