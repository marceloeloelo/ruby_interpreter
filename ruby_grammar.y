%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "st.h"
#include "eval.h"
#include "print.h"

extern int yylineno;

void yyerror(char const * error) {
  printf("ERROR: Line %d, %s\n", yylineno, error);
}

// install sym_table
struct scope* sym_table = 0;
struct class* class_table = 0;

%}

%union {
  struct ast* ast_type;
  struct list_node* list_type;
  int int_number;
  double double_number;
  char* string;
}

%nonassoc FIRST

%left RETURN
%right <ast_type> OP_EQUAL OP_PLUS_EQ OP_MINUS_EQ OP_MUL_EQ OP_DIV_EQ OP_MODULO_EQ
%left <ast_type> OP_CMP_OR
%left <ast_type> OP_CMP_AND
%left <ast_type> OP_CMP_EQ OP_CMP_EQ_EQ OP_CMP_INEQ OP_CMP_NEG
%left <ast_type> OP_CMP_GT OP_CMP_LE OP_CMP_GT_EQ OP_CMP_LE_EQ
%left <ast_type> OP_PLUS OP_MINUS
%left <ast_type> OP_MUL OP_DIV OP_MODULO
%left <ast_type> OP_EXP
%left <ast_type> OP_NOT

%token STRING1 STRING2 ARRAY_ACCESS
%token INTEGER
%token DOUBLE
%token NIL
%token TRUE_BOOL FALSE_BOOL
%token DEF CLASS END DO
%token IF ELSIF ELSE CASE WHEN THEN
%token WHILE EACH
%token ATTR_READER ATTR_WRITTER ATTR_ACCESSOR
%token IDENTIFIER SYMBOL CONSTANT
%token L_PAREN R_PAREN L_BRACE R_BRACE L_SQ_BRACK R_SQ_BRACK
%token HASH DOT COMMA SEMI_COLON OP_QUESTION NL PIPE

%type <string> STRING1 STRING2 IDENTIFIER SYMBOL CONSTANT ARRAY_ACCESS
%type <int_number> INTEGER
%type <double_number> DOUBLE
%type <ast_type> program comp_statement statement end_of_line expression primary literal declarations method_call if_remain optional_block  attr_statement sym_list array array_content
%type <list_type> arg_list arg_decl optional_ids arg_decl2 arg_list2 comp_class_statement

%start program

%%

program    : comp_statement                  { $$ = $1;
                                               push_scope();
                                               struct ast* eval = eval_ast($1);
                                               /*print_ast(eval);*/ }
           ;

comp_statement  : comp_statement statement   { $$ = new_ast_node(N_STMT_LIST, $2, $1); }
                | /* empty */                { $$ = NULL;                              }
                ;

statement  : end_of_line                     { $$ = $1; }
           | expression end_of_line          { $$ = $1; }
           | declarations end_of_line        { $$ = $1; }
           ;

method_call : CONSTANT DOT IDENTIFIER L_PAREN arg_decl2 R_PAREN optional_block  { $$ = new_method_call_node(N_METHOD_CALL_0, new_constant_node($1), $3, $5, $7); }
            | primary DOT IDENTIFIER  L_PAREN arg_decl2 R_PAREN optional_block  { $$ = new_method_call_node(N_METHOD_CALL_1, $1, $3, $5, $7);                    }
            | IDENTIFIER              L_PAREN arg_decl2 R_PAREN optional_block  { $$ = new_method_call_node(N_METHOD_CALL_2, NULL, $1, $3, $5);                  }
            ;

optional_block : DO PIPE optional_ids PIPE NL comp_statement END   { $$ = new_opt_block_node($3, $6); }
               | /* empty */                                       { $$ = NULL;                       }
               ;

optional_ids : optional_ids COMMA IDENTIFIER  { $$ = new_list_node(N_OPT_IDS, new_identifier_node($3), $1);   }
             | IDENTIFIER                     { $$ = new_list_node(N_OPT_IDS, new_identifier_node($1), NULL); }
             ;

