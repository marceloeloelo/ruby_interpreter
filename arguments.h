/*****************************************/
/*************** ARGUMENTS  **************/
/*****************************************/
#define MAX_NUMBER_SYMBOLS 9999

extern int yylineno;
void yyerror(char*);

/* Symbol table. */
// struct symbol {
//   char* var_name;
//   double var_value;
//   struct ast* function;
//   struct symbols_list* dummy_args;
// };

// struct symbol symbol_table[9999];

// struct symbol* lookup(char*);

/* List of symbols. */
// struct symbols_list {
//   struct symbol* sym;
//   struct symbols_list* next;
// };

/* API */
// struct symbols_list* new_symbols_list(struct symbol*, struct symbols_list*);
// void free_symbols_list(struct symbols_list*);
