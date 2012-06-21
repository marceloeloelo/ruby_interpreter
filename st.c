#include "st.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct sym* install_sym_table {
  return (struct sym*) 0; /* pointer to sym_table */
};

struct sym* put_sym (char* sym_name) {
  struct sym* ptr;
  ptr = (struct sym*) malloc (sizeof(sym));
  ptr->name = (char*) malloc (strlen(sym_name)+1);
  strcpy (ptr->name,sym_name);
  ptr->value = 0;
  ptr->fn = NULL;
  ptr->fn_args = NULL;
  ptr->next = (struct sym*) sym_table;
  sym_table = ptr;
  return ptr;
};

struct sym* get_sym (char* sym_name) {
  struct sym* ptr;
  for (ptr = sym_table; ptr != (struct sym*) 0; ptr = (struct sym*) ptr->next )
    if (strcmp (ptr->name,sym_name) == 0)
      return ptr;
  return 0;
};