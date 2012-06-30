#ifndef EVAL_H
#define EVAL_H

/*****************************************/
/*********** EVAL FUNCTIONS  *************/
/*****************************************/

#include "structures.h"
#include "ast.h"
#include "native_methods.h"
#include "errors.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


struct ast* eval_ast(struct ast*);
char* drop_quotes(char*);

#endif