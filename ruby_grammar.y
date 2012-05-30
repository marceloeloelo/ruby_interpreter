%{
#include <stdio.h>

void yyerror(char const * error) {
  printf("ERROR: %s\n", error);
}
%}

%token NIL SELF
%token DEF CLASS RETURN END DO
%token IF ELSIF ELSE
%token WHILE EACH
%token ATTR_READER ATTR_WRITER ATTR_ACCESSOR
%token PUTS NEW
%token IDENTIFIER SYMBOL INST_VAR
%token DOUBLE INTEGER
%right OP_EQUAL
%left OP_PLUS OP_MINUS
%left OP_MUL OP_DIV
%left OP_NOT
%token OP_CMP_AND OP_CMP_OR OP_CMP_EQ OP_CMP_NEG OP_CMP_LE OP_CMP_GT OP_CMP_LE_EQ OP_CMP_GT_EQ
%token OP_MODULO OP_EXP OP_PLUS_EQ OP_LE_EQ OP_MUL_EQ OP_DIV_EQ
%token L_PAREN R_PAREN L_BRACE R_BRACE L_SQ_BRACK R_SQ_BRACK
%token HASH DOT COMMA SEMI_COLON NL

%start input

%%

input      : /* empty */
           | input line
           ;

line       : NL
           | expresion NL
           | expresion SEMI_COLON
           ;

expresion  : ident OP_EQUAL expresion
           | L_PAREN expresion R_PAREN
        /*   | expr comparison expr */
           | ident
           | numeric
           | SEMI_COLON
           ;

numeric    : INTEGER
           | DOUBLE
           ;

ident      : IDENTIFIER
           ;

/* comparison : OP_CMP_AND
           | OP_CMP_OR
           | OP_CMP_EQ
           | OP_CMP_NEG
           | OP_CMP_LE
           | OP_CMP_GT
           | OP_CMP_LE_EQ
           | OP_CMP_GT_EQ
           ; */

%%

int main(int argc, char** argv) {
  return yyparse();
}
