#ifndef __LIST_H__
#define __LIST_H__

typedef struct _list list;
struct _list {
  list *next;
  void *o; /* an object */
};

/*
How to use.

list start = {NULL, NULL}; # start is not an element of the list.

l_append (&start, set1);

Then the list is like this.
start => set1 => NULL
*/

void *
l_lookup (list *start, const void *o);

void *
l_lookup_with_cmp (list *start, const void *o, int (*cmp) (const void *, const void *));

void *
l_append (list *start, void *o);

/* Append but only if the o doesn't exist in the list. */
void *
l_append_s (list *start, void *o);

void *
l_remove (list *start, void *o);

/* Free all the list after l */
/* Usually, it is used like this. */
/* l_free_all (&start); */
void
l_free_all (list *l);

void
l_free_full_all (list *l, void (*free_o) (void *));

/* Count the elements after l. */
/* Usually, it is used like this. */
/* l_size (&start); */
/* error => -1 */
int
l_size (list *l);

/* Create an array of elements of the list. */
/* The old list is freed when it is successfully converted. */

/* リストの要素からなる配列を返す。 */
/* 変換後にリストのメモリは解放される。 */
void **
l_l2a (list *l);

void
l_sort (list *l, int (*l_cmp)(const void *, const void *));

void
l_each (list *l, void (* func) (void *));

#endif /* __LIST_H__ */
