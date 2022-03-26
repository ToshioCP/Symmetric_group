#include <stdio.h>
#include "list.c"

void
each_test_1 (subset *set) {
  static int i=0;
  int a[6][6] = {
    {0},
    {0,1},
    {1,3,5},
    {0,15,23},
    {30,60,80,119},
    {0,2,7,13,200,719}
  };
  int b[6] = {1,2,3,3,4,6};
  subset *set1;

  set1 = set_create_set (i+1,b[i],a[i]);
  if (set_cmp (set, set1) != 0)
    printf ("l_each didn't work. (each_test_1 - %d)\n", i);
  ++i;
}

int
set_cmp_p (subset **a, subset **b) {
  return set_cmp (*a, *b);
}

void
test() {
  list start = {NULL, NULL}, *l;
  list *subset_start;
  subset *ss[6], *p;
  int a[6][6] = {
    {0},
    {0,1},
    {1,3,5},
    {0,15,23},
    {30,60,80,119},
    {0,2,7,13,200,719}
  };
  int b[6] = {1,2,3,3,4,6};
  int c[6] = {0,1,5,3,2,4}; /* sorted */
  int i, j, *ar;
  subset **sarray;

  for (i=0; i<6; ++i)
    if ((ss[i] = set_create_set (i+1,b[i],a[i])) == NULL)
      printf ("set_create_set didn't work. %d\n", i);
printf ("l_append & l_lookup\n");
  for (i=0; i<6; ++i)
    if (l_append (&start, ss[i]) != ss[i])
      printf ("l_append didn't work. 1-%d\n", i);
  for (i=0; i<6; ++i)
    if (l_lookup (&start, ss[i]) != ss[i])
      printf ("l_lookup didn't work. 1-%d\n", i);
printf ("l_lookup_with_cmp\n");
  ar = (int*) malloc (sizeof(int)*3);
  *ar = 0, *(ar+1) = 15; *(ar+2) = 23;
  p = (subset *) malloc (sizeof(subset));
  p->degree = 4;
  p->n = 3;
  p->a = ar;
  if (set_cmp (ss[3], p) != 0)
    printf ("set_cmp didn't work. 1\n");
  if (l_lookup_with_cmp (&start, p, (int (*) (const void *, const void *)) set_cmp) != ss[3])
    printf ("l_lookup_with_cmp didn't work. 1\n");
  *(p->a) = 1;
  if (l_lookup_with_cmp (&start, p, (int (*) (const void *, const void *)) set_cmp) != NULL)
    printf ("l_lookup_with_cmp didn't work. 2\n");
  free (ar);
  free (p);
  l_free_all (&start);
printf ("l_append_s\n");
  for (i=0; i<6; ++i)
    if (l_append_s (&start, ss[i]) != ss[i])
      printf ("l_append_s didn't work. 2-%d\n", i);
  for (i=0; i<6; ++i)
    if (l_lookup (&start, ss[i]) != ss[i])
      printf ("l_lookup didn't work. 2-%d\n", i);
  for (i=0; i<6; ++i)
    if (l_append_s (&start, ss[i]) != ss[i])
      printf ("l_append_s didn't work. 3-%d\n", i);
  if (l_size (&start) != 6)
    printf ("l_append_s didn't work. 4\n");
    l_free_all (&start);
printf ("l_prepend\n");
  for (i=0; i<6; ++i)
    if (l_prepend (&start, ss[i]) != ss[i])
      printf ("l_prepend didn't work. 1-%d\n", i);
  for (i=0, l=&start; i<6; ++i, l=l->next)
    if (l==NULL || l->next==NULL || (subset *)(l->next->o) != ss[5-i])
      printf ("l_prepend didn't work. 2-%d\n", i);
printf ("l_remove\n");
  if (l_remove (&start, ss[0]) == NULL)
    printf ("l_remove didn't work. 1\n");
  if (l_remove (&start, ss[0]) != NULL)
    printf ("l_remove didn't work. 2\n");
  if (l_remove (&start, ss[5]) == NULL)
    printf ("l_remove didn't work. 1\n");
  if (l_remove (&start, ss[5]) != NULL)
    printf ("l_remove didn't work. 2\n");
printf ("l_free_all\n");
  l_free_all (&start);
  if (start.next != NULL)
    printf ("l_free_all didn't work. 1\n");
printf ("l_size\n");
  if (l_size (&start) != 0)
      printf ("l_size didn't work. 1\n");
  if (l_append (&start, ss[0]) != ss[0])
      printf ("l_append didn't work. 5\n");
  if (l_size (&start) != 1)
      printf ("l_size didn't work. 2\n");
  for (i=0; i<6; ++i)
    if (l_append_s (&start, ss[i]) != ss[i])
      printf ("l_append_s didn't work. 5-%d\n", i);
  if (l_size (&start) != 6)
      printf ("l_size didn't work. 3\n");
  l_free_all (&start);
printf ("l_free_full_all\n");
  for (i=0; i<6; ++i)
    if (l_append (&start, ss[i]) != ss[i])
      printf ("l_append didn't work. 6-%d\n", i);
  l_free_full_all (&start, (void (*) (void *)) set_free_set0);
  if (start.next != NULL)
    printf ("l_free_full_all didn't work. 1\n");
printf ("l_l2a\n");
  for (i=0; i<6; ++i)
    if ((ss[i] = set_create_set (i+1,b[i],a[i])) == NULL)
      printf ("set_create_set didn't work. 2-%d\n", i);
  for (i=0; i<6; ++i)
    if (l_append (&start, ss[i]) != ss[i])
      printf ("l_append didn't work. 7-%d\n", i);
  if ((sarray = (subset **) l_l2a (&start)) == NULL)
    printf ("l_l2a didn't work. 1\n");
  for (i=0; i<6; ++i)
    if (is_set (*(sarray+i)) != 1)
      printf ("l_l2a didn't work. 2-%d\n", i);
  for (i=0; i<6; ++i)
    if (set_cmp (*(sarray+i), ss[i]) != 0)
      printf ("l_l2a didn't work. 3-%d\n", i);
  if (l_size (&start) != 0)
    printf ("l_l2a didn't work. 3\n");
printf ("l_a2l\n");
  l_a2l (&start, (void **) sarray, 6);
  for (i=0, l=&start; i<6 && l->next!=NULL; ++i, l=l->next)
    if ((subset *) l->next->o != ss[i])
      printf ("l_a2l didn't work. 1-%d\n", i);
  if (i != 6)
    printf ("l_a2l didn't work. 2\n");
  l_free_all (&start);
printf ("l_sort\n");
  for (i=0; i<6; ++i)
    if ((ss[i] = set_create_set (6,b[i],a[i])) == NULL)
      printf ("set_create_set didn't work. (l_sort) %d\n", i);
  for (i=0; i<6; ++i)
    if (l_append (&start, ss[i]) != ss[i])
      printf ("l_append didn't work. 8-%d\n", i);
  l_sort (&start, (int (*) (const void *, const void *)) set_cmp_p);
  for (i=0, l=&start; i<6 && l->next!=NULL; ++i, l=l->next)
    if ((subset *)l->next->o != ss[c[i]])
      printf ("l_sort didn't work. 1-%d\n", i);
  if (i != 6)
    printf ("l_sort didn't work. 2\n");
  l_free_all (&start);
printf ("l_each\n");
  for (i=0; i<6; ++i)
    if ((ss[i] = set_create_set (i+1,b[i],a[i])) == NULL)
      printf ("set_create_set didn't work. (i_each) %d\n", i);
  for (i=0; i<6; ++i)
    if (l_append (&start, ss[i]) != ss[i])
      printf ("l_append didn't work. (i_each) %d\n", i);
  l_each (&start, (void (*) (void *)) each_test_1);
  l_free_full_all (&start, (void (*) (void *)) set_free_set0);
}

int
main (int argc, char **argv) {
  test();
}
