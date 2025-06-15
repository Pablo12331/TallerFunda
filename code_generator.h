
#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include "Ast.h" 
#include <string> 

//Metodo que se encarga de la traduccion de el codigo a C++
void generate_code(Node* ast_root, const std::string& output_filename);

#endif