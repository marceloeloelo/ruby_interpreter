%{
#include <stdio.h>

extern int yylex();

void yyerror(const char* error) {
  printf("ERROR: %s\n", error);
}
%}

%token DEF CLASS RETURN END DO
%token IF ELSIF ELSE
%token WHILE EACH
%token ATTR_READER ATTR_WRITER ATTR_ACCESSOR
%token OPEN_COMMENT CLOSE_COMMENT
%token PUTS NEW
%token IDENTIFIER SYMBOL INST_VAR
%token OP_EQUAL
%token NOT OP_CMP_AND OP_CMP_OR OP_CMP_EQ OP_CMP_NEG OP_CMP_LE OP_CMP_GT OP_CMP_LE_EQ OP_CMP_GT_EQ
%token OP_PLUS OP_MINUS OP_MUL OP_DIV OP_PLUS_EQ OP_LE_EQ OP_MUL_EQ OP_DIV_EQ
%token L_PAREN R_PAREN L_BRACE R_BRACE L_SQ_BRACK R_SQ_BRACK
%token HASH DOT COMMA SEMI_COLON NL

%start program

%%

program   : IF expr END      { printf("IF expr END\n"); }
          | WHILE expr END   { printf("WHILE expr END\n"); }
          ;

expr      : NOT expr         { printf("NOT expr\n"); }
          | DO               { printf("DO\n"); }
          ;

%%

int main(int argc, char** argv) {
  yyparse();

  return 0;
}
