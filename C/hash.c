/* list.c */

#include <stdlib.h>
#include "list.h"
#include "hash.h"

extern int hash (const void *o);

list **
h_init (void) {
  int i;
  list **ht;

  ht = (list **) malloc (sizeof(void *)*HASHSIZE);
  for (i=0; i<HASHSIZE; ++i)
    ht[i] = NULL;
  return ht;
}

void *
h_lookup (list **ht, const void *o) {
  int i;
  list start;

  start.next = ht[hash (o)];
  return  l_lookup (&start, o);
}

void *
h_lookup_with_cmp (list **ht, const void *o, int (*cmp) (const void *, const void *)) {
  int i;
  list start;

  start.next = ht[hash (o)];
  return  l_lookup_with_cmp (&start, o, cmp);
}

void
h_install (list **ht, void *o) {
  int i;
  list start;

  if (h_lookup (ht, o) != NULL)
    return;
  start.next = ht[hash (o)];
  l_prepend (&start, o);
  ht[hash (o)] = start.next;
}

void
h_reset (list **ht) {
  int i;
  list start;

  for (i=0; i<HASHSIZE; ++i) {
    if (ht[i] == NULL)
      continue;
    start.next = ht[i];
    l_free_all (&start);
    ht[i] = NULL;
  }
}

void
h_reset_full (list **ht, void (*free_o) (void *)) {
  int i;
  list start;

  for (i=0; i<HASHSIZE; ++i) {
    if (ht[i] == NULL)
      continue;
    start.next = ht[i];
    l_free_full_all (&start, free_o);
    ht[i] = NULL;
  }
}

int
h_size (list **ht) {
  int i, cnt;
  list start;

  for (i=cnt=0; i<HASHSIZE; ++i) {
    if (ht[i] == NULL)
      continue;
    start.next = ht[i];
    cnt += l_size (&start);
  }
  return cnt;
}

void
h_each (list **ht, void (*func) (void *)) {
  int i;
  list start;

  for (i=0; i<HASHSIZE; ++i) {
    if (ht[i] == NULL)
      continue;
    start.next = ht[i];
    l_each (&start, func);
  }
}
