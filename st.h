#ifndef ST_H
#define ST_H

/*****************************************/
/************ SYMBOL TABLE  **************/
/*****************************************/

#include "structures.h"

#define SYM_FUNC 0
#define SYM_VAR  1

//declaro variable global
extern struct sym* sym_table;

struct sym {
  char* name; /* name of symbol */
  int sym_type; /* type of symbol: SYM_FUNC || SYM_VAR */
  struct ast* ast; /* value of symbol */ 
  struct arg_list_node* fn_args; /* function arguments */	
  struct sym* next; /* next symbol */
};

struct sym_list {
  struct sym* sym;
  struct sym* next;
};

void put_sym(int, char*, struct ast*, struct arg_list_node* fn_args);
struct ast* get_sym(char*);
void print_sym_table();

#endif