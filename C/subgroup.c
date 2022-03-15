#include <stdio.h>
#include <stdlib.h>
#include "permutation.h"
#include "cayley.h"
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

  if (degree <= 0 || degree > MAX_DEGREE)
    return NULL;
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

static int
int_array_cmp (int *a, int *b) {
  return *a - *b;
}

subset *
gen_cyclic_group (int degree, int i) {
  int j, k, f, *a;
  subset *set;

  if (degree < 1 || degree > MAX_DEGREE)
    return NULL;
  if (i < 0 || i >= fact (degree))
    return NULL;
  f = fact(degree); /* f can be smaller, but I couldn't find it. */
  a = (int *) malloc (sizeof(int)*f);
  *a = 0;
  for (j=i,k=1; j!=0 && k<f; j=mul_pp(degree, j, i), ++k)
    *(a+k) = j;
  if (j != 0) {
    free (a);
    return NULL;
  }
  qsort ((void *)a, k, sizeof(int), (int (*)(const void *, const void *)) int_array_cmp);
  set = set_create_set (degree, k, a);
  free (a);
  return set;
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

  for (l=&list_start; l->next!=NULL; l=l->next)
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

static subset *
lookup (list *start, const subset *set) {
  list *l;

  if (start == NULL)
    return NULL;
  for (l=start; l->next!=NULL; l=l->next)
    if (l->next->set == set)
      break;
  if (l->next == NULL)
    return NULL;
  else
    return l->next->set;
}

static subset *
append (list *start, subset *set) {
  list *l;
  subset *set1;

  if (start == NULL)
    return NULL;
  if ((set1 = lookup (start, set)) != NULL)
    return set1;
  for (l=start; l->next!=NULL; l=l->next)
    ;
  l->next = (list *) malloc (sizeof(list));
  l->next->next = NULL;
  l->next->set = set;
  return l->next->set;
}

/* error => -1 */
static int
lsize (list *start) {
  int i;
  list *l;

  if (start == NULL)
    return -1;
  for (i=0,l=start; l->next!=NULL; l=l->next)
    ++i;
  return i;
}

static void
free_all (list *l) {
  if (l == NULL || l->next == NULL)
    return;
  else {
    free_all (l->next);
    free (l->next);
    l->next = NULL;
  }
}

/* find intermediate groups between symgr and set. */

/* symgrとsetの中間群を探す */
static void
find_intermediate_group (const int n, list *start) {
  int i, j;
  subset *set1, *set2, *set3;
  list *l1, *l2, cgroups = {NULL, NULL};

  if (lsize (start) == 1)
    return;
  if (start == NULL || start->next == NULL)
    return;
  for (l1=start; l1->next!=NULL; l1=l1->next)
    for (l2=l1->next; l2->next!=NULL; l2=l2->next) {
      if ((set1 = set_union (l1->next->set, l2->next->set)) == NULL) {
        free_all (&cgroups);
        return;
      }
      if ((set2 = set_gen_group(set1)) == NULL) {
        free_all (&cgroups);
        return;
      }
      append (&cgroups, set2);
    }
  for (l1=&cgroups; l1->next!=NULL; l1=l1->next)
    list_append (l1->next->set);
  find_intermediate_group (n, &cgroups);
  free_all (&cgroups);
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
  list_append (symgr);
  a[0] = 0;
  if ((set1 = set_create_set (n, 1, a)) == NULL)
    return;
  list_append (set1);
  f = fact (n);
  for (i=1; i<f; ++i) {
    if ((set1 = gen_cyclic_group (n, i)) == NULL) {
      free_all (&cgroups);
      return;
    }
    append (&cgroups, set1);
  }
  for (l=&cgroups; l->next!=NULL; l=l->next)
    list_append (l->next->set);
  find_intermediate_group (n, &cgroups);
  free_all (&cgroups);
}

