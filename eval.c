#include "eval.h"

//
//
// aux functions /////////////////////////////////////

char* concat_strings(char* str1, char* str2) {
  char* res = malloc((strlen(str1) + strlen(str2) + 1)*sizeof(char));
  strcpy(res, str1);
  strcat(res, str2);
  return res;
};

char* drop_colon(char* str) {
  char* new_str = malloc((strlen(str)+1)*sizeof(char));
  strcpy(new_str, str);
  return new_str + 1;
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

void push_object_info(struct scope* scope, struct object_node * object ){
  put_object_sym_list_in_scope(scope, object);
  put_class_sym_list_in_scope(scope, object->class_ptr);
}

void push_method_info(struct scope* scope, struct list_node* fn_args, struct list_node* call_args){
  int i = list_length(fn_args);
  int j = list_length(call_args);
  if (i == j) {
    while (fn_args != NULL) { // paso parámetros
        struct ast* fn_arg = fn_args->arg;
        struct ast* call_arg = call_args->arg;

        struct identifier_node* i = (struct identifier_node*) fn_arg;
        put_sym_for_scope(scope, SYM_VAR, i->name, eval_ast(call_arg), NULL);

        fn_args = fn_args->next;
        call_args = call_args->next;
      };
  } else{
    wrong_arguments_error(i, j);
  }
}

void eval_and_push_args_and_object_info(struct list_node* fn_args, struct list_node* call_args, 
                                        struct object_node * object){
  struct scope* aux_scope = build_scope();
  push_object_info(aux_scope, object);
  push_method_info(aux_scope, fn_args, call_args);
  push_scope_on_copy(aux_scope);
};

void eval_end_push_args(struct list_node* fn_args, struct list_node* call_args) {
  int i = list_length(fn_args);
  int j = list_length(call_args);
  struct scope* aux_scope = build_scope();
  if (i == j) {
    while (fn_args != NULL) { // paso parámetros
      struct ast* fn_arg = fn_args->arg;
      struct ast* call_arg = call_args->arg;

      struct identifier_node* i = (struct identifier_node*) fn_arg;
      put_sym_for_scope(aux_scope, SYM_VAR, i->name, eval_ast(call_arg), NULL);

      fn_args = fn_args->next;
      call_args = call_args->next;
    };
    push_scope_on_copy(aux_scope);
  } else {
    wrong_arguments_error(i, j);
  };
};

int eval_cond(struct ast* cond) {
  return (!((cond->node_type == N_NIL) || 
           ((cond->node_type == N_BOOL) && (((struct bool_node*) cond)->value == 0))));
};

int array_tree_size(struct ast* array) {
  if (array == NULL) {
    return 0;
  } else {
    return (1 + array_tree_size(array->right));
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
                              return new_string_one_node(string_value(node));
                              break;
      };
      case N_STRING_2: {
                              return new_string_two_node(string_value(node));
                              break;
      };
      case N_IDENTIFIER : {
                              struct identifier_node* i = (struct identifier_node*) node;
                              if (is_native_method(node)){
                                return eval_native_method(node);
                              } else {
                                struct sym* sym = get_sym(SYM_VAR, i->name); // busco variable primero
                                if (sym != NULL) {
                                  return eval_ast(sym->ast); // retorno variable
                                } else {
                                  sym = get_sym(SYM_FUNC, i->name); // busco función
                                  if (sym != NULL) {
                                    push_scope(); // pusheo nuevo scope
                                    // no pasamos parámetros porque estamos en nodo identifier
                                    struct ast* evaluated = eval_ast(sym->ast); // retorno función evaluada
                                    pop_scope(); // pop del scope pusheado
                                    return evaluated;
                                  } else {
                                    undefined_variable_error(i->name);
                                  };
                                };
                              };
                              break;
      };
      case N_OP_EQUAL : {
                              struct identifier_node* left = (struct identifier_node*) node->left;
                              struct ast* expression = NULL;
                              put_sym(SYM_VAR, left->name, expression = eval_ast(node->right), NULL);
                              return expression;
                              break;
      };
      case N_OP_PLUS_EQ : {
                              struct identifier_node* id_node = (struct identifier_node*)node->left;
                              struct sym* s = get_sym(SYM_VAR, id_node->name);

                              if (s != NULL) {
                                struct ast* left = s->ast;
                                struct ast* right = eval_ast(node->right);

                                // int += int
                                if (left->node_type == N_INTEGER && right->node_type == N_INTEGER) {
                                  s->ast = new_integer_node(int_value(left) + int_value(right));

                                // double += double
                                } else if (left->node_type == N_DOUBLE && right->node_type == N_DOUBLE) {
                                  s->ast = new_double_node(double_value(left) + double_value(right));

                                // double += double || double += int || int += double
                                } else if ((left->node_type == N_DOUBLE && right->node_type == N_DOUBLE)  ||
                                           (left->node_type == N_DOUBLE && right->node_type == N_INTEGER) ||
                                           (left->node_type == N_INTEGER && right->node_type == N_DOUBLE)) {
                                  s->ast = new_double_node(double_value(left) + double_value(right));

                                // string += string
                                } else if (left->node_type == N_STRING_1 && right->node_type == N_STRING_1 ||
                                           left->node_type == N_STRING_2 && right->node_type == N_STRING_1 ||
                                           left->node_type == N_STRING_1 && right->node_type == N_STRING_2 ||
                                           left->node_type == N_STRING_2 && right->node_type == N_STRING_2) {
                                  char* s_left = string_value(left);
                                  char* s_right = string_value(right);
                                  char* res = malloc((strlen(s_left) + strlen(s_right) + 1)*sizeof(char));
                                  strcpy(res, s_left);
                                  strcat(res, s_right);
                                  s->ast = new_string_one_node(res);
                                } else if (left->node_type != N_INTEGER  &&
                                           left->node_type != N_DOUBLE   &&
                                           left->node_type != N_STRING_1 &&
                                           left->node_type != N_STRING_2) {
                                  no_method_error("+=", left);

                                } else {
                                  type_error(left->node_type, right->node_type);
                                };
                              } else {
                                undefined_variable_error(id_node->name);
                              };
                              break;
      };
      case N_OP_MINUS_EQ : {
                              struct identifier_node* id_node = (struct identifier_node*)node->left;
                              struct sym* s = get_sym(SYM_VAR, id_node->name);

                              if (s != NULL) {
                                struct ast* left = s->ast;
                                struct ast* right = eval_ast(node->right);

                                // int -= int
                                if (left->node_type == N_INTEGER && right->node_type == N_INTEGER) {
                                  s->ast = new_integer_node(int_value(left) - int_value(right));

                                // double -= double || double -= int || int -= double
                                } else if ((left->node_type == N_DOUBLE && right->node_type == N_DOUBLE)  ||
                                           (left->node_type == N_DOUBLE && right->node_type == N_INTEGER) ||
                                           (left->node_type == N_INTEGER && right->node_type == N_DOUBLE)) {
                                  s->ast = new_double_node(double_value(left) - double_value(right));
                                } else if (left->node_type != N_INTEGER &&
                                           left->node_type != N_DOUBLE ) {
                                  no_method_error("-=", left);

                                } else {
                                  type_error(left->node_type, right->node_type);
                                };
                              } else {
                                undefined_variable_error(id_node->name);
                              };
                              break;
      };
      case N_OP_MUL_EQ : {
                              struct identifier_node* id_node = (struct identifier_node*)node->left;
                              struct sym* s = get_sym(SYM_VAR, id_node->name);

                              if (s != NULL) {
                                struct ast* left = s->ast;
                                struct ast* right = eval_ast(node->right);

                                // int *= int
                                if (left->node_type == N_INTEGER && right->node_type == N_INTEGER) {
                                  s->ast = new_integer_node(int_value(left) * int_value(right));

                                // double *= int || int *= double
                                } else if ((left->node_type == N_DOUBLE && right->node_type == N_DOUBLE)  ||
                                           (left->node_type == N_DOUBLE && right->node_type == N_INTEGER) ||
                                           (left->node_type == N_INTEGER && right->node_type == N_DOUBLE)) {
                                  s->ast = new_double_node(double_value(left) * double_value(right));

                                // string *= int
                                } else if (left->node_type == N_STRING_1 && right->node_type == N_INTEGER ||
                                           left->node_type == N_STRING_2 && right->node_type == N_INTEGER) {
                                  char* s_left = string_value(left);
                                  int n = int_value(right);
                                  char* res = malloc((strlen(s_left)*n + 1)*sizeof(char));
                                  res = string_repeat(n, s_left);
                                  s->ast = new_string_one_node(res);

                                // int *= string
                                } else if (left->node_type == N_INTEGER && right->node_type == N_STRING_1 ||
                                           left->node_type == N_INTEGER && right->node_type == N_STRING_2) {
                                  char* s_right = string_value(right);
                                  int n = int_value(left);
                                  char* res = malloc((strlen(s_right)*n + 1)*sizeof(char));
                                  res = string_repeat(n, s_right);
                                  s->ast = new_string_one_node(res);
                                } else if (left->node_type != N_INTEGER  &&
                                           left->node_type != N_DOUBLE   &&
                                           left->node_type != N_STRING_1 &&
                                           left->node_type != N_STRING_2) {
                                  no_method_error("*=", left);

                                } else {
                                  type_error(N_INTEGER, left->node_type);
                                };
                              } else {
                                undefined_variable_error(id_node->name);
                              };

                              break;
      };
      case N_OP_DIV_EQ : {
                              struct identifier_node* id_node = (struct identifier_node*)node->left;
                              struct sym* s = get_sym(SYM_VAR, id_node->name);

                              if (s != NULL) {
                                struct ast* left = s->ast;
                                struct ast* right = eval_ast(node->right);

                                // int /= int
                                if (left->node_type == N_INTEGER && right->node_type == N_INTEGER) {
                                  if (int_value(right) != 0) {
                                    s->ast = new_integer_node(int_value(left) / int_value(right));
                                  } else {
                                    /* ERROR !!!! */
                                  }

                                // double /= double || double /= int || int /= double
                                } else if ((left->node_type == N_DOUBLE && right->node_type == N_DOUBLE)  ||
                                           (left->node_type == N_DOUBLE && right->node_type == N_INTEGER) ||
                                           (left->node_type == N_INTEGER && right->node_type == N_DOUBLE)) {
                                  if (double_value(right) != 0) {
                                    s->ast = new_double_node(double_value(left) / double_value(right));
                                  } else {
                                    /* ERROR !!! */
                                  }

                                } else if (left->node_type != N_INTEGER &&
                                           left->node_type != N_DOUBLE) {
                                  no_method_error("/=", left);
                                } else {
                                  type_error(left->node_type, right->node_type);
                                };
                              } else {
                                undefined_variable_error(id_node->name);
                              };
                              break;
      };
      case N_OP_MODULO_EQ : {
                              struct identifier_node* id_node = (struct identifier_node*)node->left;
                              struct sym* s = get_sym(SYM_VAR, id_node->name);

                              if (s != NULL) {
                              } else {
                                undefined_variable_error(id_node->name);
                              };
                              break;
      };
/*      case N_ARG_LIST : {
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
                              } else if (left->node_type == N_STRING_1 && right->node_type == N_INTEGER ||
                                         left->node_type == N_STRING_2 && right->node_type == N_INTEGER) {
                                char* s_left = string_value(left);
                                int n = int_value(right);
                                char* res = malloc((strlen(s_left)*n + 1)*sizeof(char));
                                res = string_repeat(n, s_left);
                                return new_string_one_node(res);

                              // int * string
                              } else if (left->node_type == N_INTEGER && right->node_type == N_STRING_1 ||
                                         left->node_type == N_INTEGER && right->node_type == N_STRING_2) {
                                char* s_right = string_value(right);
                                int n = int_value(left);
                                char* res = malloc((strlen(s_right)*n + 1)*sizeof(char));
                                res = string_repeat(n, s_right);
                                return new_string_one_node(res);

                              } else if (left->node_type != N_INTEGER  &&
                                         left->node_type != N_DOUBLE   &&
                                         left->node_type != N_STRING_1 &&
                                         left->node_type != N_STRING_2) {
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
                                  zero_division_error();
                                }

                              // double / double || double / int || int / double
                              } else if ((left->node_type == N_DOUBLE && right->node_type == N_DOUBLE)  ||
                                         (left->node_type == N_DOUBLE && right->node_type == N_INTEGER) ||
                                         (left->node_type == N_INTEGER && right->node_type == N_DOUBLE)) {
                                if (double_value(right) != 0) {
                                  return new_double_node(double_value(left) / double_value(right));
                                } else {
                                  zero_division_error();
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
                              break;
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
                              } else if (left->node_type == N_STRING_1 && right->node_type == N_STRING_1 ||
                                         left->node_type == N_STRING_2 && right->node_type == N_STRING_1 ||
                                         left->node_type == N_STRING_1 && right->node_type == N_STRING_2 ||
                                         left->node_type == N_STRING_2 && right->node_type == N_STRING_2) {
                                char* s_left = string_value(left);
                                char* s_right = string_value(right);
                                char* res = malloc((strlen(s_left) + strlen(s_right) + 1)*sizeof(char));
                                strcpy(res, s_left);
                                strcat(res, s_right);
                                return new_string_one_node(res);

                              // TODO manejar mejor casteo de tipos
                              // string + int   
                              } else if (left->node_type == N_STRING_1 && right->node_type == N_INTEGER ||
                                         left->node_type == N_STRING_2 && right->node_type == N_INTEGER) {
                                char* s_left  = string_value(left);
                                int i = int_value(right);
                                char* s_int = malloc(sizeof(i)/sizeof(int));
                                sprintf(s_int, "%i", i);
                                char* res = malloc((strlen(s_left) + strlen(s_int) + 1)*sizeof(char));
                                strcpy(res, s_left);
                                strcat(res, s_int);
                                return new_string_one_node(res);
                              // int + string
                              } else if (left->node_type == N_INTEGER && right->node_type == N_STRING_1 ||
                                         left->node_type == N_INTEGER && right->node_type == N_STRING_2) {
                                int i = int_value(left);
                                char* s_int = malloc(sizeof(i)/sizeof(int));
                                sprintf(s_int, "%i", i);
                                char* s_right  = string_value(right);
                                char* res = malloc((strlen(s_right) + strlen(s_int) + 1)*sizeof(char));
                                strcpy(res, s_int);
                                strcat(res, s_right);
                                return new_string_one_node(res);

                              } else if (left->node_type != N_INTEGER  &&
                                         left->node_type != N_DOUBLE   &&
                                         left->node_type != N_STRING_1 &&
                                         left->node_type != N_STRING_2) {
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
                              } else if (left->node_type == N_STRING_1 && right->node_type == N_STRING_1 ||
                                         left->node_type == N_STRING_2 && right->node_type == N_STRING_1 ||
                                         left->node_type == N_STRING_1 && right->node_type == N_STRING_2 ||
                                         left->node_type == N_STRING_2 && right->node_type == N_STRING_2) {
                                int value = (strcmp(string_value(left), string_value(right)) > 0) ? 1 : 0;
                                return new_bool_node(value);

                              } else if (left->node_type != N_INTEGER  &&
                                         left->node_type != N_DOUBLE   &&
                                         left->node_type != N_STRING_1 &&
                                         left->node_type != N_STRING_2) {
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
                              } else if (left->node_type == N_STRING_1 && right->node_type == N_STRING_1 ||
                                         left->node_type == N_STRING_2 && right->node_type == N_STRING_1 ||
                                         left->node_type == N_STRING_1 && right->node_type == N_STRING_2 ||
                                         left->node_type == N_STRING_2 && right->node_type == N_STRING_2) {
                                int value = (strcmp(string_value(left), string_value(right)) >= 0) ? 1 : 0;
                                return new_bool_node(value);

                              } else if (left->node_type != N_INTEGER  &&
                                         left->node_type != N_DOUBLE   &&
                                         left->node_type != N_STRING_1 &&
                                         left->node_type != N_STRING_2) {
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
                              } else if (left->node_type == N_STRING_1 && right->node_type == N_STRING_1 ||
                                         left->node_type == N_STRING_2 && right->node_type == N_STRING_1 ||
                                         left->node_type == N_STRING_1 && right->node_type == N_STRING_2 ||
                                         left->node_type == N_STRING_2 && right->node_type == N_STRING_2) {
                                int value = (strcmp(string_value(left), string_value(right)) < 0) ? 1 : 0;
                                return new_bool_node(value);

                              } else if (left->node_type != N_INTEGER  &&
                                         left->node_type != N_DOUBLE   &&
                                         left->node_type != N_STRING_1 &&
                                         left->node_type != N_STRING_2) {
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
                              } else if (left->node_type == N_STRING_1 && right->node_type == N_STRING_1 ||
                                         left->node_type == N_STRING_2 && right->node_type == N_STRING_1 ||
                                         left->node_type == N_STRING_1 && right->node_type == N_STRING_2 ||
                                         left->node_type == N_STRING_2 && right->node_type == N_STRING_2) {
                                int value = (strcmp(string_value(left), string_value(right)) <= 0) ? 1 : 0;
                                return new_bool_node(value);

                              } else if (left->node_type != N_INTEGER  &&
                                         left->node_type != N_DOUBLE   &&
                                         left->node_type != N_STRING_1 &&
                                         left->node_type != N_STRING_2) {
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
                              } else if (left->node_type == N_STRING_1 && right->node_type == N_STRING_1 ||
                                         left->node_type == N_STRING_2 && right->node_type == N_STRING_1 ||
                                         left->node_type == N_STRING_1 && right->node_type == N_STRING_2 ||
                                         left->node_type == N_STRING_2 && right->node_type == N_STRING_2) {
                                int value = (strcmp(string_value(left), string_value(right)) == 0) ? 1 : 0;
                                return new_bool_node(value);

                              // nil
                              } else if (left->node_type == N_NIL && right->node_type == N_NIL) {
                                return new_bool_node(1);

                              // boolean
                              } else if (left->node_type == N_BOOL && right->node_type == N_BOOL) {
                                int value = (bool_value(left) == bool_value(right));
                                return new_bool_node(value);

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

                              if (left != NULL && right !=NULL){
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
                                } else if (left->node_type == N_STRING_1 && right->node_type == N_STRING_1 ||
                                           left->node_type == N_STRING_2 && right->node_type == N_STRING_1 ||
                                           left->node_type == N_STRING_1 && right->node_type == N_STRING_2 ||
                                           left->node_type == N_STRING_2 && right->node_type == N_STRING_2) {
                                  int cmp = strcmp(string_value(left), string_value(right));
                                  cmp = cmp > 0 ?  1 : cmp;
                                  cmp = cmp < 0 ? -1 : cmp;
                                  return new_integer_node(cmp);
                                } else if (left->node_type == N_NIL || left->node_type == N_BOOL) {
                                  no_method_error("<=>", left);
                                } else if (right->node_type == N_NIL || right->node_type == N_BOOL){
                                  no_method_error("<=>", right);
                                } else{
                                  return new_nil_node();
                                };
                              } else {
                                no_method_error("<=>", NULL);
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
                              } else if (left->node_type == N_STRING_1 && right->node_type == N_STRING_1 ||
                                         left->node_type == N_STRING_2 && right->node_type == N_STRING_1 ||
                                         left->node_type == N_STRING_1 && right->node_type == N_STRING_2 ||
                                         left->node_type == N_STRING_2 && right->node_type == N_STRING_2) {
                                int value = (strcmp(string_value(left), string_value(right)) == 0) ? 0 : 1;
                                return new_bool_node(value);

                              // nil
                              } else if (left->node_type == N_NIL && right->node_type == N_NIL) {
                                return new_bool_node(0);

                              // boolean
                              } else if (left->node_type == N_BOOL && right->node_type == N_BOOL) {
                                int value = (bool_value(left) != bool_value(right));
                                return new_bool_node(value);

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
                              struct ast* eval_right = eval_ast(node->right);
                              struct ast* eval_left = eval_ast(node->left);
                              return eval_left;
                              break;
      };
      case N_FUNCTION : {
                              struct function_node* f = (struct function_node*) node;

                              // checkeo que todos los parámetros sean identifiers
                              struct list_node* fn_args = f->args;
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
      case N_RETURN : {
                              //return eval_ast(node->left);
                              //break;
      };
      case N_IF : {
                              struct if_node* i = (struct if_node*) node;

                              if (eval_cond(eval_ast(i->condition)) == 1) {
                                return eval_ast(i->th);
                              } else {
                                return eval_ast(i->el);
                              };
                              break;
      };
      case N_WHILE : {
                              while (eval_cond(eval_ast(node->left)) == 1) {
                                eval_ast(node->right);
                              };
                              break;
      };
      case N_CLASS : {
                              struct class_node* c = (struct class_node*) node;
                              push_scope();

                              struct list_node* s;
                              for (s = c->stmts; s != NULL; s = s->next) {
                                eval_ast(s->arg);
                              };

                              pop_scope_and_define_class(c->name);
                              //print_class_table();
                              break;
      };
      case N_METHOD_CALL_0 : {
                              struct method_call_node* m = (struct method_call_node*) node;

                              // comportamiento para métodos de clase
                              if (class_exists(string_value(m->left_ast)) == 1) {

                                // .new por ejemplo
                                if (is_native_method(node)) {
                                  return eval_class_native_method(m);

                                // otra llamada
                                } else {
                                    undefined_method_error(string_value(m->left_ast), m->method_name);
                                };
                              // si no existe la clase, error
                              } else {
                                uninitialized_constant_error(string_value(m->left_ast));
                              };
                              break;
      }; 
      case N_METHOD_CALL_1 : {
                               struct method_call_node* m = (struct method_call_node*)node;
                               if (is_native_method(node)) {
                                 return eval_instance_native_method(node);

                               // si es metodo de objeto  
                               } else {
                                 struct sym* sym = get_sym(SYM_VAR, string_value(m->left_ast)); // busco variable
                                 if (sym != NULL) {

                                  // debe ser un objeto
                                  if (sym->ast->node_type == N_OBJECT) {
                                    struct object_node* o = (struct object_node*) sym->ast;
                                    struct sym* method = find_method_for_class(o->class_ptr->name, m->method_name); // busco método

                                    if (method != NULL) {

                                      eval_and_push_args_and_object_info(method->args, m->args, o);
                                      //print_sym_table();
                                      //print_sym_list_for_object(o);

                                      struct ast* eval = eval_ast(method->ast);
                                      pop_scope(); // pop del scope pusheado
                                      return eval; // retorno función evaluada

                                      print_ast(sym->ast);
                                    } else {
                                      undefined_method_error(o->class_ptr->name, m->method_name);  
                                    };
                                    

                                  } else {
                                    undefined_method_error(string_value(m->left_ast), m->method_name);
                                  };

                                 } else {
                                   undefined_variable_error(string_value(m->left_ast));
                                 }; 
                               };
                              break;
      };
      case N_METHOD_CALL_2 : {
                              struct method_call_node* m = (struct method_call_node*) node;
                              if (is_native_method(node)){
                                return eval_native_method(node);
                              } else {
                                struct sym* sym = get_sym(SYM_FUNC, m->method_name); // busco función
                                if (sym != NULL) {
                                  eval_end_push_args(sym->args, m->args);
                                  struct ast* eval = eval_ast(sym->ast);
                                  pop_scope(); // pop del scope pusheado
                                  return eval; // retorno función evaluada
                                } else {
                                  undefined_variable_error(m->method_name);
                                };
                              };
                              break;
      };
      case N_ATTR_ACCESSOR : {
                              struct ast* sym_list;
                              for (sym_list = node->left; sym_list != NULL; sym_list = sym_list->right) {
                                // obtengo symbolo
                                struct symbol_node* s = (struct symbol_node*) sym_list->left;

                                // creo variable de instancia
                                char* sym_name = drop_colon(s->name);
                                char* at_name = concat_strings("@", sym_name);
                                put_sym(SYM_INST_VAR, at_name, new_nil_node(), NULL);

                                // creo getter
                                put_sym(SYM_FUNC, sym_name, new_identifier_node(at_name), NULL);

                                // creo setter
                                struct ast* assign = new_ast_node(N_OP_EQUAL, new_identifier_node(at_name), new_identifier_node("arg"));
                                struct list_node* param = new_list_node(N_ARG_LIST, new_identifier_node("arg"), NULL);
                                put_sym(SYM_FUNC, concat_strings(sym_name, "="), assign, param);
                              };
                              break;
      };
      case N_ATTR_READER : {
                              struct ast* sym_list; 
                              for (sym_list = node->left; sym_list != NULL; sym_list = sym_list->right) {
                                // obtengo symbolo 
                                struct symbol_node* s = (struct symbol_node*) sym_list->left;

                                // creo variable de instancia
                                char* sym_name = drop_colon(s->name);
                                char* at_name = concat_strings("@", sym_name);
                                put_sym(SYM_INST_VAR, at_name, new_nil_node(), NULL);

                                // creo getter
                                put_sym(SYM_FUNC, sym_name, new_identifier_node(at_name), NULL);
                              };
                              break;
      };
      case N_ATTR_WRITTER : {
                              struct ast* sym_list;
                              for (sym_list = node->left; sym_list != NULL; sym_list = sym_list->right) {
                                // obtengo symbolo
                                struct symbol_node* s = (struct symbol_node*) sym_list->left;

                                // creo variable de instancia
                                char* sym_name = drop_colon(s->name);
                                char* at_name = concat_strings("@", sym_name);
                                put_sym(SYM_INST_VAR, at_name, new_nil_node(), NULL);

                                // creo setter
                                struct ast* assign = new_ast_node(N_OP_EQUAL, new_identifier_node(at_name), new_identifier_node("arg"));
                                struct list_node* param = new_list_node(N_ARG_LIST, new_identifier_node("arg"), NULL);
                                put_sym(SYM_FUNC, concat_strings(sym_name, "="), assign, param);
                              };
                              break;
      };
      case N_ARRAY_CONTENT : {
                               return node;//new_ast_node(N_ARRAY_CONTENT, node, node->right);
                               break;
      };
      case N_ARRAY_ACCESS :  {
                               struct array_access_node* arr = (struct array_access_node*)node;
                               struct sym* sym = get_sym(SYM_VAR, arr->array_name);

                               if (sym != NULL) {
                                 int arr_size = array_tree_size(sym->ast);
                                 struct ast* result[arr_size];

                                 struct ast* ptr = sym->ast;
                                 int i;
                                 for (i = 0; i < arr_size; i++) {
                                   result[i] = ptr;
                                   ptr = ptr->right;
                                 };

                                 int index = arr_size - arr->index - 1;
                                 if (index >= 0 && index < arr_size) {
                                   struct ast* element = eval_ast(result[index]);
                                   return element;
                                 } else {
                                   /* ERROR !!! */
                                   /* printf("nil\n");*/
                                   return new_nil_node();
                                 };
                               };
                               break;
      };
      case N_OBJECT : {
                               return node;
                               break;
      }
      default : {
                              printf("ERROR: when evaluating %d.\n", node->node_type);
      };
    };
  } else {
    return NULL;
  };
};
