#ifndef AST_H
#define AST_H

/*****************************************/
/******* ABSTRACT SYNTAX TREE  ***********/
/*****************************************/

#include "structures.h"
#include "st.h"

//declaro variable global
extern struct sym* sym_table;

struct ast* new_ast_node(int, struct ast*, struct ast*);
struct ast* new_nil_node();
struct ast* new_bool_node(int);
struct ast* new_integer_node(int);
struct ast* new_double_node(double);
struct ast* new_string_node(char*);
struct ast* new_identifier_node(char*);
struct ast* new_symbol_node(char*);
struct list_node* new_list_node(int, struct ast*, struct list_node*);
struct ast* new_function_node(char*, struct list_node*, struct ast*);
struct ast* new_class_node(char*, struct list_node*);
struct ast* new_method_call_node(int, char*, char*, struct list_node*, struct ast*);
struct ast* new_if_node(int, struct ast*, struct ast*, struct ast*);
struct ast* new_opt_block_node(struct list_node*, struct ast*);

struct ast* eval_ast(struct ast*);
void free_ast(struct ast*);
void print_ast(struct ast*);

//
//
// aux functions

char* drop_quotes(char*);

#endif
