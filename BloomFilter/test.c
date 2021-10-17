#include <assert.h>

#include "filter.h"

// $ gcc test.c -lm -o test
int main() {
  static BaseBloomFilter stBloomFilter = {0};

  InitBloomFilter(&stBloomFilter, 0, 100000, 0.00001);
  uint32_t Value = 32;
  int Ret = 0;
  Ret = BloomFilterAdd(&stBloomFilter, &Value, sizeof(Value));
  assert(Ret == 0);
  Value = 42;
  Ret = BloomFilterCheck(&stBloomFilter, &Value, sizeof(Value));
  assert(Ret == 1);
  FreeBloomFilter(&stBloomFilter);
  return 0;
}