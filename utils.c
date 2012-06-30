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