#pragma once

#include <stdbool.h>

#define MAX_L 16

#define NEW_NODE(n) ((Node *)malloc(sizeof(Node) + n * sizeof(Node *)))

typedef int keyType;
typedef int valueType;

typedef struct node {
  keyType key;
  valueType value;
  struct node *next[1];
} Node;

typedef struct skiplist {
  int level;
  Node *head;
} skiplist;

Node *create_node(int level, keyType key, valueType value);
skiplist *create_sl();
int randomLevel();
bool insert(skiplist *sl, keyType key, valueType value);
bool erase(skiplist *sl, keyType key);
valueType *search(skiplist *sl, keyType key);
void print_sl(skiplist *sl);
void sl_free(skiplist *sl);