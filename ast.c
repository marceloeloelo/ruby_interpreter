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

struct ast* new_binary_op(int node_type, struct ast* left, struct ast* right) {
  struct binary_op* node = malloc(sizeof(struct binary_op));

  node->node_type = node_type;
  node->left = left;
  node->right = right;

  return (struct ast*)node;
};

struct ast* new_int_number(int value) {
  struct int_number* node = malloc(sizeof(struct int_number));

  node->node_type = N_INTEGER;
  node->value = value;

  return (struct ast*)node;
};

struct ast* new_double_number(double value) {
  struct double_number* node = malloc(sizeof(struct double_number));

  node->node_type = N_DOUBLE;
  node->value = value;

  return (struct ast*)node;
};

struct ast* new_string(char* value) {
  struct string* node = malloc(sizeof(struct string));

  node->node_type = N_STRING_1;
  node->value = malloc((sizeof(value)+1)*sizeof(char));
  strcpy(node->value, value);

  return (struct ast*)node;
};

struct ast* new_identifier(char* name) {
  struct identifier* node = malloc(sizeof(struct identifier));

  node->node_type = N_IDENTIFIER;
  node->name = malloc((sizeof(name)+1)*sizeof(char));
  strcpy(node->name, name);

  return (struct ast*)node;
};

struct ast* new_function(char* name, struct ast* params) {
  struct function* node = malloc(sizeof(struct function));

  node->node_type = N_FUNCTION;
  node->name = malloc((sizeof(name)+1)*sizeof(char));
  node->params = params;
  strcpy(node->name, name);

  return (struct ast*)node;
};

double eval_ast(struct ast* node) {
  return 0;
};

void free_ast(struct ast* node) {
  switch(node->node_type) {
    case N_INTEGER : case N_DOUBLE :  free(node);
                           break;
    case N_STRING_1 : case N_IDENTIFIER :  //struct string* s = (struct string*)node;
                           //free(s->value);
                           //free(s);
                           break;
    case N_PLUS : case N_MINUS : case N_MUL : case N_DIV : case N_MODULO : free_ast(node->right);
                                                           break;
    default : printf("ERROR: when deleting %c.\n", node->node_type);
  }
};

void print_ast(struct ast* node) {
  switch(node->node_type) {
    case N_INTEGER :  {
                  struct int_number* i = (struct int_number*)node;
                  printf("%d", i->value);
                  break;
                };
    case N_DOUBLE :  {
                  struct double_number* d = (struct double_number*)node;
                  printf("%f", d->value);
                  break;
                };
    /* case N_STRING_1 :  case N_IDENTIFIER :  {*/
    /*                           struct string* s = (struct string*)node;*/
    /*                           printf("%s", s->name);*/
    /*                           break;*/
    /*                         };*/
    case N_PLUS : {
                 print_ast(node->left);
                 printf("+");
                 print_ast(node->right);
                 break;
               };
    case N_MINUS : {
                 print_ast(node->left);
                 printf("-");
                 print_ast(node->right);
                 break;
               };
    case N_MUL : {
                 print_ast(node->left);
                 printf("*");
                 print_ast(node->right);
                 break;
               };
    case N_DIV : {
                 print_ast(node->left);
                 printf("/");
                 print_ast(node->right);
                 break;
               };
// case '+' : case '-' : case '*' : case '/' : case '%' : printf("%s\n", node->value);
    default : printf("ERROR: when printing %c.\n", node->node_type);
  }
};

