%{
#include <stdio.h>
#include "ruby_grammar.tab.h"
%}

%x COMMENT

%%

[ \t]                    { ; }

^=begin[ \n]             { BEGIN COMMENT; }
<COMMENT>^=end           { BEGIN INITIAL; }
<COMMENT>.|\n            { ; }
<COMMENT><<EOF>>         { return yyerror("ERROR: Commentarios"); }

'(.|\n|\t)*'             { return STRING1; }
\"(.|\n|\t)*\"           { return STRING2; }

[0-9]+\.[0-9]*           { return DOUBLE; }
[0-9]+                   { return INTEGER; }

"+"                      { return OP_PLUS; }
"-"                      { return OP_MINUS; }
"*"                      { return OP_MUL; }
"/"                      { return OP_DIV; }
"%"                      { return OP_MODULO; }
"**"                     { return OP_EXP; }

"="                      { return OP_EQUAL; }

"nil"                    { return NIL; }
"self"                   { return SELF; }
"def"                    { return DEF; }
"do"                     { return DO; }
"return"                 { return RETURN; }
"class"                  { return CLASS; }
"if"                     { return IF; }
"else"                   { return ELSE; }
"elsif"                  { return ELSIF; }
"end"                    { return END; }
"while"                  { return WHILE; }
"attr_reader"            { return ATTR_READER; }
"attr_writer"            { return ATTR_WRITER; }
"attr_accessor"          { return ATTR_ACCESSOR; }
"each"                   { return EACH; }
"puts"                   { return PUTS; }
"new"                    { return NEW; }
[a-zA-Z_][a-zA-Z0-9_]*   { return IDENTIFIER; }
:[a-zA-Z_][a-zA-Z0-9_]*  { return SYMBOL; }
@[a-zA-Z_][a-zA-Z0-9_]*  { return INST_VAR; }





"!"                      { return OP_NOT; }
"&&"                     { return OP_CMP_AND; }
"||"                     { return OP_CMP_OR; }
"=="                     { return OP_CMP_EQ; }
"!="                     { return OP_CMP_NEG; }
"<"                      { return OP_CMP_LE; }
">"                      { return OP_CMP_GT; }
"<="                     { return OP_CMP_LE_EQ; }
">="                     { return OP_CMP_GT_EQ; }
"+="                     { return OP_PLUS_EQ; }
"-="                     { return OP_LE_EQ; }
"*="                     { return OP_MUL_EQ; }
"/="                     { return OP_DIV_EQ; }
"("                      { return L_PAREN; }
")"                      { return R_PAREN; }
"{"                      { return L_BRACE; }
"}"                      { return R_BRACE; }
"["                      { return L_SQ_BRACK; }
"]"                      { return R_SQ_BRACK; }
"#"                      { return HASH; }
"."                      { return DOT; }
","                      { return COMMA; }
";"                      { return SEMI_COLON; }
"?"                      { return OP_QUESTION; }
"\n"                     { return NL; }
.                        { ECHO; }

%%

