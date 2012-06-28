%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "st.h"

extern int yylineno;

void yyerror(char const * error) {
  printf("ERROR: Line %d, %s\n", yylineno, error);
}

// install sym_table
struct sym* sym_table = 0;

%}

%union {
  struct ast* ast_type;
  struct arg_list_node* arg_list_type;
  int int_number;
  double double_number;
  char* string;
}

%left RETURN
%left <ast_type> OP_CMP_OR
%left <ast_type> OP_CMP_AND
%left <ast_type> OP_CMP_EQ OP_CMP_EQ_EQ OP_CMP_INEQ OP_CMP_NEG
%left <ast_type> OP_CMP_GT OP_CMP_LE OP_CMP_GT_EQ OP_CMP_LE_EQ
%left <ast_type> OP_PLUS OP_MINUS
%left <ast_type> OP_MUL OP_DIV OP_MODULO
%left <ast_type> OP_NOT
%left <ast_type> OP_EXP
%right <ast_type> OP_EQUAL OP_PLUS_EQ OP_MINUS_EQ OP_MUL_EQ OP_DIV_EQ OP_MODULO_EQ

%token STRING1 STRING2
%token INTEGER
%token DOUBLE
%token NIL
%token TRUE_BOOL FALSE_BOOL
%token DEF CLASS END DO
%token IF ELSIF ELSE CASE WHEN THEN
%token WHILE EACH
%token ATTR_READER ATTR_WRITER ATTR_ACCESSOR
%token IDENTIFIER SYMBOL
%token L_PAREN R_PAREN L_BRACE R_BRACE L_SQ_BRACK R_SQ_BRACK
%token HASH DOT COMMA SEMI_COLON OP_QUESTION NL PIPE


%type <string> STRING1 STRING2 IDENTIFIER SYMBOL
%type <int_number> INTEGER
%type <double_number> DOUBLE
%type <ast_type> program comp_statement statement end_of_line expression primary literal declarations method_call if_remain optional_block comp_class_statement
%type <arg_list_type> arg_list arg_decl arg_decl_fn optional_ids arg_decl2 arg_list2

%start program

%%

program    : comp_statement                  { $$ = $1; 
                                               /*struct ast* eval = eval_ast($1);
                                               print_ast(eval);*/ }
           ;

comp_statement  : comp_statement statement   { $$ = new_ast_node(N_STMT_LIST, $2, $1); }
                | /* empty */                { $$ = NULL; }
                ;

statement  : end_of_line                     { $$ = $1; }
           | expression end_of_line          { $$ = $1; }
           | declarations end_of_line        { $$ = $1; }
           | method_call end_of_line         { $$ = $1; }
           ;

method_call : IDENTIFIER DOT IDENTIFIER arg_decl2 optional_block { $$ = new_method_call_node(N_METHOD_CALL_1, $1, $3, $4);   }
            | IDENTIFIER arg_decl_fn optional_block            { $$ = new_method_call_node(N_METHOD_CALL_2, NULL, $1, $2); }
            ;

optional_block : DO PIPE optional_ids PIPE NL comp_statement END { $$ = NULL; }
               | /* empty */                                                { $$ = NULL; }
               ;

optional_ids : optional_ids COMMA IDENTIFIER { $$ = NULL; }
             | IDENTIFIER                    { $$ = NULL; }
             ;

declarations : CLASS IDENTIFIER NL comp_class_statement END        { $$ = new_class_node($2, $4);           }
             | DEF IDENTIFIER arg_decl NL comp_statement END       { $$ = new_function_node($2, $3, $5);    }
             | RETURN expression                                   { $$ = new_ast_node(N_RETURN, $2, NULL); }
             | WHILE expression NL comp_statement END              { $$ = new_ast_node(N_WHILE, $2, $4);    }
             | IF expression NL comp_statement if_remain END       { $$ = new_if_node(N_IF, $2, $4, $5);    }
             | CASE expression NL case_when case_remain END        { $$ = NULL; }
             ;

comp_class_statement : comp_class_statement statement       { $$ = NULL; }
                     | comp_class_statement attr_statement  { $$ = NULL; }
                     | /* empty */                          { $$ = NULL; }
                     ;

attr_statement : ATTR_ACCESSOR sym_list end_of_line
               | ATTR_READER sym_list end_of_line
               | ATTR_WRITER sym_list end_of_line
               ;

sym_list : sym_list COMMA SYMBOL
         | SYMBOL
         ;

if_remain : ELSIF expression NL comp_statement if_remain     { $$ = new_if_node(N_IF_REM, $2, $4, $5); }
          | ELSE NL comp_statement                           { $$ = $3;                                }
          | /* empty */                                      { $$ = NULL;                              }
          ;

case_when : WHEN expression NL comp_statement
          ;

case_remain : case_when case_remain
            | ELSE NL comp_statement
            | /* empty */
            ;

