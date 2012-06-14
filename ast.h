/*****************************************/
/******* ABSTRACT SYNTAX TREE  ***********/
/*****************************************/

#define N_MUL         1
#define N_DIV         2
#define N_MODULO      3
#define N_PLUS        4
#define N_MINUS       5
#define N_CMP_GT      6
#define N_CMP_GT_EQ   7
#define N_CMP_LE      8
#define N_CMP_LE_EQ   9
#define N_CMP_EQ      10
#define N_CMP_EQ_EQ   11
#define N_CMP_INEQ    12
#define N_CMP_NEG     13
#define N_CMP_AND     14
#define N_CMP_OR      15
#define N_PLUS_UN     16
#define N_MINUS_UN    17
#define N_NOT         18

#define N_INTEGER     19
#define N_DOUBLE      20
#define N_STRING_1    21
#define N_STRING_2    22
#define N_IDENTIFIER  23
#define N_FUNCTION    24

extern int yylineno;
void yyerror(char const*);

/* Node types.
 * + - * / %
 * =
*/

struct ast {
  int node_type;
  struct ast* left;
  struct ast* right;
};

struct binary_op {
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
};

struct identifier {
  int node_type;
  char* name;
};

struct function {
  int node_type;
  char* name;
  struct ast* params;
};

struct ast* new_ast(int, struct ast*, struct ast*);

struct ast* new_binary_op(int, struct ast*, struct ast*);

struct ast* new_int_number(int);

struct ast* new_double_number(double);

struct ast* new_string(char*);

struct ast* new_identifier(char*);

struct ast* new_function(char*, struct ast*);

double eval_ast(struct ast*);

void free_ast(struct ast*);

void print_ast(struct ast*);
