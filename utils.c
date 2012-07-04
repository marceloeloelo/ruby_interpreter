#include "utils.h"

int list_length(struct list_node* list) {
  int i = 0;
  while (list != NULL) {
    i = i + 1;
    list = list->next;
  };
  return i;
};

int array_size(void* array[]){
	int i = 0;
	while(array[i] != NULL){
		i = i + 1;
	};
	return i;
};

int string_is_in_array(void * array[], char * string){
  int i = 0;
  int encontre = 0;
  while(!encontre && i < array_size(array)) {
    encontre = !strcmp(string, (char *)array[i]);
    i = i + 1;
  }
  return encontre;
}

char * build_end_of_lines(char* str){
  /* convert end of lines in '\n' chars */
  int i = 0;
  int j = i;
  for(i = 0; i < (strlen(str) - 1); i = i + 1){
    if (str[i] == '\\' && str[i+1] == 'n'){
      str[i] = '\n';
      for(j = i + 1; j < (strlen(str)); j = j + 1){
        str[j] = str[j + 1];
      }
    }
  }
  return str;
}