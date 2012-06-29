#include "native_methods.h"

void rputs(struct method_call_node* m){
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
			default:{
				printf("Not supported type for puts, sorry :D\n");
				break;
			};
		};
		arg_node = arg_node->next;
	};
};

char* native_methods[2] = {PUTS, GETS};

int is_native_method(struct method_call_node* m){
	if (m == NULL){ 
		return 0;
	};
	int i = 0;
	int encontre = 0;
	while(!encontre && i < array_size((void*)native_methods)){
		encontre = !strcmp(m->method_name, native_methods[i]);
		i = i + 1;
	};
	return encontre;
};

void eval_native_method(struct method_call_node* m){
	if (m != NULL){
		if (!strcmp(m->method_name, PUTS)) {
			rputs(m);
      	};
	};
};

