#include "native_methods.h"

char* native_methods[2] = {PUTS, GETS};
char* class_native_methods[1] = {NEW};

struct ast* rputs(struct method_call_node* m){
	struct list_node* arg_node = m->args;
	while(arg_node != NULL){
		struct ast* evaluated = eval_ast(arg_node->arg);
		switch(evaluated->node_type){
			case N_STRING_1 : {
				printf("%s\n", string_value(evaluated));
				break;
			};
			case N_STRING_2 : {
				printf("%s\n", string_value(evaluated));
				break;
			};
			case N_INTEGER : {
				printf("%i\n", int_value(evaluated));
				break;
			};
			case N_DOUBLE : {
				double d = double_value(evaluated);
			    if ( d - floor(d) == 0.0 ) {
			        printf( "%g.0\n", d );
			    }
			    else {
			        printf( "%g\n", d );
			    }
				break;
			};
			case N_BOOL : {
				printf("%s\n", bool_value(evaluated) ? "true" : "false");
				break;
			};
			case N_NIL : {
				printf("%s\n", "");
				break;
			};
			default:{
				printf("Puts doesn't support %s type, sorry :D\n", type_name(evaluated->node_type));
				break;
			};
		};
		arg_node = arg_node->next;
	};
  return new_nil_node();
};

struct ast* rgets() {
  char input_string[2056];
  gets(input_string);
  return new_string_node(strdup(input_string));
};

int is_native_method(struct ast* m){
	if (m == NULL){
		return 0;
	};

  char* method_name;
  if (m->node_type == N_METHOD_CALL_2) {
    method_name = strdup(((struct method_call_node*)m)->method_name);
  } else if (m->node_type == N_IDENTIFIER) {
    method_name = strdup(((struct identifier_node*)m)->name);
  };

	int i = 0;
	int encontre = 0;
	while(!encontre && i < array_size((void*)native_methods)){
		encontre = !strcmp(method_name, native_methods[i]);
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
			return rputs((struct method_call_node*)m);
    } else if (!strcmp(method_name, GETS)) {
      return rgets();
    };
	};
  return new_nil_node();
};

//
//
// class_native_methods

int is_class_native_method(struct method_call_node* m){
  if (m == NULL) { 
	return 0;
  };
  int i = 0;
  int encontre = 0;
  while(!encontre && i < array_size((void*)class_native_methods)){
    encontre = !strcmp(m->method_name, class_native_methods[i]);
    i = i + 1;
  };
  return encontre;
};

struct ast* eval_class_native_method(struct method_call_node* m){
  if (m != NULL) {

  	// new
    if (!strcmp(m->method_name, NEW)) {
      
      //creo objeto
      struct class* class_ptr = find_class(m->class_name);	
      struct sym* sym_list = copy_instance_variables_for_class(class_ptr);
      struct ast* new_object = new_object_node(class_ptr, sym_list);
	  
	  struct sym* s = find_method_for_class(m->class_name, "initialize");

	  // initialize está definido
      if (s != NULL) { 
      	// llamo a initialize sobre el objeto 
      };

      return new_object;

    };

  };
};

