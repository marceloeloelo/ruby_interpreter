#ifndef ERRORS_H
#define ERRORS_H

/*****************************************/
/*********** ERROR MESSAGES  *************/
/*****************************************/

#include "structures.h"
#include "ast.h"

int type_error(int, int);
int no_method_error(char*, struct ast*);
int undefined_variable_error(char*);
int undefined_method_error(char*, char*);
int wrong_arguments_error(int, int);
int unexpected_type_error(int);
char* type_name(int node_type);
int zero_division_error();
int file_load_error(char*);
int file_bad_inclusion_line_error(int);
int memory_out_of_space_error();
int invalid_token_error(int, char*);
int uninitialized_constant_error(char*);
int interpolation_error();

#endif
