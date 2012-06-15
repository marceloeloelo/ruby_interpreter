%{
#include <stdio.h>
#include "ruby_grammar.tab.h"
#include <string.h>

#define T_PLUS_EQ     1
#define T_MINUS_EQ    2
#define T_MUL_EQ      3
#define T_DIV_EQ      4
#define T_MODULO_EQ   5
#define T_CMP_OR      6
#define T_CMP_AND     7
#define T_CMP_NEG     8
#define T_CMP_EQ_EQ   9
#define T_CMP_LE_EQ   10
#define T_CMP_LE      11
#define T_CMP_GT_EQ   12
#define T_CMP_GT      13
#define T_PLUS        14
#define T_MINUS       15
#define T_MUL         16
#define T_DIV         17
#define T_MODULO      18
#define T_NOT         19
#define T_EXP         20
%}

%option nodefault yylineno
%x COMMENT

%%

[ \t]                    { ; }

^=begin[ \n]             { BEGIN COMMENT; }
<COMMENT>^=end           { BEGIN INITIAL; }
<COMMENT>.|\n            { ; }
<COMMENT><<EOF>>         { return yyerror("ERROR: Commentarios"); }

#.* { ; }

'(.|\\\n)*'              { yylval.string = malloc((strlen(yytext)+1)*sizeof(char));
                           strcpy(yylval.string, yytext);
                           return STRING1; }
\"(.|\\\n)*\"            { yylval.string = malloc((strlen(yytext)+1)*sizeof(char));
                           strcpy(yylval.string, yytext);
                           return STRING2; }

[0-9]+\.[0-9]*           { yylval.double_number = atof(yytext);
                           return DOUBLE; }
[0-9]+                   { yylval.int_number = atoi(yytext);
                           return INTEGER; }

"="                      { return OP_EQUAL; }
"+="                     { yylval.bop = T_PLUS_EQ;   return OP_PLUS_EQ;   }
"-="                     { yylval.bop = T_MINUS_EQ;  return OP_MINUS_EQ;  }
"*="                     { yylval.bop = T_MUL_EQ;     return OP_MUL_EQ;    }
"/="                     { yylval.bop = T_DIV_EQ;    return OP_DIV_EQ;    }
"%="                     { yylval.bop = T_MODULO_EQ; return OP_MODULO_EQ; }
"||"                     { yylval.bop = T_CMP_OR;    return OP_CMP_OR;    }
"&&"                     { yylval.bop = T_CMP_AND;   return OP_CMP_AND;   }
"!="                     { yylval.bop = T_CMP_NEG;   return OP_CMP_NEG;   }
"<=>"                    { return OP_CMP_INEQ;                            }
"==="                    { return OP_CMP_EQ_EQ;                           }
"=="                     { yylval.bop = T_CMP_EQ_EQ; return OP_CMP_EQ;    }
"<="                     { yylval.bop = T_CMP_LE_EQ; return OP_CMP_LE_EQ; }
"<"                      { yylval.bop = T_CMP_LE;    return OP_CMP_LE;    }
">="                     { yylval.bop = T_CMP_GT_EQ; return OP_CMP_GT_EQ; }
">"                      { yylval.bop = T_CMP_GT;    return OP_CMP_GT;    }
"+"                      { yylval.bop = T_PLUS;      return OP_PLUS;      }
"-"                      { yylval.bop = T_MINUS;     return OP_MINUS;     }
"*"                      { yylval.bop = T_MUL;       return OP_MUL;       }
"/"                      { yylval.bop = T_DIV;       return OP_DIV;       }
"%"                      { yylval.bop = T_MODULO;    return OP_MODULO;    }
"!"                      { yylval.bop = T_NOT;       return OP_NOT;       }
"**"                     { yylval.bop = T_EXP;       return OP_EXP;       }

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
"case"                   { return CASE; }
"when"                   { return WHEN; }
"then"                   { return THEN; }
"attr_reader"            { return ATTR_READER; }
"attr_writer"            { return ATTR_WRITER; }
"attr_accessor"          { return ATTR_ACCESSOR; }
:[@]?[a-zA-Z_@][a-zA-Z0-9_]* { return SYMBOL; }
[@]?[a-zA-Z_@][a-zA-Z0-9_]* { return IDENTIFIER; }

"("                      { return L_PAREN; }
")"                      { return R_PAREN; }
"{"                      { return L_BRACE; }
"}"                      { return R_BRACE; }
"["                      { return L_SQ_BRACK; }
"]"                      { return R_SQ_BRACK; }
"."                      { return DOT; }
","                      { return COMMA; }
";"                      { return SEMI_COLON; }
"?"                      { return OP_QUESTION; }
"\n"                     { return NL; }
.                        { printf("ERROR: Line %d, invalid token \"%s\"\n", yylineno, yytext); exit(1); }

%%

int yywrap(void){}
