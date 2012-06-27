#include "st.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//struct sym* install_sym_table() {
//  return NULL; /* pointer to sym_table */
//};

void put_sym(char* sym_name, struct ast* ast, struct sym_list* fn_args) {
  struct sym* ptr = sym_table;
  while ((ptr != (struct sym*) 0) && (strcmp(ptr->name, sym_name) != 0)) {
    ptr = (struct sym*) ptr->next;
  };
  if (ptr == NULL) {
    ptr = malloc (sizeof(struct sym));  
    ptr->name = malloc (strlen(sym_name)+1);
    strcpy(ptr->name, sym_name);
    ptr->next = sym_table;
    sym_table = ptr; // actualizo puntero tabla
  };
  ptr->ast = ast;
  ptr->fn_args = fn_args;
};

struct ast* get_sym(char* sym_name) {
  struct sym* ptr;
  for (ptr = sym_table; ptr != (struct sym*) 0; ptr = (struct sym*) ptr->next)
    if (strcmp(ptr->name, sym_name) == 0)
      return ptr->ast;
  return NULL;
};

void print_sym_table() {
  struct sym* ptr;
  for (ptr = sym_table; ptr != 0; ptr = ptr->next) {
    printf("%s =\n", ptr->name);
    print_ast(ptr->ast);
  }
}