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
// aux functions /////////////////////////////////////

void print_arg_list(struct arg_list_node* args) {
  printf("(");
  struct arg_list_node* aux = args;
  while (aux != NULL) {
    print_ast(aux->arg);
    aux = aux->next;
    if (aux != NULL) { printf(", "); };
  };
  printf(")\n");
}; 

char* drop_quotes(char* str) {
  char* res = malloc((sizeof(str)+1)*sizeof(char));
  strcpy(res, str); 
  res[strlen(res) - 1] = '\0'; /* elimino comilla final */
  return res + 1; /* elimino comilla inicial */
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

//
//
//
// basic functions ///////////////////////////////////

struct ast* eval_ast(struct ast* node) {

  if (node != NULL) {
    switch(node->node_type) {
      case N_NIL : {
                              return new_nil_node();
      };
      case N_INTEGER : {
                              struct integer_node* i = (struct integer_node*) node;
                              return new_integer_node(i->value);
      };
      case N_DOUBLE : {
                              struct double_node* d = (struct double_node*) node;
                              return new_double_node(d->value);
      };
      case N_STRING_1: {
                              struct string_node* s = (struct string_node*) node;
                              return new_string_node(s->value);
      };
/*      case N_IDENTIFIER : {
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
      }; */
      case N_OP_PLUS : {
                              struct ast* left = eval_ast(node->left);
                              struct ast* right = eval_ast(node->right);

                              // int + int
                              if (left->node_type == N_INTEGER && right->node_type == N_INTEGER) {
                                return new_integer_node(int_value(left) + int_value(right));

                              // double + double
                              } else if (left->node_type == N_DOUBLE && right->node_type == N_DOUBLE) {
                                return new_double_node(double_value(left) + double_value(right));

                              // double + int || int + double
                              } else if ((left->node_type == N_DOUBLE && right->node_type == N_INTEGER) ||
                                         (left->node_type == N_INTEGER && right->node_type == N_DOUBLE)) {
                                return new_double_node(double_value(left) + double_value(right));

                              // string + string
                              } else if (left->node_type == N_STRING_1 && right->node_type == N_STRING_1) {
                                char* s_left = string_value(left);
                                char* s_right = string_value(right);
                                char* res = malloc((sizeof(s_left) + sizeof(s_right) + 1)*sizeof(char));
                                strcpy(res, s_left);
                                strcat(res, s_right);
                                return new_string_node(res);

                              // TODO manejar mejor casteo de tipos
                              } else if (left->node_type != N_INTEGER &&
                                         left->node_type != N_DOUBLE  &&
                                         left->node_type != N_STRING_1) {
                                no_method_error("+", left);

                              } else {          
                                type_error(left->node_type, right->node_type);
                              };
                              break;
      };
      case N_OP_MINUS : {
                              struct ast* left = eval_ast(node->left);
                              struct ast* right = eval_ast(node->right);

                              // int - int
                              if (left->node_type == N_INTEGER && right->node_type == N_INTEGER) {
                                return new_integer_node(int_value(left) - int_value(right));

                              // double - double
                              } else if (left->node_type == N_DOUBLE && right->node_type == N_DOUBLE) {
                                return new_double_node(double_value(left) - double_value(right));

                              // double - int || int - double
                              } else if ((left->node_type == N_DOUBLE && right->node_type == N_INTEGER) ||
                                         (left->node_type == N_INTEGER && right->node_type == N_DOUBLE)) {
                                return new_double_node(double_value(left) - double_value(right));

                              // TODO manejar mejor casteo de tipos
                              } else if (left->node_type != N_INTEGER &&
                                         left->node_type != N_DOUBLE ) {
                                no_method_error("-", left);
                                
                              } else {          
                                type_error(left->node_type, right->node_type);
                              };
                              break;
      }; /*
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
      }; */
      case N_STMT_LIST : {
                              return new_ast_node(N_STMT_LIST, eval_ast(node->left), eval_ast(node->right));
      };
/*      case N_FUNCTION : {
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
                              // por ahora solo comportamiento puts
                              struct method_call_node* m = (struct method_call_node*)node;
                              print_ast(eval_ast(m->args->arg));
                              break;
      }; */
      default : {
                              printf("ERROR: when evaluating %c.\n", node->node_type);
      };
    };
  } else {
    return NULL;
  };
};

void free_ast(struct ast* node) {
};

void print_ast(struct ast* node) {
  if (node != NULL) {
    switch(node->node_type) {
      case N_NIL : {
                              printf("nil");
                              break;
      };
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
                              printf("\n");
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

 
