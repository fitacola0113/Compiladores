#ifndef _SEMANTICS_H
#define _SEMANTICS_H

#include <stdbool.h>
#include "ast.h"

typedef struct _symbol {
    char *name;
    char *type;
    char *param_types;
    struct _symbol_table *local_table;
    bool is_param;
    struct _symbol *next;
} symbol;

typedef struct _symbol_table {
    char *table_name;
    symbol *symbols;
    struct _symbol_table *next;
    bool is_printed; 
} symbol_table;

extern symbol_table *global_table;
extern int semantic_errors;

void check_program(node *program_node);
void annotate_tree(node *n, symbol_table *local_table);
symbol_table *create_symbol_table(char *table_name);
symbol *insert_symbol(symbol_table *table, const char *name, const char *type, const char *param_types, bool is_param);
symbol *search_variable_local(symbol_table *table, const char *name);
symbol *search_method_local(symbol_table *table, const char *name, const char *param_types);
symbol *search_variable_global(symbol_table *local_table, const char *name);
symbol *search_method_global(const char *name);
void print_symbol_tables();
void free_symbol_tables();

#endif