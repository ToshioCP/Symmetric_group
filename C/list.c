/* list.c */

#include <stdlib.h>

#include "list.h"
#include "subset.h"

void *
l_lookup (const list *start, const void *o) {
  list const *l;

  if (start == NULL)
    return NULL;
  for (l=start; l->next!=NULL; l=l->next)
    if (l->next->o == o)
      break;
  if (l->next == NULL)
    return NULL;
  else
    return l->next->o;
}

void *
l_lookup_with_cmp (const list *start, const void *o, int (*cmp) (const void *, const void *)) {
  list const *l;

  if (start == NULL)
    return NULL;
  for (l=start; l->next!=NULL; l=l->next)
    if ((*cmp) (l->next->o, o) == 0)
      break;
  if (l->next == NULL)
    return NULL;
  else
    return l->next->o;
}

void *
l_append (list *start, void *o) {
  list *l;
  void *o1;

  if (start == NULL)
    return NULL;
  for (l=start; l->next!=NULL; l=l->next)
    ;
  l->next = (list *) malloc (sizeof(list));
  l->next->next = NULL;
  l->next->o = o;
  return l->next->o;
}

/* Append but only if the o doesn't exist in the list. */
void *
l_append_s (list *start, void *o) {
  list *l;
  void *o1;

  if (start == NULL)
    return NULL;
  if ((o1 = l_lookup (start, o)) == o)
    return o;
  return l_append(start, o);
}

/* prepend is a bit faster than append */
void *
l_prepend (list *start, void *o) {
  list *l;
  void *o1;

  if (start == NULL)
    return NULL;
  l = (list *) malloc (sizeof(list));
  l->next = start->next;
  l->o = o;
  start->next = l;
  return l->o;
}

void *
l_remove (list *start, void *o) {
  list *l, *l1;

  for (l=start; l->next != NULL; l=l->next)
    if (l->next->o == o)
      break;
  if (l->next == NULL) /* not found */
    return NULL;
  l1 = l->next;
  l->next = l1->next;
  free (l1);
  return o;
}

/* Free all the list after l */
/* Usually, it is used like this. */
/* l_free_all (&start); */
void
l_free_all (list *l) {
  if (l == NULL || l->next == NULL)
    return;
  else {
    l_free_all (l->next);
    free (l->next);
    l->next = NULL;
  }
}

void
l_free_full_all (list *l, void (*free_o) (void *)) {
  if (l == NULL || l->next == NULL)
    return;
  else {
    l_free_full_all (l->next, free_o);
    (*free_o) (l->next->o);
    free (l->next);
    l->next = NULL;
  }
}

/* Count the elements after l. */
/* Usually, it is used like this. */
/* l_size (&start); */
/* error => -1 */
int
l_size (const list *l) {
  int i;
  const list *l1;

  if (l == NULL)
    return -1;
  for (i=0,l1=l; l1->next!=NULL; l1=l1->next)
    ++i;
  return i;
}

/* Create an array of elements of the list. */
/* The old list is freed when it is successfully converted. */

/* リストの要素からなる配列を返す。 */
/* 変換後にリストのメモリは解放される。 */
void **
l_l2a (list *l) {
  list *l1;
  void **p, **q;
  int n;

  n = l_size (l);
  if (n <= 0)
    return NULL;
  p = q = (void **) malloc (sizeof(void *)*n);
  for (l1=l; --n >= 0 && l1->next != NULL; l1=l1->next)
    *q++ = l1->next->o;
  if (n < 0 && l1->next != NULL) { /* error */
    free (p);
    return NULL;
  }
  l_free_all (l);
  return p;
}

static void
l_a2l (list *l, void **o_array, int n) {
  int i;

  for (i=0; i<n; ++i)
    l_append (l, o_array[i]);
  free (o_array);
}

void
l_sort (list *l, int (*l_cmp)(const void *, const void *)) {
  void **o_array;
  int n;

  n = l_size (l);
  o_array = l_l2a (l);
  qsort ((void *) o_array, (size_t) n, sizeof(void *), l_cmp);
  l_a2l (l, o_array, n);
}

void
l_each (const list *l, void (* func) (void *)) {
  list const *l1;

  for (l1=l; l1->next != NULL; l1=l1->next)
    func (l1->next->o);
}
