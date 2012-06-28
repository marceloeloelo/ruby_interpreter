
#include "print.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

void print_ast(struct ast* node) {
  if (node != NULL) {
    switch(node->node_type) {
      case N_NIL : {
                              printf("nil");
                              break;
      };
      case N_BOOL : {
                              if (bool_value(node) == 1) {
                                printf("true");  
                              } else {
                                printf("false");  
                              };
                              break;
      };
      case N_INTEGER : {
                              printf("%d", int_value(node));
                              break;
      };
      case N_DOUBLE  : {
                              printf("%f", double_value(node));
                              break;
      };
      case N_STRING_1: {
                              printf("%s", string_value(node));
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
                              print_arg_list(l); 
                              break;
      };
      case N_OP_MUL : {
                              print_ast(node->left);
                              printf(" * ");
                              print_ast(node->right);
                              break;
      };
      case N_OP_DIV : {
                              print_ast(node->left);
                              printf(" / ");
                              print_ast(node->right);
                              break;
      };
      case N_OP_MODULO : {
                              print_ast(node->left);
                              printf(" mod ");
                              print_ast(node->right);
                              break;
      };
      case N_OP_PLUS : { 
                              print_ast(node->left);
                              printf(" + ");
                              print_ast(node->right);
                              break;
      };
      case N_OP_MINUS : {
                              print_ast(node->left);
                              printf(" - ");
                              print_ast(node->right);
                              break;
      };
      case N_OP_CMP_GT : {
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
      case N_OP_CMP_LE : {
                              print_ast(node->left);
                              printf(" < ");
                              print_ast(node->right);
                              break;
      };
      case N_OP_CMP_LE_EQ : {
                              print_ast(node->left);
                              printf(" <= ");
                              print_ast(node->right);
                              break;
      };
      case N_OP_CMP_EQ : {
                              print_ast(node->left);
                              printf(" == ");
                              print_ast(node->right);
                              break;
      };
      case N_OP_CMP_EQ_EQ : {
                              print_ast(node->left);
                              printf(" === ");
                              print_ast(node->right);
                              break;
      };
      case N_OP_CMP_INEQ : {
                              print_ast(node->left);
                              printf(" <=> ");
                              print_ast(node->right);
                              break;
      };
      case N_OP_CMP_NEG : {
                              print_ast(node->left);
                              printf(" != ");
                              print_ast(node->right);
                              break;
      };
      case N_OP_CMP_AND : {
                              print_ast(node->left);
                              printf(" && ");
                              print_ast(node->right);
                              break;
      };
      case N_OP_CMP_OR : {
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
                              printf("def %s", f->name); // def function name
                              print_arg_list(f->args); // function parameters
                              print_ast(f->stmts); // comp_statements
                              printf("end");
                              break;
      };
      case N_RETURN : {
                              printf("return ");
                              print_ast(node->left); // expression
                              printf("\n");
                              break;
      };
      case N_WHILE : {
                              printf("while ");
                              print_ast(node->left); // expression
                              printf("\n");
                              print_ast(node->right); // comp_statements
                              printf("\nend");
                              break;
      };
      case N_CLASS : {
                              struct class_node* c = (struct class_node*)node;
                              printf("class %s\n", c->name); // class name
                              print_ast(c->stmts); // comp_statements
                              printf("end");
                              break;
      };
      case N_METHOD_CALL_2 : { 
                              struct method_call_node* m = (struct method_call_node*) node;
                              printf("%s ", m->method_name);
                              print_arg_list(m->args);
                              break;
      };
      default : {
                              printf("%i\n", node->node_type);
                              printf("ERROR: when printing %c.\n", node->node_type);
      };
    };
  };
};

