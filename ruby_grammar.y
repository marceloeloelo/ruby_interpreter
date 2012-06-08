%{
#include <stdio.h>

void yyerror(char const * error) {
  printf("ERROR: %s\n", error);
}
%}

%left OP_PLUS OP_MINUS
%left OP_MUL OP_DIV OP_MODULO
%left OP_NOT
%left OP_EXP
%right OP_EQUAL

%token STRING1 STRING2
%token NIL SELF
%token DEF CLASS RETURN END DO
%token IF ELSIF ELSE
%token WHILE EACH
%token ATTR_READER ATTR_WRITER ATTR_ACCESSOR
%token PUTS NEW
%token IDENTIFIER SYMBOL INST_VAR
%token DOUBLE INTEGER
%token OP_CMP_AND OP_CMP_OR OP_CMP_EQ OP_CMP_NEG OP_CMP_LE OP_CMP_GT OP_CMP_LE_EQ OP_CMP_GT_EQ
%token OP_PLUS_EQ OP_LE_EQ OP_MUL_EQ OP_DIV_EQ
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
           | expresion end_of_line
           ;

expresion  : variable OP_EQUAL expresion
           | expresion OP_PLUS expresion
           | expresion OP_MINUS expresion
           | expresion OP_MUL expresion
           | expresion OP_DIV expresion
           | expresion OP_MODULO expresion
           | expresion OP_EXP expresion
           | OP_PLUS expresion   %prec OP_NOT
           | OP_MINUS expresion  %prec OP_NOT
           | primary
           ;

variable   : INST_VAR
           | IDENTIFIER
           | NIL
           | SELF
           ;

primary    : literal
           | IDENTIFIER
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
