#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

struct ast* new_ast(int node_type, struct ast* left, struct ast* right) {
  struct ast* node = malloc(sizeof(struct ast));

  node->node_type = node_type;
  node->left = left;
  node->right = right;

  return node;
};

struct ast* new_binary_op_node(int node_type, struct ast* left, struct ast* right) {
  struct binary_op_node* node = malloc(sizeof(struct binary_op_node));
  node->node_type = node_type;
  node->left = left;
  node->right = right;
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

struct ast* new_string_node(char* value) {
  struct string_node* node = malloc(sizeof(struct string_node));
  node->node_type = N_STRING_1;
  node->value = malloc((sizeof(value)+1)*sizeof(char));
  strcpy(node->value, value);
  return (struct ast*)node;
};

struct ast* new_identifier_node(char* name) {
  struct identifier_node* node = malloc(sizeof(struct identifier_node));
  node->node_type = N_IDENTIFIER;
  node->name = malloc((sizeof(name)+1)*sizeof(char));
  strcpy(node->name, name);
  return (struct ast*)node;
};

struct ast* new_arg_list_node(struct ast* arg, struct ast* next) {
  struct arg_list_node* node = malloc(sizeof(struct arg_list_node));
  node->node_type = N_ARG_LIST;
  node->arg = arg;
  node->next = (struct arg_list_node*)next;
  return (struct ast*)node;
};

struct ast* new_stmt_list_node(struct ast* stmt, struct ast* next) {
  struct stmt_list_node* node = malloc(sizeof(struct stmt_list_node));
  node->node_type = N_STMT_LIST;
  node->stmt = (struct stmt_node*)stmt;
  node->next = (struct stmt_list_node*)next;
  return (struct ast*)node;
};

struct ast* new_function_node(char* name, struct ast* args, struct ast* stmts) {
  struct function_node* node = malloc(sizeof(struct function_node));

  node->node_type = N_FUNCTION;
  node->name = malloc((sizeof(name)+1)*sizeof(char));
  strcpy(node->name, name);
  node->args = (struct arg_list_node*)args;
  node->stmts = (struct stmt_list_node*)stmts;

  return (struct ast*)node;
};


/////////////////////////////////////

double eval_ast(struct ast* node) {
  return 0;
};

void free_ast(struct ast* node) {
  /* switch(node->node_type) {*/
  /*   case N_INTEGER : case N_DOUBLE :  free(node);*/
  /*                          break;*/
  /*   case N_STRING_1 : case N_IDENTIFIER :  //struct string* s = (struct string*)node;*/
  /*                          //free(s->value);*/
  /*                          //free(s);*/
  /*                          break;*/
  /*   case N_PLUS : case N_MINUS : case N_MUL : case N_DIV : case N_MODULO : free_ast(node->right);*/
  /*                                                          break;*/
  /*   default : printf("ERROR: when deleting %c.\n", node->node_type);*/
  /* }*/
};

void print_ast(struct ast* node) {
/*   switch(node->node_type) {*/
/*     case N_INTEGER :  {*/
/*                   struct int_number* i = (struct int_number*)node;*/
/*                   printf("%d", i->value);*/
/*                   break;*/
/*                 };*/
/*     case N_DOUBLE :  {*/
/*                   struct double_number* d = (struct double_number*)node;*/
/*                   printf("%f", d->value);*/
/*                   break;*/
/*                 };*/
/*     [> case N_STRING_1 :  case N_IDENTIFIER :  {<]*/
/*     [>                           struct string* s = (struct string*)node;<]*/
/*     [>                           printf("%s", s->name);<]*/
/*     [>                           break;<]*/
/*     [>                         };<]*/
/*     case N_PLUS : {*/
/*                  print_ast(node->left);*/
/*                  printf("+");*/
/*                  print_ast(node->right);*/
/*                  break;*/
/*                };*/
/*     case N_MINUS : {*/
/*                  print_ast(node->left);*/
/*                  printf("-");*/
/*                  print_ast(node->right);*/
/*                  break;*/
/*                };*/
/*     case N_MUL : {*/
/*                  print_ast(node->left);*/
/*                  printf("*");*/
/*                  print_ast(node->right);*/
/*                  break;*/
/*                };*/
/*     case N_DIV : {*/
/*                  print_ast(node->left);*/
/*                  printf("/");*/
/*                  print_ast(node->right);*/
/*                  break;*/
/*                };*/
/* // case '+' : case '-' : case '*' : case '/' : case '%' : printf("%s\n", node->value);*/
/*     default : printf("ERROR: when printing %c.\n", node->node_type);*/
/*   }*/
};

