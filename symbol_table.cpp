#include <iostream>
#include "symbol_table.h"

SymbolManager* global_symbol_manager_instance_ptr = nullptr;

SymbolManager::SymbolManager() : m_current_scope_level(-1) {
    enterScope();
    std::cout << "[SymbolManager] SymbolManager construido. Ámbito global creado." << std::endl;
}

void SymbolManager::enterScope() {
    m_current_scope_level++;
    scope_stack.emplace_back();
}

void SymbolManager::exitScope() {
    if (!scope_stack.empty()) {
        scope_stack.pop_back(); 
        m_current_scope_level--;
    } else {
        std::cerr << "[SymbolManager] Error: Intento de salir de ámbito sin ámbitos en la pila." << std::endl;
    }
}

bool SymbolManager::insertSymbol(const SymbolTableEntry_CPP& entry_const) {
    if (scope_stack.empty()) {
        std::cerr << "[SymbolManager] Error: No hay ámbitos para insertar el símbolo '" << entry_const.name << "'." << std::endl;
        return false;
    }
    SymbolTableEntry_CPP entry = entry_const;
    entry.scope_level = m_current_scope_level;


    std::unordered_map<std::string, SymbolTableEntry_CPP>& current_scope_map = scope_stack.back();

    if (current_scope_map.count(entry.name)) {
        return false;
    }

    current_scope_map[entry.name] = entry;
    return true;
}

std::optional<SymbolTableEntry_CPP> SymbolManager::lookupSymbol(const std::string& name) const {
    for (auto it = scope_stack.rbegin(); it != scope_stack.rend(); ++it) {
        const auto& current_scope_map = *it;
        auto found_entry = current_scope_map.find(name);
        if (found_entry != current_scope_map.end()) {
            return found_entry->second; 
        }
    }
    return std::nullopt;
}

std::optional<SymbolTableEntry_CPP> SymbolManager::lookupSymbolCurrentScope(const std::string& name) const {
    if (scope_stack.empty()) {
        return std::nullopt;
    }
    const auto& current_scope_map = scope_stack.back();
    auto found_entry = current_scope_map.find(name);
    if (found_entry != current_scope_map.end()) {
        return found_entry->second;
    }
    return std::nullopt;
}

int SymbolManager::getCurrentScopeLevel() const {
    return m_current_scope_level;
}

const std::vector<ST_Param_CPP>* SymbolManager::getFunctionParamsList(const std::string& functionName) const {
    std::optional<SymbolTableEntry_CPP> entry_opt = this->lookupSymbol(functionName);
    if (entry_opt && entry_opt->kind == NODE_FUNCTION_DEF) {
        return &(entry_opt->params_list);
    }
    return nullptr;
}

void c_init_symbol_manager() {
    if (global_symbol_manager_instance_ptr == nullptr) {
        global_symbol_manager_instance_ptr = new SymbolManager();
    } else {
        std::cout << "[SymbolManager C-Interface] SymbolManager ya inicializado." << std::endl;
    }
}

void c_destroy_symbol_manager() {
    delete global_symbol_manager_instance_ptr;
    global_symbol_manager_instance_ptr = nullptr;
    std::cout << "[SymbolManager C-Interface] SymbolManager destruido." << std::endl;
}

void c_enter_scope() {
    if (global_symbol_manager_instance_ptr) {
        global_symbol_manager_instance_ptr->enterScope();
    } else {
        std::cerr << "Error: SymbolManager no inicializado antes de c_enter_scope." << std::endl;
    }
}

void c_exit_scope() {
    if (global_symbol_manager_instance_ptr) {
        global_symbol_manager_instance_ptr->exitScope();
    } else {
        std::cerr << "Error: SymbolManager no inicializado antes de c_exit_scope." << std::endl;
    }
}

SymbolBasicType c_ast_node_to_symbol_basic_type(Node* typeNode) {
    if (!typeNode || typeNode->type != NODE_TYPE) {
         if (typeNode && typeNode->type == NODE_IDENTIFIER && ((IdentifierNode*)typeNode)->sval) {
         } else {
            std::cerr << "Error Interno: Nodo de tipo AST inválido o nulo." << std::endl;
            return SYM_TYPE_UNKNOWN;
         }
    }
    
    IdentifierNode* type_id_node = (IdentifierNode*)typeNode;
    if (!type_id_node->sval) {
        std::cerr << "Error Interno: Nodo de tipo AST (IdentifierNode) con sval nulo." << std::endl;
        return SYM_TYPE_UNKNOWN;
    }


    std::string type_str = type_id_node->sval;
    if (type_str == "int") return SYM_TYPE_INT;
    if (type_str == "float") return SYM_TYPE_FLOAT;
    if (type_str == "string") return SYM_TYPE_STRING;
    if (type_str == "void") return SYM_TYPE_VOID;
    if (type_str == "bool") return SYM_TYPE_BOOL;
    
    std::cerr << "Error Interno: Tipo de AST desconocido en string: '" << type_str << "'" << std::endl;
    return SYM_TYPE_UNKNOWN;
}

