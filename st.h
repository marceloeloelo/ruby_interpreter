#ifndef ST_H
#define ST_H

/*****************************************/
/************ SYMBOL TABLE  **************/
/*****************************************/

#include "structures.h"

#define SYM_FUNC       0
#define SYM_VAR        1
#define SYM_CLASS      2
#define SYM_CLASS_VAR  3
#define SYM_INST_VAR   4


//declaro variable global
extern struct scope* sym_table;
extern struct class* class_table;

struct sym {
  char* name;              /* name of symbol */
  int sym_type;            /* type of symbol: SYM_FUNC || SYM_VAR */
  struct ast* ast;         /* value of symbol */ 
  struct list_node* args;  /* function arguments */	
  struct sym* next;        /* next symbol */
};

struct class {
  char* name;
  int sym_type;
  struct sym* sym_list; // SYM_CLASS_VAR, SYM_INST_VAR, SYM_FUNC
  struct class* next;
};

 struct scope {
  struct sym* sym_list;
  struct scope* next;
};

//
//
// sym_table

void put_sym(int, char*, struct ast*, struct list_node*);
struct sym* get_sym(int, char*);
struct scope* build_scope();
void push_scope();
void push_scope_on_copy(struct scope* new_scope);
void pop_scope();
struct sym* copy_sym_list(struct sym*);
void put_object_sym_list_in_scope(struct scope* , struct object_node*);
void put_class_sym_list_in_scope(struct scope*, struct class*);
void print_sym_list(struct sym*);
void print_sym_table();
void print_sym_list_for_object(struct object_node*);

//
//
// class_table

void pop_scope_and_define_class(char*);
int class_exists(char*);
struct class* find_class(char*);
struct sym* find_method_for_class(char*, char*);
struct sym* copy_instance_variables_for_class(struct class*);
void print_class_table();

#endif
