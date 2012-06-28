#include "st.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void put_sym(int sym_type, char* sym_name, struct ast* ast, struct list_node* args) {
  struct sym* ptr = sym_table->sym_list;
  while ((ptr != (struct sym*) 0) && ((strcmp(ptr->name, sym_name) != 0) || (ptr->sym_type != sym_type))) {
    ptr = (struct sym*) ptr->next;
  };
  if (ptr == NULL) { // si no existe en la tabla de symbols lo creo
    ptr = malloc (sizeof(struct sym));  
    ptr->name = malloc (strlen(sym_name)+1);
    strcpy(ptr->name, sym_name);
    ptr->sym_type = sym_type;
    ptr->next = sym_table->sym_list;
    sym_table->sym_list = ptr; // actualizo el scope
  };
  ptr->ast = ast;
  ptr->args = args;
};

struct sym* get_sym(int sym_type, char* sym_name) {
  struct scope* scope;
  scope = sym_table;
  //for (scope = sym_table; scope != 0; scope = scope->next) { // recorro todos los scopes hasta encontrar sym
    struct sym* ptr;
    for (ptr = scope->sym_list; ptr != 0; ptr = ptr->next) {
      if ((strcmp(ptr->name, sym_name) == 0) && (ptr->sym_type == sym_type)){
        return ptr;
      };
    };
 // };
  return NULL;  
};

void push_scope() {
  struct scope* new_scope = malloc (sizeof(struct scope)); 
  new_scope->sym_list = NULL;
  new_scope->next = sym_table;
  sym_table = new_scope; 
};

void pop_scope() { 
  sym_table = sym_table->next;
};

void print_sym_table() {
  struct scope* scope;
  for (scope = sym_table; scope!= 0; scope = scope->next) { // recorro todos los scopes hasta encontrar sym
    printf("---begin scope---\n");
    struct sym* ptr;
    for (ptr = scope->sym_list; ptr != 0; ptr = ptr->next) {
      printf("%s = ", ptr->name);
      print_ast(ptr->ast);
      printf("\n");
    };
    printf("---end scope-----\n");
  };
}
