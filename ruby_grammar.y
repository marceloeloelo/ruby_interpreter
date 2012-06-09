%{
#include <stdio.h>

void yyerror(char const * error) {
  printf("ERROR: %s\n", error);
}
%}

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
%token DEF CLASS RETURN END DO
%token IF ELSIF ELSE
%token WHILE EACH
%token ATTR_READER ATTR_WRITER ATTR_ACCESSOR
%token PUTS NEW
%token IDENTIFIER SYMBOL
%token DOUBLE INTEGER
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
           ;

expression : IDENTIFIER OP_EQUAL expression
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
