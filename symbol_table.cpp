#include <iostream>
#include "Ast.h"
#include "symbol_table.h"

SymbolManager* global_symbol_manager_instance_ptr = nullptr;

SymbolManager::SymbolManager() : m_current_scope_level(-1) { //Inicializa el SymbolManager cuando se crea una nueva instancia 
    enterScope();
    std::cout << "[SymbolManager] SymbolManager construido. Ambito global creado." << std::endl; //Confirmacion de que se creo
}

void SymbolManager::enterScope() { //Abre un nuevo ambito anidado
    m_current_scope_level++;
    scope_stack.emplace_back(); //Agrega un nuevo nivel de ambito a la cima del scope_stack(Se ocupa cada que se define una nueva funcion)
}
//Cierra el ambito en el cual esta, para luego volver despues al "ambito padre"(Se utiliza cuando el parser termina de analizar una funcion)
void SymbolManager::exitScope() { 
    if (!scope_stack.empty()) { //Verificacion de que scope_stack no este vacia
        scope_stack.pop_back(); 
        m_current_scope_level--; //Drecrementa en 1 el contador de ambito
    } else {
        std::cerr << "[SymbolManager] Error: Intento de salir de ambito sin ambitos en la pila." << std::endl;
    }
}

bool SymbolManager::insertSymbol(const SymbolTableEntry_CPP& entry_const) { //Registrar un nuevo identificador (variable o función) en el ámbito actual.
    if (scope_stack.empty()) {
        std::cerr << "[SymbolManager] Error: No hay ambitos para insertar el simbolo '" << entry_const.name << "'." << std::endl;
        return false;
    }
    SymbolTableEntry_CPP entry = entry_const;
    entry.scope_level = m_current_scope_level;


    std::unordered_map<std::string, SymbolTableEntry_CPP>& current_scope_map = scope_stack.back(); //Obtencion de la referencia al ambito actual

    if (current_scope_map.count(entry.name)) { //Verificacion de si ya existe la variable actual en el ambito o no
        return false; //Si existe una variable con ese nombre retorna false
    }
    //Si no encuentra una variable con el mismo nombre, inserta la nueva entrada y retorna true
    current_scope_map[entry.name] = entry; 
    return true;
}

//Se encarga de buscar un identificador desde adentro hacia afuera
std::optional<SymbolTableEntry_CPP> SymbolManager::lookupSymbol(const std::string& name) const { 
    for (auto it = scope_stack.rbegin(); it != scope_stack.rend(); ++it) { //Recorre la pila desde el final hacia el inicio(ambito local al global)
        const auto& current_scope_map = *it;
        auto found_entry = current_scope_map.find(name);
        if (found_entry != current_scope_map.end()) { //Si encuentra el simbolo buscado devuelve la entrada completa y termina la busqueda
            return found_entry->second; 
        }
    }
    return std::nullopt; //Si no lo encuentra retorna nullopt
}

//Comprobacion de que no haya otra variable con el mismo nombre exclusivamente en el ambito actual
std::optional<SymbolTableEntry_CPP> SymbolManager::lookupSymbolCurrentScope(const std::string& name) const {
    if (scope_stack.empty()) { //Verificar de que la scope_stack no este vacia
        return std::nullopt;
    }
    const auto& current_scope_map = scope_stack.back(); //Obtiene una referencia al ambito actual
    auto found_entry = current_scope_map.find(name); //Busca si existe alguno con ese nombre
    if (found_entry != current_scope_map.end()) { //Si lo encuentra, entonces retorna su valor
        return found_entry->second;
    }
    return std::nullopt; //Si no lo encuentra entonces retorna nullopt
}

int SymbolManager::getCurrentScopeLevel() const { //Retorna el ambito en el cual se encuentra
    return m_current_scope_level;
}

