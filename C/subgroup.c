#include <stdio.h>
#include <stdlib.h>
#include "permutation.h"
#include "subset.h"

/* yes=>1, no->0, error^>-1 */
int
set_is_subgroup (const subset *set) {
  subset *set1;
  int stat;

  if (is_set (set) == 0)
    return -1;
  set1 = set_mul_ss (set, set);
  stat = set_cmp (set, set1) == 0 ? 1 : 0;
  free (set1);
  return stat;
}

/* Create a symmetric group. */

/* 対称群の生成。 */
subset *
set_create_symgr (int degree) {
  int i, f, *a;

  f = fact (degree);
  a = (int *) malloc (sizeof(int)*f);
  for (i=0; i<f; ++i)
    *(a+i) =i;
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
  int degree, size, *array, f, k, n, *a, *b;
  subset *set1;

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
  a = b = (int *) malloc (sizeof(int)*n);
  if (n == 1) /* set is empty */
    *a = i;
  else {
    if (i < *array)
      *a++ = i;
    for (k=0; k<size; ++k) {
      if (k > 0 && *(array+k-1) < i && i < *(array+k))
        *a++ = i;
      *a++ = *(array+k);
    }
    if (*(array+size-1) < i)
      *a++ = i;
  }
  set1 = set_create_set (degree, n, b);
  free (b);
  return set1;
}

subset *
set_remove (subset *set, const int i) {
  int degree, size, *array, f, k, l, n, *p;
  subset *set1;

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
  p = (int *) malloc (sizeof(int)*n);
  for (k=l=0; k<size && l<n; ++k)
    if (*(array+k) != i) {
      *(p+l) = *(array+k);
      ++l;
    }
  if (l != n) {
    free (p);
    return NULL;
  }
  set1 = set_create_set (degree, n, p);
  free (p);
  return set1;
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
    if ((stat = set_cmp (set1, set2)) == -2) /* error */
      return NULL;
    if (stat == 0)
      break;
    set3 = set_union (set1, set2);
    set1 = set3;
  }
  return set2;
}


typedef struct _list list;
struct _list {
  list *next;
  subset *set;
};

list list_start = {NULL, NULL};

subset *
list_lookup (const subset *set) {
  list *l;

  for (l=&list_start; l->next != NULL; l=l->next)
    if (l->next->set == set)
      break;
  if (l->next == NULL)
    return NULL;
  else
    return l->next->set;
}

subset *
list_append (subset *set) {
  list *l;
  subset *set1;

  if ((set1 = list_lookup (set)) != NULL)
    return set1;
  for (l=&list_start; l->next != NULL; l=l->next)
    ;
  l->next = (list *) malloc (sizeof(list));
  l->next->next = NULL;
  l->next->set = set;
  return l->next->set;
}

static void
list_free_all0 (list *l) {
  if (l == NULL || l->next == NULL)
    return;
  else {
    list_free_all0 (l->next);
    free (l->next);
    l->next = NULL;
  }
}

void
list_free_all (void) {
  list_free_all0 (&list_start);
}

/* following functions free all the lists and subgroups. */

/* 以下の関数はリスト自身だけでなく、subgroupも解放することに注意 */
static void
list_free_full_all0 (list *l) {
  if (l == NULL || l->next == NULL)
    return;
  else {
    list_free_full_all0 (l->next);
    set_free_set (l->next->set);
    free (l->next);
    l->next = NULL;
  }
}

void
list_free_full_all (void) {
  list_free_full_all0 (&list_start);
}

int
list_size (void) {
  list *l;
  int cnt = 0;

  for (l=&list_start; l->next != NULL; l=l->next)
    cnt++;
  return cnt;
}

/* Create an array of elements of the list. */
/* The old list is freed when it is successfully converted. */

/* リストの要素からなる配列を返す。 */
/* 変換後にリストのメモリは開放される。 */
subset **
list_2_array (void) {
  list *l;
  subset **p, **q;
  int n;

  n = list_size ();
  if (n <= 0)
    return NULL;
  p = q = (subset **) malloc (sizeof(subset *)*n);
  for (l=&list_start; --n >= 0 && l->next != NULL; l=l->next)
    *p++ = l->next->set;
  if (n < 0 && l->next != NULL) { /* error */
    free (q);
    return NULL;
  }
  list_free_all ();
  return q;
}

static int
list_array_cmp (subset **set1, subset **set2) {
  return set_cmp(*set1, *set2);
}

static void
array_qsort (subset **sarray, int n) {
  qsort ((void *)sarray, n, sizeof(subset *), (int (*)(const void *, const void *)) list_array_cmp);
}

static void
array_2_list (subset **sarray, int n) {
  int i;

  for (i=0; i<n; ++i)
    list_append (sarray[i]);
  free (sarray);
}

void
list_sort (void) {
  subset **sarray;
  int n;

  n = list_size ();
  sarray = list_2_array ();
  array_qsort (sarray, n);
  array_2_list (sarray, n);  
}

void
list_each (void (* func) (subset *)) {
  list *l;

  for (l=&list_start; l->next != NULL; l=l->next)
    func (l->next->set);
}

/* find intermediate groups between symgr and set. */

/* symgrとsetの中間群を探す */
static void
find_intermediate_group (const subset *symgr, const subset *set, subset *set0) {
  int i, n, *array;
  subset *set1, *set2, *set3;

  if (set_get_size (set) * 2 >= set_get_size (symgr))
    return;
  n = set_get_size (set0);
  array = set_get_array (set0);
  if (n == 0) /* empty set */
    return;
  for (i=0,set3=set0; i<n; ++i) {
    if ((set1 = set_append (set, *(array+i))) == NULL)
      return;
    if ((set2 = set_gen_group (set1)) == NULL)
      return;
    set3 = set_remove (set3, *(array+i));
    if (list_lookup (set2) == set2)
      continue;
    list_append (set2);
    find_intermediate_group (symgr, set2, set_subtract (set3, set2));
  }      
}

/* Find all the subgroups of the n-degree symmetric group. */

/* n次対称群のすべての部分群を求める。 */
void
find_subgroups (const int n) {
  int a[1];
  subset *set1, *symgr, *set2;

  if (n <= 0 || n > MAX_DEGREE)
    return;
  if ((symgr = set_create_symgr (n)) == NULL)
    return;
  list_append (symgr);
  a[0] = 0;
  if ((set1 = set_create_set (n, 1, a)) == NULL)
    return;
  list_append (set1);
  if ((set2 = set_subtract (symgr, set1)) == NULL)
    return;
  find_intermediate_group (symgr, set1, set2);
}

