#ifndef AST_H
#define AST_H

/*****************************************/
/******* ABSTRACT SYNTAX TREE  ***********/
/*****************************************/

#include "structures.h"
#include "st.h"
#include "errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


struct ast* new_ast_node(int, struct ast*, struct ast*);
struct ast* new_nil_node();
struct ast* new_bool_node(int);
struct ast* new_integer_node(int);
struct ast* new_double_node(double);
struct ast* new_string_one_node(char*);
struct ast* new_string_two_node(char*);
struct ast* new_identifier_node(char*);
struct ast* new_symbol_node(char*);
struct ast* new_object_node(struct class*, struct sym*);
struct ast* new_constant_node(char*);
struct list_node* new_list_node(int, struct ast*, struct list_node*);
struct ast* new_function_node(char*, struct list_node*, struct ast*);
struct ast* new_class_node(char*, struct list_node*);
struct ast* new_method_call_node(int, struct ast*, char*, struct list_node*, struct ast*);
struct ast* new_if_node(struct ast*, struct ast*, struct ast*);
struct ast* new_opt_block_node(struct list_node*, struct ast*);
struct ast* new_array_access_node(struct ast*, char*);

int bool_value(struct ast*);
int int_value(struct ast*);
double double_value(struct ast*);
char* string_value(struct ast*);

void free_ast(struct ast*);

#endif