//Obtiene los parametros de una funcion para ver si la llamada a esta misma es correcta o no
const std::vector<ST_Param_CPP>* SymbolManager::getFunctionParamsList(const std::string& functionName) const {
    std::optional<SymbolTableEntry_CPP> entry_opt = this->lookupSymbol(functionName);//Busca mediante el nombre a la funcion
    if (entry_opt && entry_opt->kind == NODE_FUNCTION_DEF) { //Si encuentra algo con el mismo nombre y es NODE_FUNCTION_DEF entonces
        return &(entry_opt->params_list); //Retorna la lista de parametros
    }
    return nullptr; //Si no la encuentra retorna nullptr
}

//Funciones ocupadas para poder cominicarse con el parser (INICIO)

void c_init_symbol_manager() { //Utilizado para inicializar la instancia 
    if (global_symbol_manager_instance_ptr == nullptr) {
        global_symbol_manager_instance_ptr = new SymbolManager();
    } else {
        std::cout << "[SymbolManager C-Interface] SymbolManager ya inicializado." << std::endl;
    }
}

void c_destroy_symbol_manager() { //Utilizado para liberar la memoria de la instancia utilizado
    delete global_symbol_manager_instance_ptr;
    global_symbol_manager_instance_ptr = nullptr;
    std::cout << "[SymbolManager C-Interface] SymbolManager destruido." << std::endl;
}

void c_enter_scope() { //Utilizado para llamar al metodo enterScope
    if (global_symbol_manager_instance_ptr) {
        global_symbol_manager_instance_ptr->enterScope();
    } else {
        std::cerr << "Error: SymbolManager no inicializado antes de c_enter_scope." << std::endl;
    }
}

void c_exit_scope() {//Utilizado para llamar al metodo exitScope
    if (global_symbol_manager_instance_ptr) {
        global_symbol_manager_instance_ptr->exitScope();
    } else {
        std::cerr << "Error: SymbolManager no inicializado antes de c_exit_scope." << std::endl;
    }
}

//Funcion utilizada para traducir el nodo del AST a un enum que pueda leer la tabla de simbolos
SymbolBasicType c_ast_node_to_symbol_basic_type(Node* typeNode) { 
    if (!typeNode || typeNode->type != NODE_TYPE) { //Verificador de que el nodo actual tiene un tipo valido
         if (typeNode && typeNode->type == NODE_IDENTIFIER && ((IdentifierNode*)typeNode)->sval) {
         } else {
            std::cerr << "Error Interno: Nodo de tipo AST invalido o nulo." << std::endl;
            return SYM_TYPE_UNKNOWN;
         }
    }
    
    IdentifierNode* type_id_node = (IdentifierNode*)typeNode; //Toma el tipo del nodo
    if (!type_id_node->sval) {//Verifica de que no sea nulo
        std::cerr << "Error Interno: Nodo de tipo AST (IdentifierNode) con sval nulo." << std::endl;
        return SYM_TYPE_UNKNOWN;
    }


    std::string type_str = type_id_node->sval; //Toma el tipo
    if (type_str == "int") return SYM_TYPE_INT; //retorna su valor enum (int)
    if (type_str == "float") return SYM_TYPE_FLOAT; //retorna su valor enum (float)
    if (type_str == "string") return SYM_TYPE_STRING; //retorna su valor enum (string)
    if (type_str == "void") return SYM_TYPE_VOID; //retorna su valor enum (void)
    if (type_str == "bool") return SYM_TYPE_BOOL; //retorna su valor enum (bool)
    
    std::cerr << "Error Interno: Tipo de AST desconocido en string: '" << type_str << "'" << std::endl;
    return SYM_TYPE_UNKNOWN; //Lo marca con un valor del tipo desconocido
}

