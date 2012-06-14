%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

extern int yylineno;

void yyerror(char const * error) {
  printf("ERROR: Line %d, %s\n", yylineno, error);
}
%}

%union {
  struct ast* node;
  int int_number;
  double double_number;
  char* string;
}

%left RETURN
%left OP_CMP_OR
%left OP_CMP_AND
%left OP_CMP_EQ OP_CMP_EQ_EQ OP_CMP_INEQ OP_CMP_NEG
%left OP_CMP_GT OP_CMP_LE OP_CMP_GT_EQ OP_CMP_LE_EQ
%left OP_PLUS OP_MINUS
%left OP_MUL OP_DIV OP_MODULO
%left OP_NOT
%left OP_EXP

%right OP_EQUAL OP_PLUS_EQ OP_MINUS_EQ OP_MUL_EQ OP_DIV_EQ OP_MODULO_EQ

%token STRING1 STRING2
%token INTEGER
%token DOUBLE
%token NIL SELF
%token DEF CLASS END DO
%token IF ELSIF ELSE CASE WHEN THEN
%token WHILE EACH
%token ATTR_READER ATTR_WRITER ATTR_ACCESSOR
%token IDENTIFIER SYMBOL
%token L_PAREN R_PAREN L_BRACE R_BRACE L_SQ_BRACK R_SQ_BRACK
%token HASH DOT COMMA SEMI_COLON OP_QUESTION NL


%type <string> STRING1 STRING2
%type <int_number> INTEGER
%type <double_number> DOUBLE
%type <node> program
%type <node> comp_statement statement
%type <node> end_of_line expression declarations method_call
%type <node> arg_decl arg_decl_fn
%type <node> elsif_optional else_optional case_when
%type <node> primary
%type <node> literal
%type <node> arg_list

%start program

%%

program    : comp_statement  { $$ = $1; /*print_ast($$);*/ }
           ;

comp_statement  : comp_statement statement { $$ = $2; }
                | /* empty */
                ;

statement  : end_of_line
           | expression end_of_line { $$ = $1; }
           | declarations end_of_line
           | method_call end_of_line
           ;

method_call : IDENTIFIER DOT IDENTIFIER arg_decl
            | IDENTIFIER arg_decl_fn
            ;

declarations : CLASS IDENTIFIER NL comp_statement END
             | DEF IDENTIFIER arg_decl NL comp_statement END    /*{ $$ = new_function($2); }*/
             | RETURN expression
             | WHILE expression NL comp_statement END
             | IF expression NL comp_statement elsif_optional else_optional END
             | CASE NL case_when END
             ;

elsif_optional : elsif_optional ELSIF expression NL comp_statement
               | /* empty */
               ;

else_optional : ELSE NL comp_statement
              | /* empty */
              ;

case_when : WHEN expression THEN comp_statement
          | /* empty */

expression : IDENTIFIER OP_EQUAL expression
           | IDENTIFIER OP_PLUS_EQ expression
           | IDENTIFIER OP_MINUS_EQ expression
           | IDENTIFIER OP_MUL_EQ expression
           | IDENTIFIER OP_DIV_EQ expression
           | IDENTIFIER OP_MODULO_EQ expression
           | expression OP_EXP expression
           | expression OP_MUL expression           { $$ = new_ast(N_MUL, $1, $3);        }
           | expression OP_DIV expression           { $$ = new_ast(N_DIV, $1, $3);        }
           | expression OP_MODULO expression        { $$ = new_ast(N_MODULO, $1, $3);     }
           | expression OP_PLUS expression          { $$ = new_ast(N_PLUS, $1, $3);       }
           | expression OP_MINUS expression         { $$ = new_ast(N_MINUS, $1, $3);      }
           | expression OP_CMP_GT expression        { $$ = new_ast(N_CMP_GT, $1, $3);     }
           | expression OP_CMP_GT_EQ expression     { $$ = new_ast(N_CMP_GT_EQ, $1, $3);  }
           | expression OP_CMP_LE expression        { $$ = new_ast(N_CMP_LE, $1, $3);     }
           | expression OP_CMP_LE_EQ expression     { $$ = new_ast(N_CMP_LE_EQ, $1, $3);  }
           | expression OP_CMP_EQ expression        { $$ = new_ast(N_CMP_EQ, $1, $3);     }
           | expression OP_CMP_EQ_EQ expression     { $$ = new_ast(N_CMP_EQ_EQ, $1, $3);  }
           | expression OP_CMP_INEQ expression      { $$ = new_ast(N_CMP_INEQ, $1, $3);   }
           | expression OP_CMP_NEG expression       { $$ = new_ast(N_CMP_NEG, $1, $3);    }
           | expression OP_CMP_AND expression       { $$ = new_ast(N_CMP_AND, $1, $3);    }
           | expression OP_CMP_OR expression        { $$ = new_ast(N_CMP_OR, $1, $3);     }
           | OP_PLUS expression    %prec OP_NOT     { $$ = new_ast(N_PLUS_UN, $2, NULL);  }
           | OP_MINUS expression   %prec OP_NOT     { $$ = new_ast(N_MINUS_UN, $2, NULL); }
           | OP_NOT expression                      { $$ = new_ast(N_NOT, $2, NULL);      }
           | L_PAREN expression R_PAREN
           | primary { $$ = $1; }
           ;

primary    : literal               { $$ = $1;                 }
           | IDENTIFIER            { $$ = new_identifier($1); }
           | NIL                   { ; }
           | SELF                  { ; }
           ;

arg_decl  : L_PAREN arg_list R_PAREN
          | arg_list
          | /* empty */
          ;

arg_decl_fn : L_PAREN arg_list R_PAREN
            | arg_list
            ;

arg_list  : arg_list COMMA primary
          | primary                      { $$ = $1; }
          ;

literal    : INTEGER  { $$ = new_int_number($1);    }
           | DOUBLE   { $$ = new_double_number($1); }
           | SYMBOL   { $$ = new_symbol($1);        }
           | STRING1  { $$ = new_string_1($1);      }
           | STRING2  { $$ = new_string_2($1);      }
           ;

end_of_line   : NL
              | SEMI_COLON
           ;

%%

int main(int argc, char** argv) {
  return yyparse();
}
