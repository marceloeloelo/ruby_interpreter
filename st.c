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

/* Default constructor */
void push_scope() {

  struct scope* new_scope = malloc (sizeof(struct scope)); 
  new_scope->sym_list = NULL;
  new_scope->next = sym_table;
  sym_table = new_scope; 

};

/* On copy constructor */
void push_scope_on_copy(struct scope* scope){
  
  struct scope* new_scope = malloc (sizeof(struct scope)); 
  new_scope->sym_list = scope->sym_list;
  new_scope->next = sym_table;
  sym_table = new_scope; 

};

/*Builds an aux scope*/
struct scope* build_scope() {
  struct scope* new_scope = malloc (sizeof(struct scope)); 
  new_scope->sym_list = NULL;
  new_scope->next = NULL;
  return new_scope;
};

/*Puts a sym for a given scope*/
void put_sym_for_scope(struct scope* scope, int sym_type, char* sym_name, struct ast* ast, struct list_node* args) {

  struct sym* ptr = scope->sym_list;
  while ((ptr != (struct sym*) 0) && ((strcmp(ptr->name, sym_name) != 0) || (ptr->sym_type != sym_type))) {
    ptr = (struct sym*) ptr->next;
  };
  if (ptr == NULL) { // si no existe en la tabla de symbols lo creo
    ptr = malloc (sizeof(struct sym));  
    ptr->name = malloc (strlen(sym_name)+1);
    strcpy(ptr->name, sym_name);
    ptr->sym_type = sym_type;
    ptr->next = scope->sym_list;
    scope->sym_list = ptr; // actualizo el scope
  };
  ptr->ast = ast;
  ptr->args = args;

};

void pop_scope() { 

  sym_table = sym_table->next;

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

struct list_node* copy_list_node(struct list_node* list){
  if (list == NULL){
    return NULL;
  } else{
    struct list_node* new_list_node = malloc(sizeof(struct list_node));
    new_list_node->node_type = list->node_type;
    new_list_node->arg       = list->arg;
    new_list_node->next      = copy_list_node(list->next);
    return new_list_node;
  }
}

struct sym* copy_sym_list(struct sym* sym_list){
  if (sym_list == NULL){
    return NULL;
  } else{
    struct sym* new_sym_list = malloc(sizeof(struct sym));
    new_sym_list->name      = sym_list->name;
    new_sym_list->sym_type  = sym_list->sym_type;
    new_sym_list->ast       = sym_list->ast;
    new_sym_list->args      = copy_list_node(sym_list->args);
    new_sym_list->next      = copy_sym_list(sym_list->next);
    return new_sym_list;
  }
}

/* Copy the sym_table of the given object and puts it in the passed scope */
void put_object_sym_list_in_scope(struct scope* scope, struct object_node* object){
  struct sym* sym_list = copy_sym_list(object->sym_list);
  struct sym* s;
  for(s = sym_list; s != NULL; s = s->next){
    put_sym_for_scope(scope, s->sym_type, s->name, s->ast, s->args);
  }
}

/* Copy the sym_table of the given class and puts it in the passed scope */
void put_class_sym_list_in_scope(struct scope* scope, struct class* class){
  struct sym* sym_list = copy_sym_list(class->sym_list);
  struct sym* s;
  for(s = sym_list; s != NULL; s = s->next){
    put_sym_for_scope(scope, s->sym_type, s->name, s->ast, s->args);
  }
}

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

/*****************************************************************************************/
/**************************************** PRINTS *****************************************/
/*****************************************************************************************/
void print_sym_table() {

  struct scope* scope;
  for (scope = sym_table; scope!= 0; scope = scope->next) {
    printf("--- begin scope ---\n");
    print_sym_list(scope->sym_list);
    printf("---- end scope ----\n");
  };

};

void print_sym_list(struct sym* sym_list) {
    struct sym* ptr;
    for (ptr = sym_list; ptr != 0; ptr = ptr->next) {
      printf("%s = ", ptr->name);
      print_ast(ptr->ast);
      printf("\n");
    };
};

void print_sym_list_for_object(struct object_node* obj){
    printf("----start object symlist----\n");
    struct sym* ptr;
    for (ptr = obj->sym_list; ptr != 0; ptr = ptr->next) {
      printf("%s = ", ptr->name);
      print_ast(ptr->ast);
      printf("\n");
    };  
    printf("---- end object symlist ----\n");
}

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

