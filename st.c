#include "st.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//struct sym* install_sym_table() {
//  return NULL; /* pointer to sym_table */
//};

struct sym* put_sym(struct sym* sym_table, char* sym_name) {
  struct sym* ptr;
  ptr = malloc (sizeof(struct sym));
  ptr->name = malloc (strlen(sym_name)+1);
  strcpy (ptr->name, sym_name);
  ptr->value = 0;
  ptr->fn = 0;
  ptr->fn_args = 0;
  ptr->next = sym_table;
  return ptr;
};

struct sym* get_sym(struct sym* sym_table, char* sym_name) {
  struct sym* ptr;
  for (ptr = sym_table; ptr != (struct sym*) 0; ptr = (struct sym*) ptr->next )
    if (strcmp (ptr->name,sym_name) == 0)
      return ptr;
  return 0;
};

void print_sym_table(struct sym* sym_table) {
  struct sym* ptr;
  for (ptr = sym_table; ptr != 0; ptr = ptr->next ) {
    printf("%s\n", ptr->name);
  }
}