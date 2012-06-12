#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ast* new_ast(int node_type, struct ast* left, struct ast* right) {
  struct ast* node = malloc(sizeof(struct ast));

  node->node_type = node_type;
  node->left = left;
  node->right = right;

  return node;
};

struct ast* new_int_number(int value) {
  struct int_number* node = malloc(sizeof(struct int_number));

  node->node_type = 'I';
  node->value = value;

  return (struct ast*)node;
};

struct ast* new_double_number(double value) {
  struct double_number* node = malloc(sizeof(struct double_number));

  node->node_type = 'D';
  node->value = value;

  return (struct ast*)node;
};

struct ast* new_string(char* value) {
  struct string* node = malloc(sizeof(struct string));

  node->node_type = 'S';
  node->value = malloc((sizeof(value)+1)*sizeof(char));
  strcpy(node->value, value);

  return (struct ast*)node;
};

struct ast* new_identifier(char* value) {
  struct identifier* node = malloc(sizeof(struct identifier));

  node->node_type = 'R';
  node->value = malloc((sizeof(value)+1)*sizeof(char));
  strcpy(node->value, value);

  return (struct ast*)node;
};

double eval_ast(struct ast*) {
  return 0;
};

void free_ast(struct ast* node) {
  switch(node->node_type) {
    case 'I' : case 'D' :  free(node);
    case 'S' : case 'R' :  struct string* s = (struct string*)node;
                           free(s->value);
                           free(s);
    case '+' : case '-' : case '*' : case '/' : case '%' : free_ast(node->right);
    default : printf("ERROR: when deleting %c.\n", node->node_type);
  }
};

void print_ast(struct ast*) {
  switch(node->node_type) {
    case 'I' :  printf("%d\n", node->value);
    case 'D' :  printf("%d\n", node->value);
    case 'S' :  case 'R' :  struct string* s = (struct string*)node;
                            printf("%s\n", s->value);
    case '+' : case '-' : case '*' : case '/' : case '%' : printf("%s\n", node->value);
    default : printf("ERROR: when printing %c.\n", node->node_type);
  }
};

