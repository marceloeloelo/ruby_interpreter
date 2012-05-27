%{
#include <stdio.h>
#include "y.tab.h"

#define TOKEN(t) (yylval = t)
%}

%%

[ \t]                    { ; }
"def"                    { return TOKEN(DEF); }
"do"                     { return TOKEN(DO); }
"return"                 { return TOKEN(RETURN); }
"class"                  { return TOKEN(CLASS); }
"if"                     { return TOKEN(IF); }
"else"                   { return TOKEN(ELSE); }
"elsif"                  { return TOKEN(ELSIF); }
"end"                    { return TOKEN(END); }
"while"                  { return TOKEN(WHILE); }
"attr_reader"            { return TOKEN(ATTR_READER); }
"attr_writer"            { return TOKEN(ATTR_WRITER); }
"attr_accessor"          { return TOKEN(ATTR_ACCESSOR); }
"each"                   { return TOKEN(EACH); }
"puts"                   { return TOKEN(PUTS); }
"new"                    { return TOKEN(NEW); }
"=begin"                 { return TOKEN(OPEN_COMMENT); }
"=end"                   { return TOKEN(CLOSE_COMMENT); }
[a-zA-Z_][a-zA-Z0-9_]*   { return TOKEN(IDENTIFIER); }
:[a-zA-Z_][a-zA-Z0-9_]*  { return TOKEN(SYMBOL); }
@[a-zA-Z_][a-zA-Z0-9_]*  { return TOKEN(INST_VAR); }
"="                      { return TOKEN(OP_EQUAL); }
"!"                      { return TOKEN(NOT); }
"&&"                     { return TOKEN(OP_CMP_AND); }
"||"                     { return TOKEN(OP_CMP_OR); }
"=="                     { return TOKEN(OP_CMP_EQ); }
"!="                     { return TOKEN(OP_CMP_NEG); }
"<"                      { return TOKEN(OP_CMP_LE); }
">"                      { return TOKEN(OP_CMP_GT); }
"<="                     { return TOKEN(OP_CMP_LE_EQ); }
">="                     { return TOKEN(OP_CMP_GT_EQ); }
"+"                      { return TOKEN(OP_PLUS); }
"-"                      { return TOKEN(OP_MINUS); }
"*"                      { return TOKEN(OP_MUL); }
"/"                      { return TOKEN(OP_DIV); }
"+="                     { return TOKEN(OP_PLUS_EQ); }
"-="                     { return TOKEN(OP_LE_EQ); }
"*="                     { return TOKEN(OP_MUL_EQ); }
"/="                     { return TOKEN(OP_DIV_EQ); }
"("                      { return TOKEN(L_PAREN); }
")"                      { return TOKEN(R_PAREN); }
"{"                      { return TOKEN(L_BRACE); }
"}"                      { return TOKEN(R_BRACE); }
"["                      { return TOKEN(L_SQ_BRACK); }
"]"                      { return TOKEN(R_SQ_BRACK); }
"#"                      { return TOKEN(HASH); }
"."                      { return TOKEN(DOT); }
","                      { return TOKEN(COMMA); }
";"                      { return TOKEN(SEMI_COLON); }
"\n"                     { return TOKEN(NL); }
.                        { ECHO; }

%%

/*main(argc, argv)
int argc;
char** argv;
{
}


*/
