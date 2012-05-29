%{
#include <stdio.h>
#include "y.tab.h"
%}

%%

[ \t]                    { ; }
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
^"=begin"$               { ; }
^"=end"$                 { ; }
[a-zA-Z_][a-zA-Z0-9_]*   { return IDENTIFIER; }
:[a-zA-Z_][a-zA-Z0-9_]*  { return SYMBOL; }
@[a-zA-Z_][a-zA-Z0-9_]*  { return INST_VAR; }
[0-9]+\.[0-9]*           { return DOUBLE; }
[0-9]+                   { return INTEGER; }
"="                      { return OP_EQUAL; }
"!"                      { return OP_NOT; }
"&&"                     { return OP_CMP_AND; }
"||"                     { return OP_CMP_OR; }
"=="                     { return OP_CMP_EQ; }
"!="                     { return OP_CMP_NEG; }
"<"                      { return OP_CMP_LE; }
">"                      { return OP_CMP_GT; }
"<="                     { return OP_CMP_LE_EQ; }
">="                     { return OP_CMP_GT_EQ; }
"+"                      { return OP_PLUS; }
"-"                      { return OP_MINUS; }
"*"                      { return OP_MUL; }
"/"                      { return OP_DIV; }
"%"                      { return OP_MODULO; }
"**"                     { return OP_EXP; }
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
"\n"                     { return NL; }
.                        { ECHO; }

%%

/*main(argc, argv)
int argc;
char** argv;
{
}


*/
