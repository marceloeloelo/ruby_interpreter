#include "native_methods.h"

char* native_methods[2] = { PUTS, GETS };
char* instance_native_methods[3] = { LENGTH, EACH_ITERATOR, RESPOND_TO };
char* class_native_methods[1] = {NEW};

struct ast* rputs(struct ast* a){
  if (a->node_type == N_METHOD_CALL_0 || a->node_type == N_METHOD_CALL_1 || a->node_type == N_METHOD_CALL_2) {
    struct method_call_node* m = (struct method_call_node*)a;
    struct list_node* arg_node = m->args;
    while(arg_node != NULL){
      struct ast* evaluated = eval_ast(arg_node->arg);
      rputs(evaluated);
      arg_node = arg_node->next;
    };

  } else if (a->node_type == N_ARRAY_CONTENT) {
    rputs(eval_ast(a->left));

  } else if (a->node_type == N_STRING_1) {
    printf("%s\n", string_value(a));

  } else if (a->node_type == N_STRING_2) {
    char * str = malloc(sizeof( strlen(string_value(a)) ));
    strcpy(str, string_value(a));
    /* convert end of lines in '\n' chars */
  int i = 0;
  int j = i;
  for(i = 0; i < (strlen(str) - 1); i = i + 1){
    if (str[i] == '\\' && str[i+1] == 'n'){
      str[i] = '\n';
      for(j = i + 1; j < (strlen(str)); j = j + 1){
        str[j] = str[j + 1];
      }
    }
  }
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

struct ast* rlength(struct method_call_node* m) {
  struct ast* evaluated = eval_ast(m->left_ast);
  switch (evaluated->node_type) {
    case N_STRING_1: {
                           return new_integer_node(strlen(string_value(evaluated)));
                           break;
    };
    case N_STRING_2: {
                           return new_integer_node(strlen(string_value(evaluated)));
                           break;
    };
    /* case N_ARRAY: {*/
    /*                        break;*/
    /* };*/
    /* case N_ARRAY_ACCESS: {*/
    /*                        break;*/
    /* };*/
    /* default: {*/
		/*                        break;*/
		/* };*/
  };
  return new_nil_node();
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
     return rlength((struct method_call_node*)m);
    } else if (!strcmp(method_name, EACH_ITERATOR)) {
    } else if (!strcmp(method_name, RESPOND_TO)) {
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
        };

        return new_object;
      };
  };
};
