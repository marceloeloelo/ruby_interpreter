#include "ast.h"
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

struct arg_list_node* new_arg_list_node(struct ast* arg, struct arg_list_node* next) {
  struct arg_list_node* node = malloc(sizeof(struct arg_list_node));
  node->arg = arg;
  node->next = next;
  return node;
};

struct ast* new_function_node(char* name, struct arg_list_node* args, struct ast* stmts) {
  struct function_node* node = malloc(sizeof(struct function_node));
  node->node_type = N_FUNCTION;
  node->name = malloc((sizeof(name)+1)*sizeof(char));
  strcpy(node->name, name);
  node->args = args;
  node->stmts = stmts;
  return (struct ast*)node;
};

struct ast* new_class_node(char* name, struct ast* stmts) {
  struct class_node* node = malloc(sizeof(struct class_node));
  node->node_type = N_CLASS;
  node->name = malloc((sizeof(name)+1)*sizeof(char));
  strcpy(node->name, name);
  node->stmts = stmts;
  return (struct ast*)node;
};

struct ast* new_method_call_node(int node_type, char* class_name, char* method_name, struct arg_list_node* args) {
  struct method_call_node* node = malloc(sizeof(struct method_call_node));
  node->node_type = node_type;
  node->method_name = malloc((sizeof(method_name)+1)*sizeof(char));
  strcpy(node->method_name, method_name);

  if (node_type == N_METHOD_CALL_1) {
    node->class_name = malloc((sizeof(class_name)+1)*sizeof(char));
    strcpy(node->class_name, class_name);
  } else {
    node->class_name = NULL;
  };

  node->args = args;
  return (struct ast*)node;
}

/////////////////////////////////////

double eval_ast(struct ast* node) {
  return 0;
};

void free_ast(struct ast* node) {
};

void print_ast(struct ast* node) {
  if (node != NULL) {
    switch(node->node_type) {
      case N_INTEGER : {
                         struct integer_node* i = (struct integer_node*)node;
                         printf("%d", i->value);
                         break;
                       };
      case N_DOUBLE  : {
                         struct double_node* d = (struct double_node*)node;
                         printf("%f", d->value);
                         break;
                       };
      case N_STRING_1: {
                         struct string_node* s = (struct string_node*)node;
                         printf("%s", s->value);
                         break;
                       };
      case N_IDENTIFIER : {
                         struct identifier_node* i = (struct identifier_node*)node;
                         printf("%s", i->name);
                         break;
                       };
      case N_OP_EQUAL : {
                          print_ast(node->left);
                          printf(" = ");
                          print_ast(node->right);
                          break;
                        };
      case N_OP_PLUS_EQ : {
                          print_ast(node->left);
                          printf(" += ");
                          print_ast(node->right);
                          break;
                        };
      case N_OP_MINUS_EQ : {
                          print_ast(node->left);
                          printf(" -= ");
                          print_ast(node->right);
                          break;
                        };
      case N_OP_MUL_EQ : {
                          print_ast(node->left);
                          printf(" *= ");
                          print_ast(node->right);
                          break;
                        };
      case N_OP_DIV_EQ : {
                          print_ast(node->left);
                          printf(" /= ");
                          print_ast(node->right);
                          break;
                        };
      case N_OP_MODULO_EQ : {
                          print_ast(node->left);
                          printf(" mod= ");
                          print_ast(node->right);
                          break;
                        };
      case N_ARG_LIST : {
                          struct arg_list_node* l = (struct arg_list_node*)node;
                          printf("(");
                          struct arg_list_node* aux = l;
                          while (aux != NULL) {
                            print_ast(aux->arg);
                            printf(", ");
                            aux = aux->next;
                          };
                          printf(")");
                          break;
                        };
      case N_OP_MUL     : {
                         print_ast(node->left);
                         printf(" * ");
                         print_ast(node->right);
                         break;
                       };
      case N_OP_DIV     : {
                         print_ast(node->left);
                         printf(" / ");
                         print_ast(node->right);
                         break;
                       };
      case N_OP_MODULO  : {
                         print_ast(node->left);
                         printf(" mod ");
                         print_ast(node->right);
                         break;
                       };
      case N_OP_PLUS    : {
                         print_ast(node->left);
                         printf(" + ");
                         print_ast(node->right);
                         break;
                       };
      case N_OP_MINUS   : {
                         print_ast(node->left);
                         printf(" - ");
                         print_ast(node->right);
                         break;
                       };
      case N_OP_CMP_GT  : {
                         print_ast(node->left);
                         printf(" > ");
                         print_ast(node->right);
                         break;
                       };
      case N_OP_CMP_GT_EQ : {
                         print_ast(node->left);
                         printf(" >= ");
                         print_ast(node->right);
                         break;
                       };
      case N_OP_CMP_LE   : {
                         print_ast(node->left);
                         printf(" < ");
                         print_ast(node->right);
                         break;
                       };
      case N_OP_CMP_LE_EQ   : {
                         print_ast(node->left);
                         printf(" <= ");
                         print_ast(node->right);
                         break;
                       };
      case N_OP_CMP_EQ   : {
                         print_ast(node->left);
                         printf(" == ");
                         print_ast(node->right);
                         break;
                       };
      case N_OP_CMP_EQ_EQ   : {
                         print_ast(node->left);
                         printf(" === ");
                         print_ast(node->right);
                         break;
                       };
      case N_OP_CMP_INEQ   : {
                         print_ast(node->left);
                         printf(" <=> ");
                         print_ast(node->right);
                         break;
                       };
      case N_OP_CMP_NEG  : {
                         print_ast(node->left);
                         printf(" != ");
                         print_ast(node->right);
                         break;
                       };
      case N_OP_CMP_AND  : {
                         print_ast(node->left);
                         printf(" && ");
                         print_ast(node->right);
                         break;
                       };
      case N_OP_CMP_OR  : {
                         print_ast(node->left);
                         printf(" || ");
                         print_ast(node->right);
                         break;
                       };
      case N_OP_PLUS_UN : {
                            printf("+");
                            print_ast(node->left);
                            break;
                          };
      case N_OP_MINUS_UN : {
                            printf("+");
                            print_ast(node->left);
                            break;
                          };
      case N_OP_NOT : {
                            printf("+");
                            print_ast(node->left);
                            break;
                          };
      case N_STMT_LIST : {
                           print_ast(node->right);
                           print_ast(node->left);
                           break;
                         };
      case N_FUNCTION : {
                          struct function_node* f = (struct function_node*)node;
                          printf("def %s", f->name);

                          /* function parameters */
                          printf("(");
                          struct arg_list_node* aux = f->args;
                          while (aux != NULL) {
                            print_ast(aux->arg);
                            printf(", ");
                            aux = aux->next;
                          };
                          printf(")\n");

                          /* comp_statements */
                          print_ast(f->stmts);

                          printf("end");
                          break;
                        };
      case N_RETURN   : {
                         printf("return ");
                         /* expression */
                         print_ast(node->left);
                         printf("\n");
                         break;
                       };
      case N_WHILE   : {
                         printf("while ");
                         /* expression */
                         print_ast(node->left);

                         printf("\n");

                         /* comp_statements */
                         print_ast(node->right);

                         printf("\nend");
                         break;
                       };
      case N_CLASS   : {
                         struct class_node* c = (struct class_node*)node;

                         /* class name */
                         printf("class %s\n", c->name);
                         /* comp_statements */
                         print_ast(c->stmts);

                         printf("end");
                         break;
                       };
      default        : {
                         printf("ERROR: when printing %c.\n", node->node_type);
                       };
    }
  }
};
