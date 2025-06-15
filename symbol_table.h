//Cabecera que define las estructuras de datos y las interfaces publicas, que otras partes del compilador ocupan 
//para interactuar con la tabla de symbolos
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <vector>
#include <unordered_map>
#include <optional> 
#include "common_types.h"

struct Node; //Declaracion utilizada para evitar la dependencia circular con los archivos del AST

struct ST_Param_CPP { //Estructura utilizada para guardar la lista de parametros de una funcion
    std::string name; //Nombre de la funcion
    SymbolBasicType type; //Tipo de la funcion(Ejem: int)
    int line;
    ST_Param_CPP(std::string n = "", SymbolBasicType t = SYM_TYPE_UNKNOWN, int l = 0)
        : name(std::move(n)), type(t), line(l) {}
};

class SymbolTableEntry_CPP //Clase que contiene toda la informacion relevante sobre un identificador declarado
{
public:
    std::string name; //Nombre del identificador(Variable o funcion)
    SymbolBasicType type; //El tipo de dato del simbolo (De la variable o funcion)
    NodeType kind; //la clase de simbolo (Variable o funcion)
    int line; //La linea en donde esta declarada
    int scope_level; // variable que indica el nivel del ambito (Ejemplo: 0 ambito global, 1 para las funciones declaradas en el ambito global)
    Node* ast_node; //Puntero que apunta de vuelta al nodo del AST donde fue declarado este simbolo

    std::vector<ST_Param_CPP> params_list;//Ambito utilizado solo si es una funcion, ya que guarda la lista de parametros

    bool is_initialized; //Variable que se utiliza para ver si una variable se le ha asignado un valor

    //Inicializacion del objeto
    SymbolTableEntry_CPP(std::string n = "", SymbolBasicType t = SYM_TYPE_UNKNOWN, NodeType k = NODE_PLACEHOLDER,
                         int l = 0, int s_lvl = 0, Node* an = nullptr, bool init = false)
        : name(std::move(n)), type(t), kind(k), line(l), scope_level(s_lvl), ast_node(an), is_initialized(init) {}
};

class SymbolManager { //Clase que contiene toda la logica para manejar todos los ambitos y entradas de la tabla de simbolos 
private:
    std::vector<std::unordered_map<std::string, SymbolTableEntry_CPP>> scope_stack; //vector utilizado como una pila de ámbitos 
    int m_current_scope_level; //Contador para llevar la cuenta de la profundidad del ambito
public:
    SymbolManager(); //Inicializador del objeto

    void enterScope(); //Crea un ambito vacio y lo coloca en la parte superior de la scope_stack (Se llama al entrar a una funcion) 
    void exitScope(); //Elimina el ambito actual de la scope_stack (Se llama al salir de la funcion)

    //Intenta añadir un nuevo simbolo al ambito actual(Devuelve false si ya existe un simbolo con ese nombre en ese mismo ambito)
    bool insertSymbol(const SymbolTableEntry_CPP& entry);

     //Busca un simbolo, devuelve la entrada si lo encuentra y nullptr si no 
    std::optional<SymbolTableEntry_CPP> lookupSymbol(const std::string& name) const;

    //Busca un simbolo pero exclusivamente en el ambito en el que se encuentre 
    std::optional<SymbolTableEntry_CPP> lookupSymbolCurrentScope(const std::string& name) const; 

    int getCurrentScopeLevel() const; //De vuelve el valor que se encuentra en m_current_scope_level

    //De vuelve la lista de parametros si es que se encuentra en una funcion
    const std::vector<ST_Param_CPP>* getFunctionParamsList(const std::string& functionName) const; 
};

std::string symbolBasicTypeToString(SymbolBasicType type);

//Fachada utilizada para que Bison (Que esta basado en C) pueda comunicarse con SymbolManager (Que esta basada en C++) 
#ifdef __cplusplus
extern "C" {
#endif

extern SymbolManager* global_symbol_manager_instance_ptr; //Puntero que guardara la instancia del SymbolManager

typedef struct C_SymbolInfo { //Estructura utilizada para pasar la informacion de un simbolo desde la tabla de vuelta  el parser
    SymbolBasicType type;
    NodeType kind;
    int line;
    int scope_level;
    int num_params;
    int found;
} C_SymbolInfo;

void c_init_symbol_manager(); //Creacion de la instancia en memoria de la clase SymbolManager y se guarda en global_symbol_manager_instance_ptr
void c_destroy_symbol_manager(); //Liberacion de la memoria utilizada en SymbolManager

void c_enter_scope(); //Llama al metodo enterScopce
void c_exit_scope(); //Llama al metodo exitScope

//Guarda la informacion de la declaracion de una variable
int c_insert_variable(const char* name, Node* type_node, int line, Node* ast_decl_node); 

//Guarda la informacion de la declaracion de una funcion
int c_insert_function(const char* name, Node* return_type_node, Node* params_ast_node, int line, Node* ast_func_def_node);

int c_lookup_symbol(const char* name, C_SymbolInfo* info_out); //Llama a la funcion de busqueda lookupSymbol

//Funcion que toma de un nodo del AST el tipo y lo traduce a su enum correspondiente
SymbolBasicType c_ast_node_to_symbol_basic_type(Node* typeNode); 

#ifdef __cplusplus
} 
#endif

#endif