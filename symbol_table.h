#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <vector>
#include <unordered_map>
#include <optional> 
#include "Ast.h"

typedef enum {
    SYM_TYPE_INT,
    SYM_TYPE_FLOAT,
    SYM_TYPE_STRING,
    SYM_TYPE_VOID,     
    SYM_TYPE_BOOL,
    SYM_TYPE_UNKNOWN,
    SYM_TYPE_ERROR
} SymbolBasicType;

struct ST_Param_CPP {
    std::string name;
    SymbolBasicType type;
    int line;
    ST_Param_CPP(std::string n = "", SymbolBasicType t = SYM_TYPE_UNKNOWN, int l = 0)
        : name(std::move(n)), type(t), line(l) {}
};

class SymbolTableEntry_CPP 
{
public:
    std::string name;
    SymbolBasicType type;
    NodeType kind;
    int line;
    int scope_level;
    Node* ast_node;

    std::vector<ST_Param_CPP> params_list;

    bool is_initialized;

    SymbolTableEntry_CPP(std::string n = "", SymbolBasicType t = SYM_TYPE_UNKNOWN, NodeType k = NODE_PLACEHOLDER,
                         int l = 0, int s_lvl = 0, Node* an = nullptr, bool init = false)
        : name(std::move(n)), type(t), kind(k), line(l), scope_level(s_lvl), ast_node(an), is_initialized(init) {}
};

class SymbolManager {
private:
    std::vector<std::unordered_map<std::string, SymbolTableEntry_CPP>> scope_stack;
    int m_current_scope_level;
public:
    SymbolManager();

    void enterScope();
    void exitScope();
    bool insertSymbol(const SymbolTableEntry_CPP& entry);
    std::optional<SymbolTableEntry_CPP> lookupSymbol(const std::string& name) const;
    std::optional<SymbolTableEntry_CPP> lookupSymbolCurrentScope(const std::string& name) const;

    int getCurrentScopeLevel() const;
    const std::vector<ST_Param_CPP>* getFunctionParamsList(const std::string& functionName) const;
};

#ifdef __cplusplus
extern "C" {
#endif

extern SymbolManager* global_symbol_manager_instance_ptr;

typedef struct C_SymbolInfo {
    SymbolBasicType type;
    NodeType kind;
    int line;
    int scope_level;
    int num_params;
    int found;
} C_SymbolInfo;

void c_init_symbol_manager();
void c_destroy_symbol_manager();

void c_enter_scope();
void c_exit_scope();

int c_insert_variable(const char* name, Node* type_node, int line, Node* ast_decl_node);
int c_insert_function(const char* name, Node* return_type_node, Node* params_ast_node, int line, Node* ast_func_def_node);

int c_lookup_symbol(const char* name, C_SymbolInfo* info_out);

SymbolBasicType c_ast_node_to_symbol_basic_type(Node* typeNode);


#ifdef __cplusplus
} 
#endif

#endif