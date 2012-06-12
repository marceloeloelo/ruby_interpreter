extern int yylineno;
void yyerror(char const * error);

struct ast {
  int node_type;
  struct ast* left;
  struct ast* right;
};

struct int_number {
  int node_type;
  int value;
};

struct double_number {
  int node_type;
  double value;
};

struct string {
  int node_type;
  char* value;
}

struct identifier {
  int node_type;
  char* value;
};

struct ast* new_ast(int, struct ast*, struct ast*);

struct ast* new_int_number(int);

struct ast* new_double_number(double);

struct ast* new_string(char*);

struct ast* new_identifier(char*);

double eval_ast(struct ast*);

void free_ast(struct ast*);

void print_ast(struct ast*);
