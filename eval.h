#ifndef EVAL_H
#define EVAL_H

/*****************************************/
/*********** EVAL FUNCTIONS  *************/
/*****************************************/

#include "structures.h"
#include "ast.h"
#include "errors.h"

struct ast* eval_ast(struct ast*);
char* drop_quotes(char*);

#endif