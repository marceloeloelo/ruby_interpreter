#ifndef STRUCTURES_H
#define STRUCTURES_H

/*****************************************/
/************** STRUCTURES  **************/
/*****************************************/

#define N_OP_PLUS_EQ      1
#define N_OP_MINUS_EQ     2
#define N_OP_MUL_EQ       3
#define N_OP_DIV_EQ       4
#define N_OP_MODULO_EQ    5
#define N_OP_CMP_OR       6
#define N_OP_CMP_AND      7
#define N_OP_CMP_NEG      8
#define N_OP_CMP_EQ_EQ    9
#define N_OP_CMP_LE_EQ    10
#define N_OP_CMP_LE       11
#define N_OP_CMP_GT_EQ    12
#define N_OP_CMP_GT       13
#define N_OP_PLUS         14
#define N_OP_MINUS        15
#define N_OP_MUL          16
#define N_OP_DIV          17
#define N_OP_MODULO       18
#define N_OP_NOT          19
#define N_OP_EXP          20
#define N_OP_PLUS_UN      21
#define N_OP_MINUS_UN     22
#define N_OP_CMP_EQ       23
#define N_OP_CMP_INEQ     24
#define N_OP_EQUAL        25
#define N_INTEGER         26
#define N_DOUBLE          27
#define N_STRING_1        28
#define N_STRING_2        29
#define N_IDENTIFIER      30
#define N_FUNCTION        31
#define N_STATEMENT       32
#define N_ARG_LIST        33
#define N_STMT_LIST       34
#define N_RETURN          35
#define N_WHILE           36
#define N_CASE            37
#define N_CLASS           38
#define N_METHOD_CALL_1   39
#define N_METHOD_CALL_2   40
#define N_IF              41
#define N_IF_REM          42
#define N_NIL             43
#define N_BOOL            44
#define N_SYM_LIST        45
#define N_OPT_IDS         46
#define N_OPT_BLOCK       47
#define N_SYMBOL          48
#define N_COMP_CLASS_STMT 49
#define N_ATTR_ACCESSOR   50
#define N_ATTR_READER     51
#define N_ATTR_WRITTER    52
#define N_ARRAY           53
#define N_ARRAY_CONTENT   54
#define N_METHOD_CALL_0   55
#define N_OBJECT          56
#define N_ARRAY_ACCESS    57
#define N_CONSTANT        58

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

struct bool_node {
  int node_type;
  int value;
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

struct symbol_node {
  int node_type;
  char* name;
};

struct object_node {
  int node_type;
  struct class* class_ptr;
  struct sym* sym_list;
};

struct constant_node {
  int node_type;
  char* value;
};

struct list_node {
  int node_type;
  struct ast* arg;
  struct list_node* next;
};

struct function_node {
  int node_type;
  char* name;
  struct list_node* args;
  struct ast* stmts;
};

struct class_node {
  int node_type;
  char* name;
  struct list_node* stmts;
};

struct method_call_node {
  int node_type;
  struct ast* left_ast;
  char* method_name;
  struct list_node* args;
  struct ast* opt_block;
};

struct if_node {
  int node_type;
  struct ast* condition;
  struct ast* th;
  struct ast* el;
};

struct opt_block_node {
  int node_type;
  struct list_node* opt_ids;
  struct ast* stmts;
};

struct array_access_node {
  int node_type;
  char* array_name;
  int entry;
};

#endif



