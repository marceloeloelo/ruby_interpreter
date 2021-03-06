#include "native_methods.h"

char* native_methods[2] = { PUTS, GETS };
char* instance_native_methods[5] = { LENGTH, EACH_ITERATOR, RESPOND_TO, NIL_METHOD, OBJECT_ID };
char* class_native_methods[1] = {NEW};
// String method support
char* string_methods_array[4]  = { LENGTH, RESPOND_TO, NIL_METHOD, OBJECT_ID };
// Array method support
char* array_methods_array[4]   = { LENGTH, RESPOND_TO, NIL_METHOD, OBJECT_ID };
// Integer method support
char* integer_methods_array[3] = { RESPOND_TO, NIL_METHOD, OBJECT_ID };
// Bool method support
char* bool_methods_array[3]    = { RESPOND_TO, NIL_METHOD, OBJECT_ID };

struct ast* rputs(struct ast* a){

  if (a->node_type == N_METHOD_CALL_0 || a->node_type == N_METHOD_CALL_1 || a->node_type == N_METHOD_CALL_2) {
    struct method_call_node* m = (struct method_call_node*)a;
    struct list_node* arg_node = m->args;

    // caso especial: puts() sin parámetros imprime salto de línea
    if (arg_node == NULL) {
      printf("\n");

    // comportamiento normal
    } else {  
      while(arg_node != NULL){
        struct ast* evaluated = eval_ast(arg_node->arg);
        rputs(evaluated);
        arg_node = arg_node->next;
      };
    };

  } else if (a->node_type == N_ARRAY) {
    int arr_size = array_tree_size(a->left);
    struct ast* result[arr_size];
    struct ast* ptr = a->left;
    int i;
    for (i = 0; i < arr_size; i++) {
      result[i] = ptr;
      ptr = ptr->right;
    };

    ptr = result[arr_size-1];
    for (i = arr_size-1; i >= 0; i--) {
      rputs(eval_ast(result[i]));
    };

  } else if (a->node_type == N_ARRAY_CONTENT) {
    rputs(eval_ast(a->left));

  } else if (a->node_type == N_STRING_1) {
    printf("%s\n", string_value(a));

  } else if (a->node_type == N_STRING_2) {

    char * str = malloc(sizeof( strlen(string_value(a)) ));
    strcpy(str, string_value(a));
    str = build_end_of_lines(str);
    printf("%s\n", str);

  } else if (a->node_type == N_INTEGER) {
    printf("%d\n", int_value(a));

  } else if (a->node_type == N_DOUBLE) {
    double d = double_value(a);
    if ( d - floor(d) == 0.0 ) {
      printf( "%g.0\n", d );
    } else {
      printf( "%g\n", d );
    };

  } else if (a->node_type == N_BOOL) {
    printf("%s\n", bool_value(a) ? "true" : "false");

  } else if (a->node_type == N_NIL) {
    printf("\n");

  } else if (a->node_type == N_OBJECT) {
    struct object_node * object = (struct object_node *) a;
    printf("<#%s:%p>\n", object->class_ptr->name, (void *)object);

  } else {
    printf("Puts doesn't support %s type, sorry :D\n", type_name(a->node_type));
  };
  return new_nil_node();
};

struct ast* rgets() {
  char input_string[2056];
  gets(input_string);
  return new_string_one_node(strdup(input_string));
};

struct ast* rlength(struct ast* a) {
  switch (a->node_type) {
    case N_STRING_1: {
                           return new_integer_node(strlen(string_value(a)));
                           break;
    };
    case N_STRING_2: {
                           return new_integer_node(strlen(string_value(a)));
                           break;
    };
    case N_IDENTIFIER: {
                           return rlength(eval_ast(a));
                           break;
    };
    case N_ARRAY: {
                           return new_integer_node(array_tree_size(a->left));
                           break;
    };
    case N_ARRAY_CONTENT: {
                           return rlength(eval_ast(a->left));
                           break;
    };
     default: {
                          no_method_error("length", a);
		                      break;
		 };
  };
  return new_nil_node();
};

struct ast* rnil(struct method_call_node* m) {
  struct ast* evaluated = eval_ast(m->left_ast);
  return new_bool_node(evaluated->node_type == N_NIL);
};

struct ast* robject_id(struct method_call_node* m) {
  struct ast* evaluated = eval_ast(m->left_ast);
  return new_integer_node((long)evaluated);
};

struct ast* rrespond_to(struct ast* a, char* method_name){
  switch (a->node_type) {
    case N_STRING_1: {
                            return new_bool_node(string_is_in_array((void*)string_methods_array, method_name));
    };
    case N_STRING_2: {
                            return new_bool_node(string_is_in_array((void*)string_methods_array, method_name));
    };
    case N_ARRAY: {
                            return new_bool_node(string_is_in_array((void*)array_methods_array, method_name));
    };
    case N_INTEGER : {
                            return new_bool_node(string_is_in_array((void*)integer_methods_array, method_name));
    };
    case N_BOOL : {
                            return new_bool_node(string_is_in_array((void*)bool_methods_array, method_name));
    };
    case N_OBJECT : {
                            struct object_node* o = (struct object_node*) a;
                            return new_bool_node(NULL != find_method_for_class(o->class_ptr->name, strdup(method_name)));
    };
    case N_IDENTIFIER: {
                            return rrespond_to(eval_ast(a), strdup(method_name));
    };
    case N_ARRAY_CONTENT: {
                            return rrespond_to(eval_ast(a->left), strdup(method_name));
    };
    default: {
                            no_method_error("respond_to?", a);
                            break;
              };
  }
}

