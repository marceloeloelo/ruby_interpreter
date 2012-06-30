#include "errors.h"
#include <stdio.h>
#include <stdlib.h>


//
//
// aux

char* type_name(int node_type) {
  switch(node_type) {
    case N_INTEGER  : { return "Fixnum";   };
    case N_DOUBLE   : { return "Float";    };
    case N_STRING_1 : { return "String";   };
    case N_NIL      : { return "NilClass"; };
    case N_BOOL     : { return "Bool"; };
    default         : { return "FALTA CONSIDERAR TIPO"; };
  };
};


int type_error(int type_one, int type_two) {
  
  printf("TypeError: %s can't be coerced into %s\n", type_name(type_two), type_name(type_one));
  exit(1);

};

int no_method_error(char* method, struct ast* ast) {
  switch(ast->node_type) {
    case N_NIL : {
      printf("NoMethodError: undefined method `%s' for nil:NilClass", method);
      break;
    };
    case N_STRING_1 : {
      struct string_node* s = (struct string_node*) ast;
      printf("NoMethodError: undefined method `%s' for \"%s\":%s\n", method, s->value, type_name(s->node_type));
      break;
    };
    case N_INTEGER : {
      struct integer_node* i = (struct integer_node*) ast;
      printf("NoMethodError: undefined method `%s' for %i:%s\n", method, i->value, type_name(i->node_type));	
      break;
    };
    case N_DOUBLE : {
      struct double_node* d = (struct double_node*) ast;
      printf("NoMethodError: undefined method `%s' for %e:%s\n", method, d->value, type_name(d->node_type));	
      break;
    };
    case N_BOOL : {
      struct bool_node* b = (struct bool_node*) ast;
      printf("NoMethodError: undefined method `%s' for %s:%s\n", method, (b->value == 1) ? "true" : "false", type_name(b->node_type));  
      break;
    };
    default : {
  	  // TODO imprimir distintos tipos de datos
      printf("NoMethodError: FALTA DISTINGUIR ESTE TIPO");	

    };
  };
  exit(1);

};

int undefined_variable_error(char* variable_name) {
  printf("NameError: undefined local variable or method `%s' for main:Object\n", variable_name);
  exit(1);
};


int wrong_arguments_error(int x, int y) {
  printf("ArgumentError: wrong number of arguments (%i for %i)\n", x, y);
  exit(1);
};

int unexpected_type_error(int type) {
  printf("syntax error, unexpected %s\n", type_name(type));
  exit(1);
};
