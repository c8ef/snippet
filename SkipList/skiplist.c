#include "skiplist.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Node *create_node(int level, keyType key, valueType value) {
  Node *p = NEW_NODE(level);
  if (!p) return NULL;
  p->key = key;
  p->value = value;
  return p;
}

skiplist *create_sl() {
  skiplist *sl = (skiplist *)malloc(sizeof(skiplist));
  if (sl == NULL) return NULL;

  sl->level = 0;
  Node *h = create_node(MAX_L - 1, 0, 0);
  if (h == NULL) {
    free(sl);
    return NULL;
  }

  sl->head = h;
  for (int i = 0; i < MAX_L; ++i) h->next[i] = NULL;
  srand(time(0));
  return sl;
}

int randomLevel() {
  int level = 1;
  while (rand() % 2) ++level;
  level = (MAX_L > level) ? level : MAX_L;
  return level;
}

bool insert(skiplist *sl, keyType key, valueType value) {
  Node *update[MAX_L];
  Node *q = NULL, *p = sl->head;

  for (int i = sl->level - 1; i >= 0; --i) {
    while ((q = p->next[i]) && q->key < key) p = q;
    update[i] = p;
  }

  if (q && q->key == key) {
    q->value = value;
    return true;
  }

  int level = randomLevel();
  if (level > sl->level) {
    for (int i = sl->level; i < level; ++i) update[i] = sl->head;
  }
  sl->level = level;

  q = create_node(level, key, value);
  if (!q) return false;
  for (int i = level - 1; i >= 0; --i) {
    q->next[i] = update[i]->next[i];
    update[i]->next[i] = q;
  }
  return true;
}

bool erase(skiplist *sl, keyType key) {
  Node *update[MAX_L];
  Node *q = NULL, *p = sl->head;

  for (int i = sl->level - 1; i >= 0; --i) {
    while ((q = p->next[i]) && q->key < key) {
      p = q;
    }
    update[i] = p;
  }

  if (!q || (q && q->key != key)) return false;

  for (int i = sl->level - 1; i >= 0; --i) {
    if (update[i]->next[i] == q) {
      update[i]->next[i] = q->next[i];
      if (sl->head->next[i] == NULL) sl->level--;
    }
  }

  free(q);
  q = NULL;
  return true;
}

valueType *search(skiplist *sl, keyType key) {
  Node *q = NULL, *p = sl->head;
  for (int i = sl->level - 1; i >= 0; --i) {
    while ((q = p->next[i]) && q->key < key) {
      p = q;
    }
    if (q && q->key == key) return &(q->value);
  }
  return NULL;
}

void print_sl(skiplist *sl) {
  Node *q;
  for (int i = sl->level - 1; i >= 0; --i) {
    q = sl->head->next[i];
    printf("level %d:\n", i + 1);
    while (q) {
      printf("key: %d value: %d\t", q->key, q->value);
      q = q->next[i];
    }
    printf("\n");
  }
}

void sl_free(skiplist *sl) {
  if (!sl) return;

  Node *q = sl->head;
  Node *next;

  while (q) {
    next = q->next[0];
    free(q);
    q = next;
  }
  free(sl);
}