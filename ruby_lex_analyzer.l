%{
#include <stdio.h>
#include "ruby_grammar.tab.h"
#include <string.h>
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

'([^']|\n|\t)*'          { yylval.string = malloc((strlen(yytext)+1)*sizeof(char));
                           strcpy(yylval.string, yytext);
                           return STRING1; }
\"([^"]|\n|\t)*\"        { yylval.string = malloc((strlen(yytext)+1)*sizeof(char));
                           strcpy(yylval.string, yytext);
                           return STRING2; }

[0-9]+\.[0-9]*           { yylval.double_number = atof(yytext);
                           return DOUBLE; }
[0-9]+                   { yylval.int_number = atoi(yytext);
                           return INTEGER; }

"="                      { return OP_EQUAL; }
"+="                     { return OP_PLUS_EQ;   }
"-="                     { return OP_MINUS_EQ;  }
"*="                     { return OP_MUL_EQ;    }
"/="                     { return OP_DIV_EQ;    }
"%="                     { return OP_MODULO_EQ; }
"||"                     { return OP_CMP_OR;    }
"&&"                     { return OP_CMP_AND;   }
"!="                     { return OP_CMP_NEG;   }
"<=>"                    { return OP_CMP_INEQ;  }
"==="                    { return OP_CMP_EQ_EQ; }
"=="                     { return OP_CMP_EQ;    }
"<="                     { return OP_CMP_LE_EQ; }
"<"                      { return OP_CMP_LE;    }
">="                     { return OP_CMP_GT_EQ; }
">"                      { return OP_CMP_GT;    }
"+"                      { return OP_PLUS;      }
"-"                      { return OP_MINUS;     }
"*"                      { return OP_MUL;       }
"/"                      { return OP_DIV;       }
"%"                      { return OP_MODULO;    }
"!"                      { return OP_NOT;       }
"**"                     { return OP_EXP;       }

"nil"                    { return NIL; }
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
:[@]?[a-zA-Z_@][a-zA-Z0-9_]* { yylval.string = malloc((strlen(yytext)+1)*sizeof(char));
                               strcpy(yylval.string, yytext);
                               return SYMBOL; }
[@]?[a-zA-Z_@][a-zA-Z0-9_]* { yylval.string = malloc((strlen(yytext)+1)*sizeof(char));
                              strcpy(yylval.string, yytext);
                              return IDENTIFIER; }

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
