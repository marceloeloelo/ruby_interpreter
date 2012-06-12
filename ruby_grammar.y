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

%token <string> STRING1 STRING2
%token <int_number> INTEGER
%token <double_number> DOUBLE
%token NIL SELF
%token DEF CLASS END DO
%token IF ELSIF ELSE CASE WHEN THEN
%token WHILE EACH
%token ATTR_READER ATTR_WRITER ATTR_ACCESSOR
%token IDENTIFIER SYMBOL
%token L_PAREN R_PAREN L_BRACE R_BRACE L_SQ_BRACK R_SQ_BRACK
%token HASH DOT COMMA SEMI_COLON OP_QUESTION NL

%start program

%%

program    : comp_statement
           ;

comp_statement  : comp_statement statement
                | /* empty */
                ;

statement  : end_of_line
           | expression end_of_line
           | declarations end_of_line
           | method_call end_of_line
           ;

method_call : IDENTIFIER DOT IDENTIFIER arg_decl
            | IDENTIFIER arg_decl_fn
            ;

declarations : CLASS IDENTIFIER NL comp_statement END
             | DEF IDENTIFIER arg_decl NL comp_statement END
             | RETURN expression
             | WHILE expression NL comp_statement END
             | IF expression NL comp_statement elsif_optional else_optional END
             | CASE expression NL case_when case_when_optional else_optional END
             ;

elsif_optional : elsif_optional ELSIF expression NL comp_statement
               | /* empty */
               ;

else_optional : ELSE NL comp_statement
              | /* empty */
              ;

case_when : WHEN expression NL comp_statement
          ;

case_when_optional : case_when_optional case_when
                   | /* empty */
                   ;

expression : IDENTIFIER OP_EQUAL expression
           | IDENTIFIER OP_PLUS_EQ expression
           | IDENTIFIER OP_MINUS_EQ expression
           | IDENTIFIER OP_MUL_EQ expression
           | IDENTIFIER OP_DIV_EQ expression
           | IDENTIFIER OP_MODULO_EQ expression
           | expression OP_EXP expression
           | expression OP_MUL expression           { $$ = new_ast('*', $1, $3); }
           | expression OP_DIV expression           { $$ = new_ast('/', $1, $3); }
           | expression OP_MODULO expression        { $$ = new_ast('%', $1, $3); }
           | expression OP_PLUS expression          { $$ = new_ast('+', $1, $3); }
           | expression OP_MINUS expression         { $$ = new_ast('-', $1, $3); }
           | expression OP_CMP_GT expression
           | expression OP_CMP_GT_EQ expression
           | expression OP_CMP_LE expression
           | expression OP_CMP_LE_EQ expression
           | expression OP_CMP_EQ expression
           | expression OP_CMP_EQ_EQ expression
           | expression OP_CMP_INEQ expression
           | expression OP_CMP_NEG expression
           | expression OP_CMP_AND expression
           | expression OP_CMP_OR expression
           | OP_PLUS expression    %prec OP_NOT
           | OP_MINUS expression   %prec OP_NOT
           | OP_NOT expression
           | L_PAREN expression R_PAREN
           | primary
           ;

primary    : literal
           | IDENTIFIER
           | NIL
           | SELF
           ;

arg_decl  : L_PAREN arg_list R_PAREN
          | arg_list
          | /* empty */
          ;

arg_decl_fn : L_PAREN arg_list R_PAREN
            | arg_list
            ;

arg_list  : arg_list COMMA primary
          | primary
          ;

literal    : INTEGER  { $$ = new_int_number($1);    }
           | DOUBLE   { $$ = new_double_number($1); }
           | SYMBOL
           | STRING1  { $$ = new_string($1);        }
           | STRING2  { $$ = new_string($2);        }
           ;

end_of_line   : NL
              | SEMI_COLON
           ;

%%

int main(int argc, char** argv) {
  return yyparse();
}
