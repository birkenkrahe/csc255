#include <stdio.h>
#include <stdlib.h>
#include <string.h>






int get_value(int *array, int index) {
  return array[index];
}
int main() {
  int *array = NULL;
  int value = get_value(array,0); // dereferencing NULL
  printf("Value: %d\n",value);
  return 0;
}
