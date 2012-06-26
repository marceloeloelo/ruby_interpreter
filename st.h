#ifndef ST_H
#define ST_H

/*****************************************/
/************ SYMBOL TABLE  **************/
/*****************************************/

#include "structures.h"

//declaro variable global
extern struct sym* sym_table;

struct sym {
  char* name; /* name of symbol */
  struct ast* ast; /* value of symbol */ 
  struct sym* next; /* next symbol */
};

struct sym_list {
  struct sym* sym;
  struct sym* next;
};

//struct sym* install_sym_table();
void put_sym(char*, struct ast*);
struct ast* get_sym(char*);
void print_sym_table();

#endif