//Convierte la lista enlzada de nodos que contienen los parametros de una funcion en un vector de ST_PARAM_CPP, que es el formato con el cual
//trabaja los parametros de las funciones la tabla de simbolos
std::vector<ST_Param_CPP> convert_ast_params_to_st_params_cpp(Node* astParamList) {
    std::vector<ST_Param_CPP> st_params; //Crea el vector para almacenar los parametros
    Node* current_ast_param = astParamList; //Guarda la raiz de la lista enlazada donde guarda los parametros de la funcion el AST para ocuparla

    while (current_ast_param != nullptr) { //Recorre la lista enlazada de parametros
        if (current_ast_param->type != NODE_VAR_DECLARATION) { //Si no se encuentra el tipo buscado tira error
            std::cerr << "Error Interno: Se esperaba NODE_VAR_DECLARATION en lista de parametros AST para SymbolManager." << std::endl;
            st_params.clear(); //limpia el vector de los parametros
            return st_params; //Retorna el vector vacio
        }
        VarDeclarationNode* var_decl_node = (VarDeclarationNode*)current_ast_param; //Toma la variable actual de la lista de parametros
        IdentifierNode* param_id_node = var_decl_node->identifier; //Toma el identificador de el nodo (Que contiene el nombre del parametro)
        Node* param_type_node = var_decl_node->varTypeNode; //Toma el tipo del nodo (Que contiene el tipo del parametro)

        if (!param_id_node || !param_id_node->sval || !param_type_node) { //Verifica si el nodo tiene los parametros bien formados
             std::cerr << "Error Interno: Nodo de parametro AST malformado." << std::endl;
             current_ast_param = current_ast_param->next;
             continue;
        }
        
        //Crea un nuevo objeto ST_PARAM_CPP pasandole los 3 valores correspondientes para poder ser creado
        st_params.emplace_back(
            param_id_node->sval,
            c_ast_node_to_symbol_basic_type(param_type_node), //Convierte el nodo de tipo AST a enum
            var_decl_node->line
        );
        current_ast_param = current_ast_param->next; //Pasa al parametro siguiente en la lista enlazada
    }
    return st_params; //retorna el vector con los parametros convertidos en el formato aceptado por la tabla de simbolos
}

//Funcion que permite tomar la declaracion de una variable y guardarla en la tabla de simbolos(LLamaada por el parser)
int c_insert_variable(const char* name, Node* type_node, int line, Node* ast_decl_node) {
    if (!global_symbol_manager_instance_ptr) { //Se asegura que la tabla de simbolos este inicializada
        std::cerr << "Error: SymbolManager no inicializado antes de c_insert_variable." << std::endl; 
        return 0; 
    }
    if (!name || !type_node) { //Se asegura de que los arguemntos no sean nulos
        std::cerr << "Error: Argumentos nulos para c_insert_variable." << std::endl;
        return 0;
    }
    //Llama a la funcion que traduce el valor del nodo del AST a su correspondiente valor en enum
    SymbolBasicType sym_type = c_ast_node_to_symbol_basic_type(type_node);
    if (sym_type == SYM_TYPE_UNKNOWN || sym_type == SYM_TYPE_VOID) { //Verifica que no sea de tipo desconocido o invalido
        std::cerr << "Error Semantico (linea " << line << "): Tipo de variable invalido o desconocido para '" << name << "'." << std::endl;
        return 0;
    }

    //Creacion de un objeto SymbolTableEntry_CPP con toda la informacion necesaria
    //Nombre, el tipo en formato enum, la linea, y el nivel del ambito actual(Obtenido desde el llamado a getCurrentScopeLevel)
    SymbolTableEntry_CPP entry(name, sym_type, NODE_VAR_DECLARATION, line, 
                               global_symbol_manager_instance_ptr->getCurrentScopeLevel(), 
                               ast_decl_node);

    VarDeclarationNode* var_node = (VarDeclarationNode*)ast_decl_node;
    if(var_node && var_node->initialValue) { //Verifica si el nodo declarado tiene algun tipo de valor inicial
        entry.is_initialized = true; //para asi marcar is_initialized como true
    }

    //Verifica si es que insertSymbol retorna false, si es asi retorna 0 que es utilizado para avisar que hay un error de re-declaracion
    if (!global_symbol_manager_instance_ptr->insertSymbol(entry)) { 
        return 0; 
    }
    return 1; //en caso contrario retorna 1 ya que la insercion fue exitosa
}

