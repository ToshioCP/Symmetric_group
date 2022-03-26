#include <stdio.h>
#include <stdlib.h>
#include "permutation.h"
#include "cayley.h"
#include "list.h"
#include "subset.h"
#include "hash.h"

/* yes=>1, no->0, error^>-1 */
int
set_is_subgroup (const subset *set) {
  subset *set1;
  int stat;

  if (is_set (set) == 0)
    return -1;
  set1 = set_mul_ss (set, set);
  return (set == set1);
}

/* Create a symmetric group. */

/* 対称群の生成。 */
subset *
set_create_symgr (int degree) {
  int i, f, a[MAX_FACT_DEGREE];

  if (degree <= 0 || degree > MAX_DEGREE)
    return NULL;
  f = fact (degree);
  for (i=0; i<f; ++i)
    a[i] =i;
  return set_create_set (degree, f, a);
}

/* This is similar to set_union function. */
/* But the second set has only one element that doesn't belong to the first set. */
/* If the second argument belongs to the set of the first argument, an error will be returned. */

/* これは set_union 関数に似た関数である。 */
/* だが、2番目の引数が集合ではなく、1つの要素である。その要素は1番目の引数で与えられる集合の要素ではない。 */
/* もしそれが要素であったら、エラーが返される。 */
subset *
set_append (const subset *set, const int i) {
  int degree, size, *array, f, k, n;
  int p[MAX_FACT_DEGREE], *q;

  if (is_set (set) == 0)
    return NULL;
  degree = set_get_degree (set);
  size = set_get_size (set);
  array = set_get_array (set);
  f = fact(degree);
  if (i < 0 || i >= f)
    return NULL;
  if (size > 0) /* not empty */
    for (k=0; k<size; ++k)
      if (*(array+k) == i)
        return NULL;
  n = size + 1;
  q = p;
  if (n == 1) /* set is empty */
    *q = i;
  else {
    if (i < *array)
      *q++ = i;
    for (k=0; k<size; ++k) {
      if (k > 0 && *(array+k-1) < i && i < *(array+k))
        *q++ = i;
      *q++ = *(array+k);
    }
    if (*(array+size-1) < i)
      *q++ = i;
  }
  return set_create_set (degree, n, p);
}

subset *
set_remove (subset *set, const int i) {
  int degree, size, *array, f, k, l, n;
  int p[MAX_FACT_DEGREE];

  if (is_set (set) == 0)
    return NULL;
  degree = set_get_degree (set);
  size = set_get_size (set);
  array = set_get_array (set);
  f = fact(degree);
  if (i < 0 || i >= f)
    return NULL;
  if (set_include (set, i) != 1 || size == 0)
    return set;
  n = size - 1;
  if (n == 0)
    return set_create_set (degree, 0, NULL);
  for (k=l=0; k<size && l<n; ++k)
    if (*(array+k) != i) {
      p[l] = *(array+k);
      ++l;
    }
  if (l != n)
    return NULL;
  return set_create_set (degree, n, p);
}

/* Return the pointer of the group generated from the set. */
/* The returned pointer will have to be freed when it becomes useless. */

/* 集合から生成された群へのポインタを返す */
/* ポインタは不要になったら（メモリを）解放すること */
subset *
set_gen_group (subset *set) {
  int stat;
  subset *set1, *set2, *set3;

  if (is_set (set) == 0)
    return NULL;
  for (set1 = set; ; ) {
    if ((set2 = set_mul_ss (set1, set1)) == NULL) /* error */
      return NULL;
    if (set1 == set2)
      break;
    set3 = set_union (set1, set2);
    set1 = set3;
  }
  return set2;
}

static int
int_array_cmp (int *a, int *b) {
  return *a - *b;
}

subset *
gen_cyclic_group (int degree, int i) {
  int j, k, f;
  int p[MAX_FACT_DEGREE];

  if (degree < 1 || degree > MAX_DEGREE)
    return NULL;
  if (i < 0 || i >= fact (degree))
    return NULL;
  f = fact(degree); /* f can be smaller, but I couldn't find it. */
  p[0] = 0;
  for (j=i,k=1; j!=0 && k<f; j=mul_pp(degree, j, i), ++k)
    p[k] = j;
  if (j != 0)
    return NULL;
  qsort ((void *)p, k, sizeof(int), (int (*)(const void *, const void *)) int_array_cmp);
  return set_create_set (degree, k, p);
}

static list **ht = NULL;

/* find intermediate groups between symgr and set. */

/* symgrとsetの中間群を探す */
static void
find_intermediate_group (const int n, list *cgroups_0) {
  int i, j;
  subset *set1, *set2, *set3;
  list *l1, *l2, cgroups = {NULL, NULL};

/*printf ("%d\n", l_size(cgroups_0));*/
  if (l_size (cgroups_0) == 1)
    return;
  if (cgroups_0 == NULL || cgroups_0->next == NULL)
    return;
  for (l1=cgroups_0; l1->next!=NULL; l1=l1->next)
    for (l2=l1->next; l2->next!=NULL; l2=l2->next) {
      if ((set1 = set_union (l1->next->o, l2->next->o)) == NULL) {
        l_free_all (&cgroups);
        return;
      }
      if ((set2 = set_gen_group(set1)) == NULL) {
        l_free_all (&cgroups);
        return;
      }
      if (h_lookup (ht, set2) == NULL) {
        h_install (ht, set2);
        l_append_s (&cgroups, set2);
      }
    }
  find_intermediate_group (n, &cgroups);
  l_free_all (&cgroups);
}

/* Find all the subgroups of the n-degree symmetric group. */

/* n次対称群のすべての部分群を求める。 */
void
find_subgroups (const int n) {
  int a[1], i, f;
  subset *set1, *symgr, *set2;
  list cgroups = {NULL, NULL}, *l;

  if (n < 1 || n > MAX_DEGREE)
    return;
  if ((symgr = set_create_symgr (n)) == NULL)
    return;
  if (ht == NULL)
    ht = h_init ();
  else
    h_reset (ht);
  h_install (ht, symgr);
  a[0] = 0;
  if ((set1 = set_create_set (n, 1, a)) == NULL)
    return;
  h_install (ht, set1);
  f = fact (n);
  for (i=1; i<f; ++i) {
    if ((set1 = gen_cyclic_group (n, i)) == NULL) {
      l_free_all (&cgroups);
      return;
    }
    if (h_lookup (ht, set1) == NULL) {
      h_install (ht, set1);
      l_append_s (&cgroups, set1);
    }
  }
  find_intermediate_group (n, &cgroups);
  l_free_all (&cgroups);
}

int
n_subgroups (void) {
  return h_size (ht);
}

static void
each_print_c (subset *set) {
  printf ("%s\n",set_to_s_c (set));
}

void
print_subgroups (void) {
  h_each (ht, (void (*) (void *)) each_print_c);
}
