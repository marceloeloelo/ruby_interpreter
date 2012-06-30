#ifndef ERRORS_H
#define ERRORS_H

/*****************************************/
/*********** ERROR MESSAGES  *************/
/*****************************************/

#include "structures.h"

int type_error(int, int);
int no_method_error(char*, struct ast*);
int undefined_variable_error(char*);
int wrong_arguments_error(int, int);
int unexpected_type_error(int);
char* type_name(int node_type);

#endif