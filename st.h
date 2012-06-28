#ifndef ST_H
#define ST_H

/*****************************************/
/************ SYMBOL TABLE  **************/
/*****************************************/

#include "structures.h"

#define SYM_FUNC 0
#define SYM_VAR  1

//declaro variable global
extern struct scope* sym_table;

struct sym {
  char* name; /* name of symbol */
  int sym_type; /* type of symbol: SYM_FUNC || SYM_VAR */
  struct ast* ast; /* value of symbol */ 
  struct list_node* args; /* function arguments */	
  struct sym* next; /* next symbol */
};

struct scope {
  struct sym* sym_list;
  struct scope* next;
};

void put_sym(int, char*, struct ast*, struct list_node*);
struct sym* get_sym(int, char*);
void push_scope();
void pop_scope();
void print_sym_table();

#endif
