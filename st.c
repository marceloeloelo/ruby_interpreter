#include "st.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
//
// sym_table

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
  struct sym* ptr;
  for (ptr = scope->sym_list; ptr != 0; ptr = ptr->next) {
    if ((strcmp(ptr->name, sym_name) == 0) && (ptr->sym_type == sym_type)){
      return ptr;
    };
  };
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
  for (scope = sym_table; scope!= 0; scope = scope->next) {
    printf("--- begin scope ---\n");
    struct sym* ptr;
    for (ptr = scope->sym_list; ptr != 0; ptr = ptr->next) {
      printf("%s = ", ptr->name);
      print_ast(ptr->ast);
      printf("\n");
    };
    printf("---- end scope ----\n");
  };

};


//
//
// class_table

void pop_scope_and_define_class(char* name) {

  // adquiero sym_list de scope actual
  struct sym* sym_list = sym_table->sym_list;

  // initialization
  struct class* c = malloc (sizeof(struct class));
  c->sym_type = SYM_CLASS;
  c->name = malloc((strlen(name)+1)*sizeof(char));
  strcpy(c->name, name);
  c->sym_list = sym_list;

  // add to class table
  c->next = class_table;
  class_table = c;

  // pop_scope
  sym_table = sym_table->next;

};

void print_class_table() {

  struct class* c;
  for (c = class_table; c != NULL; c = c->next) {

    struct sym* s;
    fprintf(stdout, "--- class %s -----\n", c->name);
    for (s = c->sym_list; s != NULL; s = s->next) {
      fprintf(stdout, "%s = ", s->name);
      print_ast(s->ast);
      fprintf(stdout, "\n");
    };
    fprintf(stdout, "---- end class ----\n");
  };

};

