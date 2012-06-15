/*****************************************/
/******* ABSTRACT SYNTAX TREE  ***********/
/*****************************************/

#define N_PLUS_EQ     1
#define N_MINUS_EQ    2
#define N_MUL_EQ      3
#define N_DIV_EQ      4
#define N_MODULO_EQ   5
#define N_CMP_OR      6
#define N_CMP_AND     7
#define N_CMP_NEG     8
#define N_CMP_EQ_EQ   9
#define N_CMP_LE_EQ   10
#define N_CMP_LE      11
#define N_CMP_GT_EQ   12
#define N_CMP_GT      13
#define N_PLUS        14
#define N_MINUS       15
#define N_MUL         16
#define N_DIV         17
#define N_MODULO      18
#define N_NOT         19
#define N_EXP         20

#define N_INTEGER     21
#define N_DOUBLE      22
#define N_STRING_1    23
#define N_STRING_2    24
#define N_IDENTIFIER  25
#define N_FUNCTION    26
#define N_STATEMENT   27
#define N_ARG_LIST    28
#define N_STMT_LIST   29

extern int yylineno;
void yyerror(char const*);

struct ast {
  int node_type;
  struct ast* left;
  struct ast* right;
};

struct binary_op_node {
  int node_type;
  struct ast* left;
  struct ast* right;
};

struct integer_node {
  int node_type;
  int value;
};

struct double_node {
  int node_type;
  double value;
};

struct string_node {
  int node_type;
  char* value;
};

struct identifier_node {
  int node_type;
  char* name;
};

struct arg_list_node {
  int node_type;
  struct ast* arg;
  struct arg_list_node* next;
};

struct stmt_list_node {
  int node_type;
  struct stmt_node* stmt;
  struct stmt_list_node* next;
};

struct function_node {
  int node_type;
  char* name;
  struct arg_list_node* args;
  struct stmt_list_node* stmts;
};


struct ast* new_ast(int, struct ast*, struct ast*);
struct ast* new_binary_op_node(int, struct ast*, struct ast*);
struct ast* new_integer_node(int);
struct ast* new_double_node(double);
struct ast* new_string_node(char*);
struct ast* new_identifier_node(char*);
struct ast* new_arg_list_node(struct ast*, struct ast*);
struct ast* new_stmt_list_node(struct ast*, struct ast*);
struct ast* new_function_node(char*, struct ast*, struct ast*);

double eval_ast(struct ast*);
void free_ast(struct ast*);
void print_ast(struct ast*);