declarations : CLASS CONSTANT NL comp_class_statement END                     { $$ = new_class_node($2, $4);           }
             | DEF IDENTIFIER         arg_decl         NL comp_statement END  { $$ = new_function_node($2, $3, $5);    }
             | DEF IDENTIFIER L_PAREN arg_decl R_PAREN NL comp_statement END  { $$ = new_function_node($2, $4, $7);    }
             | RETURN expression                                              { $$ = new_ast_node(N_RETURN, $2, NULL); }
             | WHILE expression NL comp_statement END                         { $$ = new_ast_node(N_WHILE, $2, $4);    }
             | IF expression NL comp_statement if_remain END                  { $$ = new_if_node($2, $4, $5);          }
             | CASE expression NL case_when case_remain END                   { $$ = NULL;                             }
             ;

comp_class_statement : comp_class_statement statement       { $$ = new_list_node(N_COMP_CLASS_STMT, $2, $1); }
                     | comp_class_statement attr_statement  { $$ = new_list_node(N_COMP_CLASS_STMT, $2, $1); }
                     | /* empty */                          { $$ = NULL;                                     }
                     ;

attr_statement : ATTR_ACCESSOR sym_list end_of_line  { $$ = new_ast_node(N_ATTR_ACCESSOR, $2, NULL); }
               | ATTR_READER sym_list end_of_line    { $$ = new_ast_node(N_ATTR_READER, $2, NULL);   }
               | ATTR_WRITTER sym_list end_of_line   { $$ = new_ast_node(N_ATTR_WRITTER, $2, NULL);  }
               ;

sym_list : sym_list COMMA SYMBOL { $$ = new_ast_node(N_SYM_LIST, new_symbol_node($3), $1);   }
         | SYMBOL                { $$ = new_ast_node(N_SYM_LIST, new_symbol_node($1), NULL); }
         ;

if_remain : ELSIF expression NL comp_statement if_remain  { $$ = new_if_node($2, $4, $5); }
          | ELSE NL comp_statement                        { $$ = $3;                      }
          | /* empty */                                   { $$ = NULL;                    }
          ;

case_when : WHEN expression NL comp_statement
          ;

case_remain : case_when case_remain
            | ELSE NL comp_statement
            | /* empty */
            ;

expression : IDENTIFIER OP_EQUAL expression              { $$ = new_ast_node(N_OP_EQUAL, new_identifier_node($1), $3);                               }
           | IDENTIFIER ARRAY_ACCESS OP_EQUAL expression { $$ = new_ast_node(N_OP_EQUAL, new_array_access_node(new_identifier_node($1), $2), $4);    }
           | IDENTIFIER OP_PLUS_EQ expression            { $$ = new_ast_node(N_OP_PLUS_EQ, new_identifier_node($1), $3);                             }
           | IDENTIFIER OP_MINUS_EQ expression           { $$ = new_ast_node(N_OP_MINUS_EQ, new_identifier_node($1), $3);                            }
           | IDENTIFIER OP_MUL_EQ expression             { $$ = new_ast_node(N_OP_MUL_EQ, new_identifier_node($1), $3);                              }
           | IDENTIFIER OP_DIV_EQ expression             { $$ = new_ast_node(N_OP_DIV_EQ, new_identifier_node($1), $3);                              }
           | IDENTIFIER OP_MODULO_EQ expression          { $$ = new_ast_node(N_OP_MODULO_EQ, new_identifier_node($1), $3);                           }
           | expression OP_EXP expression                { $$ = new_ast_node(N_OP_EXP, $1, $3);                                                      }
           | expression OP_MUL expression                { $$ = new_ast_node(N_OP_MUL, $1, $3);                                                      }
           | expression OP_DIV expression                { $$ = new_ast_node(N_OP_DIV, $1, $3);                                                      }
           | expression OP_MODULO expression             { $$ = new_ast_node(N_OP_MODULO, $1, $3);                                                   }
           | expression OP_PLUS expression               { $$ = new_ast_node(N_OP_PLUS, $1, $3);                                                     }
           | expression OP_MINUS expression              { $$ = new_ast_node(N_OP_MINUS, $1, $3);                                                    }
           | expression OP_CMP_GT expression             { $$ = new_ast_node(N_OP_CMP_GT, $1, $3);                                                   }
           | expression OP_CMP_GT_EQ expression          { $$ = new_ast_node(N_OP_CMP_GT_EQ, $1, $3);                                                }
           | expression OP_CMP_LE expression             { $$ = new_ast_node(N_OP_CMP_LE, $1, $3);                                                   }
           | expression OP_CMP_LE_EQ expression          { $$ = new_ast_node(N_OP_CMP_LE_EQ, $1, $3);                                                }
           | expression OP_CMP_EQ expression             { $$ = new_ast_node(N_OP_CMP_EQ, $1, $3);                                                   }
           | expression OP_CMP_EQ_EQ expression          { $$ = new_ast_node(N_OP_CMP_EQ_EQ, $1, $3);                                                }
           | expression OP_CMP_INEQ expression           { $$ = new_ast_node(N_OP_CMP_INEQ, $1, $3);                                                 }
           | expression OP_CMP_NEG expression            { $$ = new_ast_node(N_OP_CMP_NEG, $1, $3);                                                  }
           | expression OP_CMP_AND expression            { $$ = new_ast_node(N_OP_CMP_AND, $1, $3);                                                  }
           | expression OP_CMP_OR expression             { $$ = new_ast_node(N_OP_CMP_OR, $1, $3);                                                   }
           | OP_PLUS expression    %prec OP_NOT          { $$ = new_ast_node(N_OP_PLUS_UN, $2, NULL);                                                }
           | OP_MINUS expression   %prec OP_NOT          { $$ = new_ast_node(N_OP_MINUS_UN, $2, NULL);                                               }
           | OP_NOT expression                           { $$ = new_ast_node(N_OP_NOT, $2, NULL);                                                    }
           | L_PAREN expression R_PAREN                  { $$ = $2;                                                                                  }
           | primary                                     { $$ = $1;                                                                                  }
           | method_call                                 { $$ = $1;                                                                                  }
           ;

