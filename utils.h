#ifndef UTILS_H
#define UTILS_H

/*****************************************/
/********** AUXILIARS METHODS  ***********/
/*****************************************/
#include "structures.h"
#include <stdio.h>
#include <string.h>

int list_length(struct list_node*);
int array_size(void* array[]);
char * build_end_of_lines(char*);

#endif