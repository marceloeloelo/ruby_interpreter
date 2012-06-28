#include "ast.h"
#include "errors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

struct ast* new_ast_node(int node_type, struct ast* left, struct ast* right) {
  struct ast* node = malloc(sizeof(struct ast));

  node->node_type = node_type;
  node->left = left;
  node->right = right;

  return node;
};

struct ast* new_nil_node() {
  struct integer_node* node = malloc(sizeof(struct nil_node));
  node->node_type = N_NIL;
  return (struct ast*)node;
};

struct ast* new_bool_node(int value) {
  struct bool_node* node = malloc(sizeof(struct bool_node));
  node->node_type = N_BOOL;
  node->value = value;
  return (struct ast*)node;
};

struct ast* new_integer_node(int value) {
  struct integer_node* node = malloc(sizeof(struct integer_node));
  node->node_type = N_INTEGER;
  node->value = value;
  return (struct ast*)node;
};

struct ast* new_double_node(double value) {
  struct double_node* node = malloc(sizeof(struct double_node));
  node->node_type = N_DOUBLE;
  node->value = value;
  return (struct ast*)node;
};

// TODO crear string_nodo para interpolación
struct ast* new_string_node(char* value) {
  struct string_node* node = malloc(sizeof(struct string_node));
  node->node_type = N_STRING_1;
  node->value = malloc((strlen(value)+1)*sizeof(char));
  strcpy(node->value, value);
  return (struct ast*)node;
};

struct ast* new_identifier_node(char* name) {
  struct identifier_node* node = malloc(sizeof(struct identifier_node));
  node->node_type = N_IDENTIFIER;
  node->name = malloc((strlen(name)+1)*sizeof(char));
  strcpy(node->name, name);
  return (struct ast*)node;
};

struct arg_list_node* new_arg_list_node(struct ast* arg, struct arg_list_node* next) {
  struct arg_list_node* node = malloc(sizeof(struct arg_list_node));
  node->arg = arg;
  node->next = next;
  return node;
};

struct ast* new_function_node(char* name, struct arg_list_node* args, struct ast* stmts) {
  struct function_node* node = malloc(sizeof(struct function_node));
  node->node_type = N_FUNCTION;
  node->name = malloc((strlen(name)+1)*sizeof(char));
  strcpy(node->name, name);
  node->args = args;
  node->stmts = stmts;
  return (struct ast*)node;
};

struct ast* new_class_node(char* name, struct ast* stmts) {
  struct class_node* node = malloc(sizeof(struct class_node));
  node->node_type = N_CLASS;
  node->name = malloc((strlen(name)+1)*sizeof(char));
  strcpy(node->name, name);
  node->stmts = stmts;
  return (struct ast*)node;
};

struct ast* new_method_call_node(int node_type, char* class_name, char* method_name, struct arg_list_node* args) {
  struct method_call_node* node = malloc(sizeof(struct method_call_node));
  node->node_type = node_type;
  node->method_name = malloc((strlen(method_name)+1)*sizeof(char));
  strcpy(node->method_name, method_name);

  if (node_type == N_METHOD_CALL_1) {
    node->class_name = malloc((strlen(class_name)+1)*sizeof(char));
    strcpy(node->class_name, class_name);
  } else {
    node->class_name = NULL;
  };

  node->args = args;
  return (struct ast*)node;
}

struct ast* new_if_node(int node_type, struct ast* condition, struct ast* th, struct ast* el) {
  struct if_node* node = malloc(sizeof(struct if_node));
  node->node_type = node_type;
  node->condition = condition;
  node->th = th;
  node->el = el;
  return (struct ast*)node;
};

//
//
// other functions

int bool_value(struct ast* ast) {
  if (ast->node_type == N_BOOL) {
    struct bool_node* b = (struct bool_node*) ast;
    return b->value;
  } else if (ast->node_type == N_NIL) {
    struct nil_node* n = (struct nil_node*) ast;
    return 0;
  } else {
    return 1; 
  };
};

int int_value(struct ast* ast) {
  if (ast->node_type == N_INTEGER) {
    struct integer_node* i = (struct integer_node*) ast;
    return i->value;
  } else {
    return 0; // no debería pasar
  };
};

double double_value(struct ast* ast) {
  if (ast->node_type == N_DOUBLE) {
    struct double_node* d = (struct double_node*) ast;
    return d->value;
  } else if (ast->node_type == N_INTEGER) {
    struct integer_node* d = (struct integer_node*) ast;
    return (double) d->value;
  } else {
    return 0; // no debería pasar
  };
};

char* string_value(struct ast* ast) {
  if ((ast->node_type == N_STRING_1) || (ast->node_type == N_STRING_2)) {
    struct string_node* s = (struct string_node*) ast;
    return s->value;
  } else {
    return ""; // no debería pasar
  };
};

void free_ast(struct ast* node) {
};

