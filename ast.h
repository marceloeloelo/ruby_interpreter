/*****************************************/
/******* ABSTRACT SYNTAX TREE  ***********/
/*****************************************/

#define N_OP_PLUS_EQ     1
#define N_OP_MINUS_EQ    2
#define N_OP_MUL_EQ      3
#define N_OP_DIV_EQ      4
#define N_OP_MODULO_EQ   5
#define N_OP_CMP_OR      6
#define N_OP_CMP_AND     7
#define N_OP_CMP_NEG     8
#define N_OP_CMP_EQ_EQ   9
#define N_OP_CMP_LE_EQ   10
#define N_OP_CMP_LE      11
#define N_OP_CMP_GT_EQ   12
#define N_OP_CMP_GT      13
#define N_OP_PLUS        14
#define N_OP_MINUS       15
#define N_OP_MUL         16
#define N_OP_DIV         17
#define N_OP_MODULO      18
#define N_OP_NOT         19
#define N_OP_EXP         20
#define N_OP_PLUS_UN     21
#define N_OP_MINUS_UN    22
#define N_OP_CMP_EQ      23
#define N_OP_CMP_INEQ    24
#define N_OP_EQUAL       25
#define N_INTEGER        26
#define N_DOUBLE         27
#define N_STRING_1       28
#define N_STRING_2       29
#define N_IDENTIFIER     30
#define N_FUNCTION       31
#define N_STATEMENT      32
#define N_ARG_LIST       33
#define N_STMT_LIST      34
#define N_RETURN         35
#define N_WHILE          36
#define N_CASE           37
#define N_CLASS          38
#define N_METHOD_CALL_1  39
#define N_METHOD_CALL_2  40
#define N_IF             41
#define N_IF_REM         42
#define N_NIL            43

extern int yylineno;
void yyerror(char const*);

struct ast {
  int node_type;
  struct ast* left;
  struct ast* right;
};

struct nil_node {
  int node_type;
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
  struct ast* arg;
  struct arg_list_node* next;
};

struct function_node {
  int node_type;
  char* name;
  struct arg_list_node* args;
  struct ast* stmts;
};

struct class_node {
  int node_type;
  char* name;
  struct ast* stmts;
};

struct method_call_node {
  int node_type;
  char* class_name;
  char* method_name;
  struct arg_list_node* args;
};

struct if_node {
  int node_type;
  struct ast* condition;
  struct ast* th;
  struct ast* el;
};

struct ast* new_ast_node(int, struct ast*, struct ast*);
struct ast* new_nil_node();
struct ast* new_integer_node(int);
struct ast* new_double_node(double);
struct ast* new_string_node(char*);
struct ast* new_identifier_node(char*);
struct arg_list_node* new_arg_list_node(struct ast*, struct arg_list_node*);
struct ast* new_function_node(char*, struct arg_list_node*, struct ast*);
struct ast* new_class_node(char*, struct ast*);
struct ast* new_method_call_node(int, char*, char*, struct arg_list_node*);
struct ast* new_if_node(int, struct ast*, struct ast*, struct ast*);

struct ast* eval_ast(struct ast*);
void free_ast(struct ast*);
void print_ast(struct ast*);

//
//
// aux functions

char* drop_quotes(char*);
