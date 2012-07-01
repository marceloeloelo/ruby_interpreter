#include "st.h"
#include "ast.h"
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

void print_sym_list(struct sym* sym_list) {
    struct sym* ptr;
    for (ptr = sym_list; ptr != 0; ptr = ptr->next) {
      printf("%s = ", ptr->name);
      print_ast(ptr->ast);
      printf("\n");
    };
};

void print_sym_table() {

  struct scope* scope;
  for (scope = sym_table; scope!= 0; scope = scope->next) {
    printf("--- begin scope ---\n");
    print_sym_list(scope->sym_list);
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

int class_exists(char* class_name) {

  struct class* c;
  for (c = class_table; c != NULL; c = c->next) {
    if (strcmp(c->name, class_name) == 0){
      return 1;
    };
  };
  return 0;

};

struct class* find_class(char* class_name) {

  struct class* c;
  for (c = class_table; c != NULL; c = c->next) {
    if (strcmp(c->name, class_name) == 0){
      return c;
    };
  };
  return NULL;

};

struct sym* find_method_for_class(char* class_name, char* method_name){

  // obtengo class
  struct class* c = find_class(class_name);
  struct sym* s;

  for (s = c->sym_list; s != 0; s = s->next) {
    if ((strcmp(s->name, method_name) == 0) && (s->sym_type == SYM_FUNC)){
      return s;
    };
  };
  return NULL;

};

struct sym* copy_instance_variables_for_class(struct class* class_ptr) {
  
  struct sym* s;
  struct sym* sym_list = NULL;

  for (s = class_ptr->sym_list; s != 0; s = s->next) {
    if (s->sym_type == SYM_INST_VAR) {

      // copio sym
      struct sym* s_copy = malloc (sizeof(struct sym));  
      s_copy->name = malloc (strlen(s->name)+1);
      strcpy(s_copy->name, s->name);
      s_copy->sym_type = s->sym_type;
      s_copy->ast = new_nil_node(); // inicializados en nil  
      
      // lo agrego a la lista
      s_copy->next = sym_list;
      sym_list = s_copy;

    };
  };
  
  return sym_list;
};

void print_class_table() {

  struct class* c;
  for (c = class_table; c != NULL; c = c->next) {

    struct sym* s;
    printf("--- class %s -----\n", c->name);
    for (s = c->sym_list; s != NULL; s = s->next) {
      printf("%s = ", s->name);
      print_ast(s->ast);
      printf("\n");
    };
    printf("---- end class ----\n");
  };

};

