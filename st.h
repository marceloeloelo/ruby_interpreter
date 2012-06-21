/*****************************************/
/************ SYMBOL TABLE  **************/
/*****************************************/

struct sym {
  char* name; /* name of symbol */
  double value; /* value of symbol */ 
  struct ast* fn; /* asociated function of symbol */
  struct sym_list* fn_args; /* function arguments */
  struct sym* next; /* next symbol */
};

struct sym_list {
  struct sym* sym;
  struct sym* next;
};

struct sym* sym_table; /* global variable sym_table */
struct sym* install_sym_table;
struct sym* put_sym(char*);
struct sym* get_sym(char*);