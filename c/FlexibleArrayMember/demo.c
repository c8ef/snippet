#include <stdio.h>
#include <stdlib.h>

// C struct data types may end with a flexible array member[1]
// with no specified size
typedef struct array {
  int len;
  int arr[];
} array;

int main() {
  int len = 10;
  printf("%zd\n", sizeof(array));
  // It is common to allocate
  // sizeof(struct) + array_len*sizeof(array element) bytes
  array *p = (array *)malloc(sizeof(array) + len * sizeof(int));
  printf("%zd\n", sizeof(array));
  free(p);
  return 0;
}