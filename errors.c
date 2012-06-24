#include "errors.h"
#include <stdio.h>
#include <stdlib.h>

int type_error(char* type_one, char* type_two) {
  
  printf("TypeError: %s can't be coerced into %s\n", type_two, type_one);
  exit(1);

};

int no_method_error(char* method, char* class) {
  
  printf("NoMethodError: undefined method `%s' for %s\n", method, class);
  exit(1);

};