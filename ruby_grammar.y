%{
#include <stdio.h>
#include <stdlib.h>

extern int yylineno;

void yyerror(char const * error) {
  printf("ERROR: Line %d, %s\n", yylineno, error);
}
%}

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
%token NIL SELF
%token DEF CLASS END DO
%token IF ELSIF ELSE CASE WHEN THEN
%token WHILE EACH
%token ATTR_READER ATTR_WRITER ATTR_ACCESSOR
%token IDENTIFIER SYMBOL
%token DOUBLE INTEGER
%token L_PAREN R_PAREN L_BRACE R_BRACE L_SQ_BRACK R_SQ_BRACK PIPE
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

method_call : IDENTIFIER DOT IDENTIFIER arg_decl block_optional
            | IDENTIFIER arg_decl_fn block_optional
            ;

block_optional : DO PIPE IDENTIFIER ids_optional PIPE NL comp_statement END
               | /* empty */
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
           | expression OP_MUL expression
           | expression OP_DIV expression
           | expression OP_MODULO expression
           | expression OP_PLUS expression
           | expression OP_MINUS expression
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
           | array
           ;

array : L_SQ_BRACK primary array_content_optional R_SQ_BRACK
      | L_SQ_BRACK R_SQ_BRACK
      ;

array_content_optional : array_content_optional COMMA primary
                       | /* empty */                 
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

ids_optional : ids_optional COMMA IDENTIFIER 
             | /* empty */
             ;


literal    : INTEGER
           | DOUBLE
           | SYMBOL
           | STRING1
           | STRING2
           ;

end_of_line   : NL
              | SEMI_COLON
           ;

%%

int main(int argc, char** argv) {
  return yyparse();
}
