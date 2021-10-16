#include <stdio.h>

#pragma pack(1)
typedef struct demo {
  char c;
  int i;
} Demo;
#pragma pack()

typedef struct deno {
  char c;
  int i;
} Deno;

int main() {
  printf("%zd\n", sizeof(Demo));
  printf("%zd\n", sizeof(Deno));
  return 0;
}