//Este metedo es llamada cuando el parser se encuentra con una nueva funcion
//Su trabajo es registrarlo, preparar el nuevo ambito para sus parametros y cuerpo
int c_insert_function(const char* name, Node* return_type_node, Node* params_ast_node, int line, Node* ast_node_for_func_sig) {
    if (!global_symbol_manager_instance_ptr) { //Se asegura que la tabla de simbolos este inicializada
        std::cerr << "Error Crítico: SymbolManager no inicializado antes de c_insert_function." << std::endl;
        return 0;
    }
    if (!name || !return_type_node) { //Se asegura de que los argumentos no sean nulos
        std::cerr << "Error Crítico: Argumentos nulos para c_insert_function (nombre o tipo de retorno)." << std::endl;
        return 0;
    }

    SymbolBasicType func_ret_type = c_ast_node_to_symbol_basic_type(return_type_node); //Utilizado para ver el tipo de retorno
    if (func_ret_type == SYM_TYPE_UNKNOWN || func_ret_type == SYM_TYPE_ERROR) { //Verifica que el tipo de retorno sea el adecuado o un tipo valido
        return 0; 
    }

    //Transforma la lista enlazada de parametros del AST en un vector de parametros
    std::vector<ST_Param_CPP> st_params = convert_ast_params_to_st_params_cpp(params_ast_node);

    //Creacion del objeto SymbolTableEntry_CPP con toda la informacion de la funcion que fue transformada al formato aceptado por la tabla de simbolos
    SymbolTableEntry_CPP entry(
        name,
        func_ret_type,
        NODE_FUNCTION_DEF, 
        line,
        global_symbol_manager_instance_ptr->getCurrentScopeLevel(), 
        ast_node_for_func_sig 
    );

    //Guarda el vector con la lista de parametros
    entry.params_list = st_params;

    //Intenta insertar la nueva funcion en la tabla de simbolos del ambito actual verificando de que no exista una con el mismo nombre
    if (!global_symbol_manager_instance_ptr->insertSymbol(entry)) {
        return 0; //Retorna 0 si es que ya existe la funcion 
    }
    
    //Si es insertado correctamente llama a enterScope, ya que tiene que agregar otro ambito, que seria el bloque de codigo que viene de la funcion
    global_symbol_manager_instance_ptr->enterScope();
    return 1; //Retorna 1 para verificar que fue insertada con exito    
}

//Funcion principal para poder buscar y verificar que existe un identificador en especifico(Llamado por parser y Type_checker)
int c_lookup_symbol(const char* name, C_SymbolInfo* info_out) {
    if (!global_symbol_manager_instance_ptr) { //Se asegura de que la tabla de simbolos este inicializada
        std::cerr << "Error: SymbolManager no inicializado antes de c_lookup_symbol." << std::endl;
        if(info_out) info_out->found = 0;
        return 0;
    }
    if (!name) { //Y que el nombre este bien (Que no sea nullptr)
         if(info_out) info_out->found = 0;
        return 0;
    }

    //Busca el identificador en la tabla de simbolos
    std::optional<SymbolTableEntry_CPP> entry_opt = global_symbol_manager_instance_ptr->lookupSymbol(name); 

    //La variable found es para verificar si se encontro o no de manera correcta el identificador en cuestion 
    if (entry_opt) { //Si no es nulo, entonces saca toda la informacion y la guarda en la estructura correspondiente para que la pueda leer el parser
        if (info_out) {
            info_out->type = entry_opt->type;
            info_out->kind = entry_opt->kind;
            info_out->line = entry_opt->line;
            info_out->scope_level = entry_opt->scope_level;
            info_out->num_params = (entry_opt->kind == NODE_FUNCTION_DEF) ? entry_opt->params_list.size() : 0;
            info_out->found = 1; 
        }
        return 1; //Si lo encontro retorna 1
    } else {
        if (info_out) {
            info_out->found = 0;
        }
        return 0; 
    }
}

//Funciones ocupadas para poder comunicarse con el parser (FINAL)