%{
  #include <stdio.h>
%}

%token IDENTIFIER
%token OP_ASIGN
%token STRING
%token OPERATION
%token VAR_NAME
%token SYMBOL

/* %start program */

%%

/* program */
literal  : symbol
         | string
         ;

term     : ';'
         | '\n'
         ;

%%
