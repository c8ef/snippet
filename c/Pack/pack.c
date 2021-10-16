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
  // 5
  printf("%zd\n", sizeof(Demo));
  // 8
  printf("%zd\n", sizeof(Deno));
  return 0;
}