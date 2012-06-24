#ifndef ERRORS_H
#define ERRORS_H

/*****************************************/
/*********** ERROR MESSAGES  *************/
/*****************************************/

#include "structures.h"

int type_error(int, int);
int no_method_error(char*, struct ast*);

#endif