primary    : literal                     { $$ = $1;                                                 }
           | IDENTIFIER                  { $$ = new_identifier_node($1);                            }
           | NIL                         { $$ = new_nil_node();                                     }
           | array                       { $$ = new_ast_node(N_ARRAY, $1, NULL);                    }
           | IDENTIFIER ARRAY_ACCESS     { $$ = new_array_access_node(new_identifier_node($1), $2); }
           ;

arg_decl  : arg_list                     { $$ = $1;   }
          | /* empty */                  { $$ = NULL; }
          ;

arg_list  : arg_list COMMA primary       { $$ = new_list_node(N_ARG_LIST, $3, $1);   }
          | primary                      { $$ = new_list_node(N_ARG_LIST, $1, NULL); }
          ;

arg_decl2  : arg_list2                   { $$ = $1;   }
           | /* empty */                 { $$ = NULL; }
           ;

arg_list2  : expression                 %prec FIRST  { $$ = new_list_node(N_ARG_LIST, $1, NULL); }
           | arg_list2 COMMA expression              { $$ = new_list_node(N_ARG_LIST, $3, $1);   }
           ;

array : L_SQ_BRACK array_content R_SQ_BRACK { $$ = $2;   }
      | L_SQ_BRACK R_SQ_BRACK               { $$ = NULL; }
      ;

array_content : array_content COMMA primary { $$ = new_ast_node(N_ARRAY_CONTENT, $3, $1);   }
              | primary                     { $$ = new_ast_node(N_ARRAY_CONTENT, $1, NULL); }
              ;

literal    : INTEGER                     { $$ = new_integer_node($1);                 }
           | DOUBLE                      { $$ = new_double_node($1);                  }
           | SYMBOL                      { $$ = new_symbol_node($1);                  }
           | STRING1                     { $$ = new_string_one_node(drop_quotes($1)); }
           | STRING2                     { $$ = new_string_two_node(drop_quotes($1)); }
           | TRUE_BOOL                   { $$ = new_bool_node(1);                     }
           | FALSE_BOOL                  { $$ = new_bool_node(0);                     }
           ;

end_of_line : NL                         { $$ = NULL; }
            | SEMI_COLON                 { $$ = NULL; }
            ;

%%

int main(int argc, char** argv) {
  if (open_file(argv[1])) {
    return yyparse();
  };
};

