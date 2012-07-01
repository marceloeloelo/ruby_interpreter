%{
#include <stdio.h>
#include "ruby_grammar.tab.h"
#include <string.h>

struct buffer_stack {
  struct buffer_stack* previous;   /* Previous entry.     */
  YY_BUFFER_STATE buff_state;      /* Saved buffer.       */
  int line_number;                 /* Saved line number.  */
  char* file_name;                 /* Name of this file.  */
  FILE* f;                         /* Current file.       */
}* current_buffs = NULL;           /* Current input file. */

char* current_file_name;           /* Name of current input file. */

int open_file(char*);
int close_file();

struct open_file_list {
  char* file_name;
  struct open_file_list* next;
}* opened_files = NULL;

int add_file_to_open_file_list(char*);
int is_file_already_opened(char*);

%}

%option nodefault yylineno
%x COMMENT
%x REQUIRE_INCLUSION
%x LOAD_INCLUSION

%%

[ \t]                           { ;                        }

^require[ \t]*[\"]              { BEGIN REQUIRE_INCLUSION; }

<REQUIRE_INCLUSION>[^\"]+       {
                                   {
                                     int c;
                                     while ((c = input()) && c != '\n') {};
                                   };
                                   yylineno++;
                                   char* fn = strdup(yytext);
                                   if (!is_file_already_opened(fn)) {
                                     if (!open_file(fn)) {
                                         yyterminate();         /* No such file. */
                                     } else {
                                       add_file_to_open_file_list(fn);
                                     }
                                   }
                                   BEGIN INITIAL;
                                 }
<REQUIRE_INCLUSION>.|\n          {
                                   file_bad_inclusion_line_error(yylineno);
                                   yyterminate();
                                 }

^load[ \t]*[\"]                 { BEGIN LOAD_INCLUSION; }

<LOAD_INCLUSION>[^\"]+          {
                                   {
                                     int c;
                                     while ((c = input()) && c != '\n') {};
                                   };
                                   yylineno++;
                                   if (!open_file(strdup(yytext))) {
                                     yyterminate();         /* No such file. */
                                   }
                                   BEGIN INITIAL;
                                 }
<LOAD_INCLUSION>.|\n             {
                                   file_bad_inclusion_line_error(yylineno);
                                   yyterminate();
                                 }


^=begin[ \n]                     { BEGIN COMMENT;             }
<COMMENT>^=end                   { BEGIN INITIAL;             }
<COMMENT>.|\n                    { ;                          }
<COMMENT><<EOF>>                 { return yyerror("comment"); }

#.*                              { ;                          }

'([^']|\n|\t)*'                  {
                                   yylval.string = malloc((strlen(yytext)+1)*sizeof(char));
                                   strcpy(yylval.string, yytext);
                                   return STRING1;
                                 }
\"([^"]|\n|\t)*\"                {
                                   yylval.string = malloc((strlen(yytext)+1)*sizeof(char));
                                   strcpy(yylval.string, yytext);
                                   return STRING2;
                                 }

[0-9]+\.[0-9]*                   {
                                   yylval.double_number = atof(yytext);
                                   return DOUBLE;
                                 }
[0-9]+                           {
                                   yylval.int_number = atoi(yytext);
                                   return INTEGER;
                                 }

"="                              { return OP_EQUAL;      }
"+="                             { return OP_PLUS_EQ;    }
"-="                             { return OP_MINUS_EQ;   }
"*="                             { return OP_MUL_EQ;     }
"/="                             { return OP_DIV_EQ;     }
"%="                             { return OP_MODULO_EQ;  }
"||"                             { return OP_CMP_OR;     }
"&&"                             { return OP_CMP_AND;    }
"!="                             { return OP_CMP_NEG;    }
"<=>"                            { return OP_CMP_INEQ;   }
"==="                            { return OP_CMP_EQ_EQ;  }
"=="                             { return OP_CMP_EQ;     }
"<="                             { return OP_CMP_LE_EQ;  }
"<"                              { return OP_CMP_LE;     }
">="                             { return OP_CMP_GT_EQ;  }
">"                              { return OP_CMP_GT;     }
"+"                              { return OP_PLUS;       }
"-"                              { return OP_MINUS;      }
"*"                              { return OP_MUL;        }
"/"                              { return OP_DIV;        }
"%"                              { return OP_MODULO;     }
"!"                              { return OP_NOT;        }
"**"                             { return OP_EXP;        }