struct ast* reach(struct ast* a, struct ast* block) {

  if (a->node_type == N_ARRAY) {

    if (block != NULL) {

      struct ast* arr = (struct ast*) a->left;
      struct opt_block_node* b = (struct opt_block_node*) block;

      // obtengo arreglo a partir de Array
      int arr_size = array_tree_size(arr);

      struct ast* result[arr_size];

      struct ast* ptr = arr;
      int i;
      for (i = (arr_size - 1); i > -1; i--) {
        result[i] = ptr->left;
        ptr = ptr->right;
      };

      // itero sobre array
      for (i = 0; i < arr_size; i++) {
        push_scope();

        // put first arg to scope
        if (b->opt_ids != NULL) {
          put_sym(SYM_VAR, string_value(b->opt_ids->arg), result[i], NULL);
        };

        // eval block stmts
        eval_ast(b->stmts);

        pop_scope();
      }; 

      return a;

    } else {
      block_is_required_error(EACH_ITERATOR);
    };

  } else {
    no_method_error(EACH_ITERATOR, a);
  };  

};

int is_native_method(struct ast* m){
	if (m == NULL){
		return 0;
	};

  char* method_name;
  char** native_methods_array;
  if (m->node_type == N_METHOD_CALL_0) {
    method_name = strdup(((struct method_call_node*)m)->method_name);
    native_methods_array = class_native_methods;
  } else if (m->node_type == N_METHOD_CALL_1) {
    method_name = strdup(((struct method_call_node*)m)->method_name);
    native_methods_array = instance_native_methods;
  } else if (m->node_type == N_METHOD_CALL_2) {
    method_name = strdup(((struct method_call_node*)m)->method_name);
    native_methods_array = native_methods;
  } else if (m->node_type == N_IDENTIFIER) {
    method_name = strdup(((struct identifier_node*)m)->name);
    native_methods_array = native_methods;
  };

	int i = 0;
	int encontre = 0;
	while(!encontre && i < array_size((void*)native_methods_array)){
		encontre = !strcmp(method_name, native_methods_array[i]);
		i = i + 1;
	};
	return encontre;
};

struct ast* eval_native_method(struct ast* m){
	if (m != NULL){
    char* method_name;
    if (m->node_type == N_METHOD_CALL_2) {
      method_name = strdup(((struct method_call_node*)m)->method_name);
    } else if (m->node_type == N_IDENTIFIER) {
      method_name = strdup(((struct identifier_node*)m)->name);
    };

		if (!strcmp(method_name, PUTS)) {
			return rputs(m);
    } else if (!strcmp(method_name, GETS)) {
      return rgets();
    };
	};
  return new_nil_node();
};

struct ast* eval_instance_native_method(struct ast* m) {
	if (m != NULL){
    char* method_name;

    if (m->node_type == N_METHOD_CALL_1) {
      method_name = strdup(((struct method_call_node*)m)->method_name);
    };

		if (!strcmp(method_name, LENGTH)) {
      struct method_call_node* mc = (struct method_call_node*)m;
      return rlength(eval_ast(mc->left_ast));

    } else if (!strcmp(method_name, EACH_ITERATOR)) {
      struct method_call_node* mc = (struct method_call_node*)m;
      return reach(eval_ast(mc->left_ast), mc->opt_block);

    } else if (!strcmp(method_name, RESPOND_TO)) {
      struct method_call_node* mc = (struct method_call_node*)m;
      struct list_node* arg_node = mc->args;
      if (list_length(arg_node) != 1){
        wrong_arguments_error(list_length(arg_node), 1);
      }
      return rrespond_to(eval_ast(mc->left_ast), strdup(string_value(eval_ast(arg_node->arg))));

    } else if (!strcmp(method_name, NIL_METHOD)) {
      return rnil((struct method_call_node*)m);

    } else if (!strcmp(method_name, OBJECT_ID)) {
      return robject_id((struct method_call_node*)m);
    };
	};
  return new_nil_node();
};

struct ast* eval_class_native_method(struct method_call_node* m){
  if (m != NULL) {

  	// new
    if (!strcmp(m->method_name, NEW)) {
      //creo objeto
      struct class* class_ptr = find_class(string_value(m->left_ast));	
      struct sym* sym_list = copy_instance_variables_for_class(class_ptr);
      struct ast* new_object = new_object_node(class_ptr, sym_list);

      struct sym* s = find_method_for_class(string_value(m->left_ast), "initialize");

      // initialize está definido
        if (s != NULL) {           
          // llamo a initialize sobre el objeto 
          eval_and_push_args_and_object_info(s->args, m->args, new_object);
          struct ast* eval = eval_ast(s->ast); //eval initialize
          update_instance(new_object); //Antes de hacer pop, salvo en la instancia los cambios en sus variables de instancia
          pop_scope(); // pop del scope pusheado
        };
        return new_object;
      };
  };
};