expression : IDENTIFIER OP_EQUAL expression         { $$ = new_ast_node(N_OP_EQUAL, new_identifier_node($1), $3);     }
           | IDENTIFIER OP_PLUS_EQ expression       { $$ = new_ast_node(N_OP_PLUS_EQ, new_identifier_node($1), $3);   }
           | IDENTIFIER OP_MINUS_EQ expression      { $$ = new_ast_node(N_OP_MINUS_EQ, new_identifier_node($1), $3);  }
           | IDENTIFIER OP_MUL_EQ expression        { $$ = new_ast_node(N_OP_MUL_EQ, new_identifier_node($1), $3);    }
           | IDENTIFIER OP_DIV_EQ expression        { $$ = new_ast_node(N_OP_DIV_EQ, new_identifier_node($1), $3);    }
           | IDENTIFIER OP_MODULO_EQ expression     { $$ = new_ast_node(N_OP_MODULO_EQ, new_identifier_node($1), $3); }
           | expression OP_EXP expression           { $$ = new_ast_node(N_OP_EXP, $1, $3);                            }
           | expression OP_MUL expression           { $$ = new_ast_node(N_OP_MUL, $1, $3);                            }
           | expression OP_DIV expression           { $$ = new_ast_node(N_OP_DIV, $1, $3);                            }
           | expression OP_MODULO expression        { $$ = new_ast_node(N_OP_MODULO, $1, $3);                         }
           | expression OP_PLUS expression          { $$ = new_ast_node(N_OP_PLUS, $1, $3);                           }
           | expression OP_MINUS expression         { $$ = new_ast_node(N_OP_MINUS, $1, $3);                          }
           | expression OP_CMP_GT expression        { $$ = new_ast_node(N_OP_CMP_GT, $1, $3);                         }
           | expression OP_CMP_GT_EQ expression     { $$ = new_ast_node(N_OP_CMP_GT_EQ, $1, $3);                      }
           | expression OP_CMP_LE expression        { $$ = new_ast_node(N_OP_CMP_LE, $1, $3);                         }
           | expression OP_CMP_LE_EQ expression     { $$ = new_ast_node(N_OP_CMP_LE_EQ, $1, $3);                      }
           | expression OP_CMP_EQ expression        { $$ = new_ast_node(N_OP_CMP_EQ, $1, $3);                         }
           | expression OP_CMP_EQ_EQ expression     { $$ = new_ast_node(N_OP_CMP_EQ_EQ, $1, $3);                      }
           | expression OP_CMP_INEQ expression      { $$ = new_ast_node(N_OP_CMP_INEQ, $1, $3);                       }
           | expression OP_CMP_NEG expression       { $$ = new_ast_node(N_OP_CMP_NEG, $1, $3);                        }
           | expression OP_CMP_AND expression       { $$ = new_ast_node(N_OP_CMP_AND, $1, $3);                        }
           | expression OP_CMP_OR expression        { $$ = new_ast_node(N_OP_CMP_OR, $1, $3);                         }
           | OP_PLUS expression    %prec OP_NOT     { $$ = new_ast_node(N_OP_PLUS_UN, $2, NULL);                      }
           | OP_MINUS expression   %prec OP_NOT     { $$ = new_ast_node(N_OP_MINUS_UN, $2, NULL);                     }
           | OP_NOT expression                      { $$ = new_ast_node(N_OP_NOT, $2, NULL);                          }
           | L_PAREN expression R_PAREN             { $$ = $2;                                                        }
           | primary                                { $$ = $1;                                                        }
           ;

primary    : literal                     { $$ = $1;                      }
           | IDENTIFIER                  { $$ = new_identifier_node($1); }
           | NIL                         { $$ = new_nil_node();          }
           | array                       { $$ = NULL; }
           ;

arg_decl  : L_PAREN arg_list R_PAREN     { $$ = $2;   }
          | arg_list                     { $$ = $1;   }
          | /* empty */                  { $$ = NULL; }
          ;

arg_list  : arg_list COMMA primary       { $$ = new_arg_list_node($3, $1);   }
          | primary                      { $$ = new_arg_list_node($1, NULL); }
          ;

arg_decl2  : L_PAREN arg_list2 R_PAREN     { $$ = $2;   }
           | arg_list2                     { $$ = $1;   }
           | /* empty */                  { $$ = NULL; }
           ;

arg_decl_fn : L_PAREN arg_list2 R_PAREN   { $$ = $2;   }
            | arg_list2                   { $$ = $1;   }
            ;

arg_list2  : arg_list2 COMMA expression       { $$ = NULL;/*new_arg_list_node($3, $1);*/   }
          | expression                      { $$ = NULL; /*new_arg_list_node($1, NULL);*/ }
          ;

array : L_SQ_BRACK array_content R_SQ_BRACK
      | L_SQ_BRACK R_SQ_BRACK
      ;

array_content : array_content COMMA primary
              | primary
              ;

literal    : INTEGER                     { $$ = new_integer_node($1);             }
           | DOUBLE                      { $$ = new_double_node($1);              }
           | SYMBOL                      { $$ = new_string_node($1);              }
           | STRING1                     { $$ = new_string_node(drop_quotes($1)); }
           | STRING2                     { $$ = new_string_node(drop_quotes($1)); }
           | TRUE_BOOL                   { $$ = new_bool_node(1); }
           | FALSE_BOOL                  { $$ = new_bool_node(0); }
           ;

end_of_line : NL                         { $$ = NULL; }
            | SEMI_COLON                 { $$ = NULL; }
           ;

%%

int main(int argc, char** argv) {

  return yyparse();
}