"nil"                            { return NIL;           }
"def"                            { return DEF;           }
"do"                             { return DO;            }
"return"                         { return RETURN;        }
"class"                          { return CLASS;         }
"if"                             { return IF;            }
"else"                           { return ELSE;          }
"elsif"                          { return ELSIF;         }
"end"                            { return END;           }
"while"                          { return WHILE;         }
"case"                           { return CASE;          }
"when"                           { return WHEN;          }
"then"                           { return THEN;          }
"attr_reader"                    { return ATTR_READER;   }
"attr_writter"                   { return ATTR_WRITTER;  }
"attr_accessor"                  { return ATTR_ACCESSOR; }
"true"                           { return TRUE_BOOL;     }
"false"                          { return FALSE_BOOL;    }

:[@]?[a-zA-Z_@][a-zA-Z0-9_]*     {
                                   yylval.string = malloc((strlen(yytext)+1)*sizeof(char));
                                   strcpy(yylval.string, yytext);
                                   return SYMBOL;
                                 }
[A-Z_@][a-zA-Z0-9_]*             {
                                   yylval.string = malloc((strlen(yytext)+1)*sizeof(char));
                                   strcpy(yylval.string, yytext);
                                   return CONSTANT;
                                 } 
[@]?[a-zA-Z_@][a-zA-Z0-9_]*      {
                                   yylval.string = malloc((strlen(yytext)+1)*sizeof(char));
                                   strcpy(yylval.string, yytext);
                                   return IDENTIFIER;
                                 }        

"("                              { return L_PAREN;                        }
")"                              { return R_PAREN;                        }
"{"                              { return L_BRACE;                        }
"}"                              { return R_BRACE;                        }
"["                              { return L_SQ_BRACK;                     }
"]"                              { return R_SQ_BRACK;                     }
"."                              { return DOT;                            }
","                              { return COMMA;                          }
";"                              { return SEMI_COLON;                     }
"?"                              { return OP_QUESTION;                    }
"|"                              { return PIPE;                           }
"\n"                             { return NL;                             }
.                                { invalid_token_error(yylineno, yytext); }

<<EOF>>                          {
                                   if (!close_file()) {
                                     yyterminate();
                                   }
                                 }

%%

int open_file(char* file_name) {
  FILE* f = fopen(strdup(file_name), "r");
  struct buffer_stack* bs = malloc(sizeof(struct buffer_stack));

  if (!f) {               /* File not found. */
    file_load_error(file_name);
    return 0;
  };

  if (!bs) {              /* No more free memory. */
    exit(1);
  };

  if (current_buffs) {  /* Remember state. */
    current_buffs->line_number = yylineno;
  };
  bs->previous = current_buffs;

  bs->buff_state = yy_create_buffer(f, YY_BUF_SIZE); /* Set up current entry. */
  bs->f = f;
  bs->file_name = strdup(file_name);
  yy_switch_to_buffer(bs->buff_state);
  current_buffs = bs;
  yylineno = 1;
  current_file_name = strdup(file_name);
  return 1;
};

int close_file() {
  struct buffer_stack* bs = current_buffs;
  struct buffer_stack* previous_bs;

  if (!bs) {
    return 0;
  };

  fclose(bs->f);                    /* Get rid of current entry. */
  yy_delete_buffer(bs->buff_state);

  previous_bs = bs->previous;       /* Switch back to previous.  */
  free(bs);

  if (!previous_bs) {
    return 0;
  };

  yy_switch_to_buffer(previous_bs->buff_state);
  current_buffs = previous_bs;
  yylineno = current_buffs->line_number;
  current_file_name = strdup(current_buffs->file_name);
  return 1;
};

int add_file_to_open_file_list(char* file_name) {
  struct open_file_list* ofl = malloc(sizeof(struct open_file_list));
  ofl->file_name = strdup(file_name);
  ofl->next = opened_files;
  opened_files = ofl;
  return 1;
};

int is_file_already_opened(char* file_name) {
  struct open_file_list* ofl = opened_files;
  int found = 0;
  while ((!found) && (ofl != NULL)) {
    found = (strcmp(ofl->file_name, file_name) == 0);
    ofl = ofl->next;
  };
  if (found) {
    return 1;
  } else {
    return 0;
  };
};

int yywrap(void) {
};