std::vector<ST_Param_CPP> convert_ast_params_to_st_params_cpp(Node* astParamList) {
    std::vector<ST_Param_CPP> st_params;
    Node* current_ast_param = astParamList;

    while (current_ast_param != nullptr) {
        if (current_ast_param->type != NODE_VAR_DECLARATION) {
            std::cerr << "Error Interno: Se esperaba NODE_VAR_DECLARATION en lista de parámetros AST para SymbolManager." << std::endl;
            st_params.clear(); 
            return st_params;
        }
        VarDeclarationNode* var_decl_node = (VarDeclarationNode*)current_ast_param;
        IdentifierNode* param_id_node = var_decl_node->identifier;
        Node* param_type_node = var_decl_node->varTypeNode;

        if (!param_id_node || !param_id_node->sval || !param_type_node) {
             std::cerr << "Error Interno: Nodo de parámetro AST malformado." << std::endl;
             current_ast_param = current_ast_param->next;
             continue;
        }
        
        st_params.emplace_back(
            param_id_node->sval,
            c_ast_node_to_symbol_basic_type(param_type_node),
            var_decl_node->line
        );
        current_ast_param = current_ast_param->next;
    }
    return st_params;
}


int c_insert_variable(const char* name, Node* type_node, int line, Node* ast_decl_node) {
    if (!global_symbol_manager_instance_ptr) {
        std::cerr << "Error: SymbolManager no inicializado antes de c_insert_variable." << std::endl;
        return 0; 
    }
    if (!name || !type_node) {
        std::cerr << "Error: Argumentos nulos para c_insert_variable." << std::endl;
        return 0;
    }

    SymbolBasicType sym_type = c_ast_node_to_symbol_basic_type(type_node);
    if (sym_type == SYM_TYPE_UNKNOWN || sym_type == SYM_TYPE_VOID) {
        std::cerr << "Error Semántico (línea " << line << "): Tipo de variable inválido o desconocido para '" << name << "'." << std::endl;
        return 0;
    }
    
    SymbolTableEntry_CPP entry(name, sym_type, NODE_VAR_DECLARATION, line, 
                               global_symbol_manager_instance_ptr->getCurrentScopeLevel(), 
                               ast_decl_node);
    VarDeclarationNode* var_node = (VarDeclarationNode*)ast_decl_node;
    if(var_node && var_node->initialValue) {
        entry.is_initialized = true;
    }


    if (!global_symbol_manager_instance_ptr->insertSymbol(entry)) {
        return 0; 
    }
    return 1; 
}

int c_insert_function(const char* name, Node* return_type_node, Node* params_ast_node, int line, Node* ast_node_for_func_sig) {
    if (!global_symbol_manager_instance_ptr) {
        std::cerr << "Error Crítico: SymbolManager no inicializado antes de c_insert_function." << std::endl;
        return 0; // Fracaso
    }
    if (!name || !return_type_node) {
        std::cerr << "Error Crítico: Argumentos nulos para c_insert_function (nombre o tipo de retorno)." << std::endl;
        return 0;
    }

    SymbolBasicType func_ret_type = c_ast_node_to_symbol_basic_type(return_type_node);
    if (func_ret_type == SYM_TYPE_UNKNOWN || func_ret_type == SYM_TYPE_ERROR) {
        return 0; 
    }

    std::vector<ST_Param_CPP> st_params = convert_ast_params_to_st_params_cpp(params_ast_node);

    SymbolTableEntry_CPP entry(
        name,
        func_ret_type,
        NODE_FUNCTION_DEF, 
        line,
        global_symbol_manager_instance_ptr->getCurrentScopeLevel(), 
        ast_node_for_func_sig 
    );
    entry.params_list = st_params;

    if (!global_symbol_manager_instance_ptr->insertSymbol(entry)) {
        return 0;
    }
    
    global_symbol_manager_instance_ptr->enterScope();
    return 1; 
}

int c_lookup_symbol(const char* name, C_SymbolInfo* info_out) {
    if (!global_symbol_manager_instance_ptr) {
        std::cerr << "Error: SymbolManager no inicializado antes de c_lookup_symbol." << std::endl;
        if(info_out) info_out->found = 0;
        return 0;
    }
    if (!name) {
         if(info_out) info_out->found = 0;
        return 0;
    }

    std::optional<SymbolTableEntry_CPP> entry_opt = global_symbol_manager_instance_ptr->lookupSymbol(name);

    if (entry_opt) {
        if (info_out) {
            info_out->type = entry_opt->type;
            info_out->kind = entry_opt->kind;
            info_out->line = entry_opt->line;
            info_out->scope_level = entry_opt->scope_level;
            info_out->num_params = (entry_opt->kind == NODE_FUNCTION_DEF) ? entry_opt->params_list.size() : 0;
            info_out->found = 1;
        }
        return 1;
    } else {
        if (info_out) {
            info_out->found = 0;
        }
        return 0;
    }
}