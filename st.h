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

//struct sym* install_sym_table();
struct sym* put_sym(struct sym*, char*);
struct sym* get_sym(struct sym*, char*);
void print_sym_table(struct sym*);