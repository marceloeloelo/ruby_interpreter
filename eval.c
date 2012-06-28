
#include "eval.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

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
  char* res = malloc((strlen(str)+1)*sizeof(char));
  strcpy(res, str);
  res[strlen(res) - 1] = '\0'; /* elimino comilla final */
  return res + 1; /* elimino comilla inicial */
};

char * string_repeat( int n, const char * s ) {
  size_t slen = strlen(s);
  char * dest = malloc(n*slen+1);

  int i; char * p;
  for ( i=0, p = dest; i < n; ++i, p += slen ) {
    memcpy(p, s, slen);
  }
  *p = '\0';
  return dest;
};

int arg_list_length(struct arg_list_node* arg_list) {
  int i = 0;
  while (arg_list != NULL) {
    i = i + 1;
    arg_list = arg_list->next;
  };
  return i;
};

void eval_end_push_args(struct arg_list_node* fn_args, struct arg_list_node* call_args) {
  int i = arg_list_length(fn_args);
  int j = arg_list_length(call_args);
  if (i == j) {
    while (fn_args != NULL) { // paso parámetros
      struct ast* fn_arg = fn_args->arg;
      struct ast* call_arg = call_args->arg;

      struct identifier_node* i = (struct identifier_node*) fn_arg;
      put_sym(SYM_VAR, i->name, eval_ast(call_arg), NULL);

      fn_args = fn_args->next;
      call_args = call_args->next;
    };
  } else {
    wrong_arguments_error(i, j);
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
                              break;
      };
      case N_BOOL : {
                              return new_bool_node(bool_value(node));
                              break;
      };
      case N_INTEGER : {
                              return new_integer_node(int_value(node));
                              break;
      };
      case N_DOUBLE : {
                              return new_double_node(double_value(node));
                              break;
      };
      case N_STRING_1: {
                              return new_string_node(string_value(node));
                              break;
      };
      case N_IDENTIFIER : {
                              struct identifier_node* i = (struct identifier_node*) node;
                              struct sym* sym = get_sym(SYM_VAR, i->name); // busco variable primero
                              if (sym != NULL) {
                                return eval_ast(sym->ast); // retorno variable
                              } else {
                                sym = get_sym(SYM_FUNC, i->name); // busco función
                                if (sym != NULL) {
                                  push_scope(); // pusheo nuevo scope
                                  // no pasamos parámetros porque estamos en nodo identifier
                                  return eval_ast(sym->ast); // retorno función evaluada
                                  pop_scope(); // pop del scope pusheado
                                } else {
                                  undefined_variable_error(i->name);
                                };  
                              };
                              break;
      };
      case N_OP_EQUAL : {
                              struct identifier_node* left = (struct identifier_node*) node->left;
                              put_sym(SYM_VAR, left->name, eval_ast(node->right), NULL);
                              break;
      };
/*      case N_OP_PLUS_EQ : {
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
      };*/
      case N_OP_MUL : {
                              struct ast* left = eval_ast(node->left);
                              struct ast* right = eval_ast(node->right);

                              // int * int
                              if (left->node_type == N_INTEGER && right->node_type == N_INTEGER) {
                                return new_integer_node(int_value(left) * int_value(right));

                              // double * int || int * double
                              } else if ((left->node_type == N_DOUBLE && right->node_type == N_DOUBLE)  ||
                                         (left->node_type == N_DOUBLE && right->node_type == N_INTEGER) ||
                                         (left->node_type == N_INTEGER && right->node_type == N_DOUBLE)) {
                                return new_double_node(double_value(left) * double_value(right));

                              // string * int
                              } else if (left->node_type == N_STRING_1 && right->node_type == N_INTEGER) {
                                char* s_left = string_value(left);
                                int n = int_value(right);
                                char* res = malloc((strlen(s_left)*n + 1)*sizeof(char));
                                res = string_repeat(n, s_left);
                                return new_string_node(res);

                              // int * string
                              } else if (left->node_type == N_INTEGER && right->node_type == N_STRING_1) {
                                char* s_right = string_value(right);
                                int n = int_value(left);
                                char* res = malloc((strlen(s_right)*n + 1)*sizeof(char));
                                res = string_repeat(n, s_right);
                                return new_string_node(res);

                              } else if (left->node_type != N_INTEGER &&
                                         left->node_type != N_DOUBLE  &&
                                         left->node_type != N_STRING_1) {
                                no_method_error("*", left);

                              } else {
                                type_error(N_INTEGER, left->node_type);
                              };
                              break;
      };
      case N_OP_DIV : {
                              struct ast* left = eval_ast(node->left);
                              struct ast* right = eval_ast(node->right);

                              // int / int
                              if (left->node_type == N_INTEGER && right->node_type == N_INTEGER) {
                                if (int_value(right) != 0) {
                                  return new_integer_node(int_value(left) / int_value(right));
                                } else {
                                  /* ERROR !!!! */
                                }

                              // double / double || double / int || int / double
                              } else if ((left->node_type == N_DOUBLE && right->node_type == N_DOUBLE)  ||
                                         (left->node_type == N_DOUBLE && right->node_type == N_INTEGER) ||
                                         (left->node_type == N_INTEGER && right->node_type == N_DOUBLE)) {
                                if (double_value(right) != 0) {
                                  return new_double_node(double_value(left) * double_value(right));
                                } else {
                                  /* ERROR !!! */
                                }

                              } else if (left->node_type != N_INTEGER &&
                                         left->node_type != N_DOUBLE) {
                                no_method_error("/", left);

                              } else {
                                type_error(left->node_type, right->node_type);
                              };
                              break;
      };
      case N_OP_MODULO : {
                              struct ast* left = eval_ast(node->left);
                              struct ast* right = eval_ast(node->right);

                              // int % int
                              if (left->node_type == N_INTEGER && right->node_type == N_INTEGER) {
                                if (int_value(right) != 0) {
                                  return new_integer_node((int_value(left) % int_value(right) + int_value(right)) % int_value(right));
                                } else {
                                  /* ERROR !!!! */
                                }

                              // double % double
                              /* } else if (left->node_type == N_DOUBLE && right->node_type == N_DOUBLE) {*/
                              /*   if (double_value(right) != 0) {*/
                              /*     return new_double_node((double_value(left) % double_value(right) + double_value(right)) % double_value(right));*/
                              /*   } else {*/
                              /*     [> ERROR !!! <]*/
                              /*   }*/
                              // double % int || int % double
                              /* } else if ((left->node_type == N_DOUBLE && right->node_type == N_INTEGER) ||*/
                              /*            (left->node_type == N_INTEGER && right->node_type == N_DOUBLE)) {*/
                              /*   if (double_value(right) != 0) {*/
                              /*     return new_double_node((double_value(left) % double_value(right) + double_value(right)) % double_value(right));*/
                              /*   } else {*/
                              /*     [> ERROR !!! <]*/
                              /*   }*/

                              } else if (left->node_type != N_INTEGER) {
                                no_method_error("mod", left);

                              } else {
                                type_error(left->node_type, right->node_type);
                              };
      };
      case N_OP_PLUS : {
                              struct ast* left = eval_ast(node->left);
                              struct ast* right = eval_ast(node->right);

                              // int + int
                              if (left->node_type == N_INTEGER && right->node_type == N_INTEGER) {
                                return new_integer_node(int_value(left) + int_value(right));

                              // double + double
                              } else if (left->node_type == N_DOUBLE && right->node_type == N_DOUBLE) {
                                return new_double_node(double_value(left) + double_value(right));

                              // double + double || double + int || int + double
                              } else if ((left->node_type == N_DOUBLE && right->node_type == N_DOUBLE)  ||
                                         (left->node_type == N_DOUBLE && right->node_type == N_INTEGER) ||
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

                              // double - double || double - int || int - double
                              } else if ((left->node_type == N_DOUBLE && right->node_type == N_DOUBLE)  ||
                                         (left->node_type == N_DOUBLE && right->node_type == N_INTEGER) ||
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
      };
      case N_OP_CMP_GT : {
                              struct ast* left = eval_ast(node->left);
                              struct ast* right = eval_ast(node->right);

                              // int > int || int > double || double > int || double > double
                              if ((left->node_type == N_INTEGER  || left->node_type == N_DOUBLE) &&
                                  (right->node_type == N_INTEGER || right->node_type == N_DOUBLE)) {
                                int value = (double_value(left) > double_value(right)) ? 1 : 0;
                                return new_bool_node(value);

                              // string > string
                              } else if (left->node_type == N_STRING_1 && right->node_type == N_STRING_1) {
                                int value = (strcmp(string_value(left), string_value(right)) > 0) ? 1 : 0;
                                return new_bool_node(value);

                              } else if (left->node_type != N_INTEGER &&
                                         left->node_type != N_DOUBLE  &&
                                         left->node_type != N_STRING_1) {
                                no_method_error(">", left);

                              } else {
                                type_error(left->node_type, right->node_type);
                              };

                              break;
      };
      case N_OP_CMP_GT_EQ : {
                              struct ast* left = eval_ast(node->left);
                              struct ast* right = eval_ast(node->right);

                              // int >= int || int >= double || double >= int || double >= double
                              if ((left->node_type == N_INTEGER  || left->node_type == N_DOUBLE) &&
                                  (right->node_type == N_INTEGER || right->node_type == N_DOUBLE)) {
                                int value = (double_value(left) >= double_value(right)) ? 1 : 0;
                                return new_bool_node(value);

                              // string >= string
                              } else if (left->node_type == N_STRING_1 && right->node_type == N_STRING_1) {
                                int value = (strcmp(string_value(left), string_value(right)) >= 0) ? 1 : 0;
                                return new_bool_node(value);

                              } else if (left->node_type != N_INTEGER &&
                                         left->node_type != N_DOUBLE  &&
                                         left->node_type != N_STRING_1) {
                                no_method_error(">=", left);

                              } else {
                                type_error(left->node_type, right->node_type);
                              };

                              break;
      };
      case N_OP_CMP_LE : {
                              struct ast* left = eval_ast(node->left);
                              struct ast* right = eval_ast(node->right);

                              // int < int || int < double || double < int || double < double
                              if ((left->node_type == N_INTEGER  || left->node_type == N_DOUBLE) &&
                                  (right->node_type == N_INTEGER || right->node_type == N_DOUBLE)) {
                                int value = (double_value(left) < double_value(right)) ? 1 : 0;
                                return new_bool_node(value);

                              // string < string
                              } else if (left->node_type == N_STRING_1 && right->node_type == N_STRING_1) {
                                int value = (strcmp(string_value(left), string_value(right)) < 0) ? 1 : 0;
                                return new_bool_node(value);

                              } else if (left->node_type != N_INTEGER &&
                                         left->node_type != N_DOUBLE  &&
                                         left->node_type != N_STRING_1) {
                                no_method_error("<", left);

                              } else {
                                type_error(left->node_type, right->node_type);
                              };

                              break;
      };
      case N_OP_CMP_LE_EQ : {
                              struct ast* left = eval_ast(node->left);
                              struct ast* right = eval_ast(node->right);

                              // int <= int || int <= double || double <= int || double <= double
                              if ((left->node_type == N_INTEGER  || left->node_type == N_DOUBLE) &&
                                  (right->node_type == N_INTEGER || right->node_type == N_DOUBLE)) {
                                int value = (double_value(left) <= double_value(right)) ? 1 : 0;
                                return new_bool_node(value);

                              // string <= string
                              } else if (left->node_type == N_STRING_1 && right->node_type == N_STRING_1) {
                                int value = (strcmp(string_value(left), string_value(right)) <= 0) ? 1 : 0;
                                return new_bool_node(value);

                              } else if (left->node_type != N_INTEGER &&
                                         left->node_type != N_DOUBLE  &&
                                         left->node_type != N_STRING_1) {
                                no_method_error("<=", left);

                              } else {
                                type_error(left->node_type, right->node_type);
                              };

                              break;
      };
     case N_OP_CMP_EQ : {
                              struct ast* left = eval_ast(node->left);
                              struct ast* right = eval_ast(node->right);

                              // int, double
                              if ((left->node_type == N_INTEGER  || left->node_type == N_DOUBLE) &&
                                  (right->node_type == N_INTEGER || right->node_type == N_DOUBLE)) {
                                int value = (double_value(left) == double_value(right)) ? 1 : 0;
                                return new_bool_node(value);

                              // string 
                              } else if (left->node_type == N_STRING_1 && right->node_type == N_STRING_1) {
                                int value = (strcmp(string_value(left), string_value(right)) == 0) ? 1 : 0;
                                return new_bool_node(value);

                              // nil
                              } else if (left->node_type == N_NIL && right->node_type == N_NIL) {
                                return new_bool_node(1);

                              } else {
                                return new_bool_node(0);
                              };

                              break;
       };
/*      case N_OP_CMP_EQ_EQ : {
                              print_ast(node->left);
                              printf(" === ");
                              print_ast(node->right);
                              break;
      }; */
      case N_OP_CMP_INEQ : {
                              struct ast* left = eval_ast(node->left);
                              struct ast* right = eval_ast(node->right);

                              // int, double
                              if ((left->node_type == N_INTEGER  || left->node_type == N_DOUBLE) &&
                                  (right->node_type == N_INTEGER || right->node_type == N_DOUBLE)) {
                                if (double_value(left) == double_value(right)) {
                                  return new_integer_node(0);    
                                } else if (double_value(left) > double_value(right)) {
                                  return new_integer_node(1);   
                                } else {
                                  return new_integer_node(-1);   
                                };

                              // string <= string
                              } else if (left->node_type == N_STRING_1 && right->node_type == N_STRING_1) {
                                return new_integer_node(strcmp(string_value(left), string_value(right)));

                              } else {
                                no_method_error("<=>", left);
                              };

                              break;
      };
      case N_OP_CMP_NEG : {
                              struct ast* left = eval_ast(node->left);
                              struct ast* right = eval_ast(node->right);

                              // int, double
                              if ((left->node_type == N_INTEGER  || left->node_type == N_DOUBLE) &&
                                  (right->node_type == N_INTEGER || right->node_type == N_DOUBLE)) {
                                int value = (double_value(left) == double_value(right)) ? 0 : 1;
                                return new_bool_node(value);

                              // string 
                              } else if (left->node_type == N_STRING_1 && right->node_type == N_STRING_1) {
                                int value = (strcmp(string_value(left), string_value(right)) == 0) ? 0 : 1;
                                return new_bool_node(value);

                              // nil
                              } else if (left->node_type == N_NIL && right->node_type == N_NIL) {
                                return new_bool_node(0);

                              } else {
                                return new_bool_node(1);
                              };

                              break;
      };
      case N_OP_CMP_AND : {
                              struct ast* left = eval_ast(node->left);
                              struct ast* right = eval_ast(node->right);
                              return new_bool_node(bool_value(left) && bool_value(right));
                              break;
      };
      case N_OP_CMP_OR : {
                              struct ast* left = eval_ast(node->left);
                              struct ast* right = eval_ast(node->right);
                              return new_bool_node(bool_value(left) || bool_value(right));
                              break;
      };
      case N_OP_PLUS_UN : {
                              struct ast* left = eval_ast(node->left);

                              // int
                              if (left->node_type == N_INTEGER) {
                                return new_integer_node(int_value(left));

                              // double 
                              } else if (left->node_type == N_DOUBLE) {
                                return new_double_node(double_value(left));

                              // TODO manejar mejor casteo de tipos
                              } else {
                                no_method_error("+", left);
                              };
                              break;
      };
      case N_OP_MINUS_UN : {
                              struct ast* left = eval_ast(node->left);

                              // int
                              if (left->node_type == N_INTEGER) {
                                return new_integer_node(int_value(left) * (-1));

                              // double 
                              } else if (left->node_type == N_DOUBLE) {
                                return new_double_node(double_value(left) * (-1));

                              // TODO manejar mejor casteo de tipos
                              } else {
                                no_method_error("-", left);
                              };
                              break;
      };
      case N_OP_NOT : {
                              struct ast* left = eval_ast(node->left);
                              return new_bool_node(!bool_value(left));
                              break;
      }; 
      case N_STMT_LIST : {
                              return new_ast_node(N_STMT_LIST, eval_ast(node->left), eval_ast(node->right));
      };
      case N_FUNCTION : {
                              struct function_node* f = (struct function_node*) node;

                              // checkeo que todos los parámetros sean identifiers
                              struct arg_list_node* fn_args = f->args;
                              while (fn_args != NULL) { // paso parámetros
                                struct ast* fn_arg = fn_args->arg;
                                if (fn_arg->node_type != N_IDENTIFIER) {
                                  unexpected_type_error(fn_arg->node_type);
                                };
                                fn_args = fn_args->next;
                              };

                              put_sym(SYM_FUNC, f->name, f->stmts, f->args);
                              break;
      };
/*      case N_RETURN : {
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
      }; */
      case N_METHOD_CALL_2 : { 
                              struct method_call_node* m = (struct method_call_node*) node;
                              struct sym* sym = get_sym(SYM_FUNC, m->method_name); // busco función
                              if (sym != NULL) {
                                push_scope(); // pusheo nuevo scope
                                eval_end_push_args(sym->args, m->args);
                                struct ast* eval = eval_ast(sym->ast);                                 
                                pop_scope(); // pop del scope pusheado
                                return eval; // retorno función evaluada
                              } else {
                                undefined_variable_error(m->method_name);
                              };  
                              break;
      }; 
      default : {
                              printf("ERROR: when evaluating %c.\n", node->node_type);
      };
    };
  } else {
    return NULL;
  };
};