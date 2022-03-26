#include <stdio.h>
#include "permutation.h"
#include "list.h"
#include "subset.h"
#include "hash.c"

extern int hash (const void *set);

void test_each(subset *set) {
  int a[MAX_DEGREE], i;
  subset *set1;

  a[0]=10; a[1]=20; a[2]=30;
  set1 = set_create_set (5, 3, a);
  if (set != set1)
    printf ("each didn't work.\n");
}

void
test (void) {
  list **ht;

  int a[MAX_DEGREE], i, j;
  subset *set1, *set2;

printf ("h_init\n");
  ht = h_init ();
  for (i=0; i<HASHSIZE; ++i)
    if (*(ht+i) != NULL) {
      printf ("init_hash didn't work.\n");
      return;
    }
printf ("hash\n");
  a[0]=10; a[1]=20; a[2]=30;
  set1 = set_create_set (5, 3, a);
  i = hash(set1);
  if (i != ((a[0]*31+a[1])*31+a[2]) % HASHSIZE)
    printf ("hash didn't work.\n");
printf ("h_install and h_lookup\n");
  if (h_lookup(ht, set1) != NULL)
    printf ("lookup didn't work.\n");
  h_install (ht, set1);
  if (h_lookup(ht, set1) != set1)
    printf ("lookup didn't work.\n");
  set2 = (subset *) malloc(sizeof(subset));
  set2->degree = 5;
  set2->n = 3;
  set2->a = a;
  if (h_lookup_with_cmp(ht, set2, (int (*) (const void *, const void *)) set_cmp) != set1)
    printf ("h_lookup_with_cmp didn't work.\n");
  free (set1);
printf ("h_reset\n");
  h_reset (ht);
  set1 = set_create_set (5, 3, a);
  if (h_lookup(ht, set1) != NULL)
    printf ("reset_hash didn't work.\n");
printf ("h_size\n");
  if (h_size(ht) != 0)
    printf ("h_size didn't work.\n");
  h_install (ht, set1);
  if (h_size(ht) != 1)
    printf ("h_size didn't work.\n");
printf ("each\n");
  h_each (ht, (void (*) (void *)) test_each);
}

int
main (int argvc, char **argv) {
  test();
}
