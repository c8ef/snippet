#include <stdio.h>

#include "skiplist.h"

int main() {
  skiplist *sl = create_sl();
  for (int i = 1; i < 1111; ++i) {
    insert(sl, i, i);
  }
  for (int i = 111; i < 1111; ++i) {
    if (!erase(sl, i)) printf("NO!\n");
  }
  print_sl(sl);
  int *p = search(sl, 10);
  if (p) printf("FOUND!\n");
  sl_free(sl);
  return 0;
}