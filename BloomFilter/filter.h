#pragma once

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash.h"

#define BYTE_BITS 8
#define MIX_UINT64(v) ((uint64_t)(((v) >> 32) ^ (v)))

#define SETBIT(filter, n) \
  (filter->pstFilter[n / BYTE_BITS] |= (1 << (n % BYTE_BITS)))

#define GETBIT(filter, n) \
  (filter->pstFilter[n / BYTE_BITS] & (1 << (n % BYTE_BITS)))

#pragma pack(1)

typedef struct {
  uint8_t cInitFlag;
  // for memory alignment
  uint8_t cResv[3];

  // n
  uint32_t dwMaxItems;
  // p
  double dProbFalse;
  // m
  uint32_t dwFilterBits;
  // k
  uint32_t dwHashFuncs;

  uint32_t dwSeed;
  // when Add too many times, p will rise
  uint32_t dwCount;

  uint32_t dwFilterSize;
  unsigned char* pstFilter;
  uint32_t* pdwHashPos;
} BaseBloomFilter;

#pragma pack()

void _CalcBloomFilterParam(uint32_t n, double p, uint32_t* pm, uint32_t* pk) {
  uint32_t m, k;

  m = (uint32_t)ceil(-1 * log(p) * n / 0.6185);
  m = (m - m % 64) + 64;

  k = (uint32_t)(0.6931 * m / n);
  ++k;

  *pm = m;
  *pk = k;
}

int InitBloomFilter(BaseBloomFilter* pstBloomFilter, uint32_t dwSeed,
                    uint32_t dwMaxItems, double dProbFalse) {
  if (pstBloomFilter == NULL) return -1;
  if ((dProbFalse <= 0) || (dProbFalse >= 1)) return -2;

  if (pstBloomFilter->pstFilter != NULL) free(pstBloomFilter->pstFilter);
  if (pstBloomFilter->pdwHashPos != NULL) free(pstBloomFilter->pdwHashPos);

  memset(pstBloomFilter, 0, sizeof(BaseBloomFilter));

  pstBloomFilter->dwMaxItems = dwMaxItems;
  pstBloomFilter->dProbFalse = dProbFalse;
  pstBloomFilter->dwSeed = dwSeed;

  _CalcBloomFilterParam(pstBloomFilter->dwMaxItems, pstBloomFilter->dProbFalse,
                        &pstBloomFilter->dwFilterBits,
                        &pstBloomFilter->dwHashFuncs);

  pstBloomFilter->dwFilterSize = pstBloomFilter->dwFilterBits / BYTE_BITS;
  pstBloomFilter->pstFilter =
      (unsigned char*)malloc(pstBloomFilter->dwFilterSize);
  if (pstBloomFilter->pstFilter == NULL) return -100;

  pstBloomFilter->pdwHashPos =
      (uint32_t*)malloc(pstBloomFilter->dwHashFuncs * sizeof(uint32_t));
  if (pstBloomFilter->pdwHashPos == NULL) return -200;

  printf(">>> Init BloomFilter(n=%u, p=%f, m=%u, k=%d), malloc() size=%.2fMB\n",
         pstBloomFilter->dwMaxItems, pstBloomFilter->dProbFalse,
         pstBloomFilter->dwFilterBits, pstBloomFilter->dwHashFuncs,
         (double)pstBloomFilter->dwFilterSize / 1024 / 1024);
  memset(pstBloomFilter->pstFilter, 0, pstBloomFilter->dwFilterSize);
  pstBloomFilter->cInitFlag = 1;
  return 0;
}

int FreeBloomFilter(BaseBloomFilter* pstBloomFilter) {
  if (pstBloomFilter == NULL) return -1;

  pstBloomFilter->cInitFlag = 0;
  pstBloomFilter->dwCount = 0;

  free(pstBloomFilter->pstFilter);
  pstBloomFilter->pstFilter = NULL;
  free(pstBloomFilter->pdwHashPos);
  pstBloomFilter->pdwHashPos = NULL;

  return 0;
}

int ResetBloomFilter(BaseBloomFilter* pstBloomFilter) {
  if (pstBloomFilter == NULL) return -1;
  pstBloomFilter->cInitFlag = 0;
  pstBloomFilter->dwCount = 0;
  return 0;
}

int RealResetBloomFilter(BaseBloomFilter* pstBloomFilter) {
  if (pstBloomFilter == NULL) return -1;
  memset(pstBloomFilter->pstFilter, 0, pstBloomFilter->dwFilterSize);
  pstBloomFilter->cInitFlag = 1;
  pstBloomFilter->dwCount = 0;
  return 0;
}

void bloom_hash(BaseBloomFilter* pstBloomFilter, const void* key, int len) {
  uint32_t dwFilterBits = pstBloomFilter->dwFilterBits;
  uint64_t hash1 = MurmurHash2_64(key, len, pstBloomFilter->dwSeed);
  uint64_t hash2 = MurmurHash2_64(key, len, MIX_UINT64(hash1));

  for (int i = 0; i < (int)pstBloomFilter->dwHashFuncs; ++i)
    pstBloomFilter->pdwHashPos[i] = (hash1 + i * hash2) % dwFilterBits;
}

int BloomFilterAdd(BaseBloomFilter* pstBloomFilter, const void* key, int len) {
  if (pstBloomFilter == NULL || key == NULL || len <= 0) return -1;

  if (pstBloomFilter->cInitFlag != 1) {
    memset(pstBloomFilter->pstFilter, 0, pstBloomFilter->dwFilterSize);
    pstBloomFilter->cInitFlag = 1;
  }

  bloom_hash(pstBloomFilter, key, len);
  for (int i = 0; i < (int)pstBloomFilter->dwHashFuncs; ++i)
    SETBIT(pstBloomFilter, pstBloomFilter->pdwHashPos[i]);

  pstBloomFilter->dwCount++;
  if (pstBloomFilter->dwCount <= pstBloomFilter->dwMaxItems)
    return 0;
  else
    return 1;
}

int BloomFilterCheck(BaseBloomFilter* pstBloomFilter, const void* key,
                     int len) {
  if (pstBloomFilter == NULL || key == NULL || len <= 0) return -1;
  bloom_hash(pstBloomFilter, key, len);

  for (int i = 0; i < (int)pstBloomFilter->dwHashFuncs; ++i)
    if (GETBIT(pstBloomFilter, pstBloomFilter->pdwHashPos[i]) == 0) return 1;
  return 0;
}
