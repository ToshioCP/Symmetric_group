
#include <stdlib.h>
#include <string.h>

#include "permutation.h"
#include "cayley.h"
#include "hash.h"
#include "subset.h"

#include "compile_option.h"

extern int no_check;

static list **ht = NULL;

int
hash (const void *set) {
  const subset *set1 = (const subset *) set;
  int i;
  long h;

  h = set1->degree * 31 + set1->n;
  for (i=0; i<set1->n; ++i)
    h = (h * 31 + set1->a[i]) % HASHSIZE;
  return (int) h;
}

static int
is_set0 (const int degree, const int n, const int set[]) {
  int i;
  int f;

  if (no_check) return 1;

  if (degree <= 0 || degree > MAX_DEGREE)
    return 0;
  f = fact(degree);
  if (n < 0 || n > f)
    return 0;
  if (n == 0) /* null set */
    return 1;
  if (set == NULL)
    return 0;
  if (set[0] < 0 || set[n-1] >= f)
    return 0;
  for (i=0; i<n-1; ++i)
    if (set[i] >= set[i+1])
      return 0;
  return 1;
}

/* yes => 1, no => 0 */
int
is_set (const subset *set) {
  if (set == NULL)
    return 0;
  return is_set0 (set->degree, set->n, set->a);
}

void
set_free_set0 (subset *set) {
  if (is_set (set) == 0)
    return;
  if (set->a != NULL)
    free (set->a);
  free (set);
}

void
set_finalize (void) {
  int i;

  if (ht == NULL)
    return;
  h_reset_full (ht, (void (*) (void *)) set_free_set0);
}

/* degree: the degree of the symmetric group contains the subsets */
/* n: size if the array a[] */
/* a[]: the array contains elements of the subgroup */

/* degree: 部分集合の元になる対称群の次数 */
/* n: 配列a[]の大きさ */
/* a[]: 部分集合の要素の配列 */
subset *
set_create_set (const int degree, const int n, const int a[]) {
  subset *set, *set1;
  int i;

  if (is_set0 (degree, n, a) == 0)
    return NULL;
  if (ht == NULL)
    ht = h_init ();
  set = (subset *) malloc(sizeof(subset));
  set->degree = degree;
  set->n = n;
  if (n == 0)
    set->a = NULL;
  else {
    set->a = (int *) malloc (sizeof(int)*n);
    for (i=0; i<n; ++i)
      *(set->a + i) = a[i];
  }
  if ((set1 = h_lookup_with_cmp (ht, set, (int (*) (const void *, const void *)) set_cmp)) != NULL) {
    set_free_set0 (set);
    return set1;
  } else {
    h_install (ht, set);
    return set;
  }
}

int
set_get_degree (const subset *set) {
  if (is_set (set) == 0)
    return -1;
  return set->degree;
}

int
set_get_size (const subset *set) {
  if (is_set (set) == 0)
    return -1;
  return set->n;
}

int *
set_get_array (const subset *set) {
  if (is_set (set) == 0)
    return NULL;
  return set->a;
}

/* yes => 1, no => 0, error => -1 */
int
set_include (const subset *set, int i) {
  int k;

  if (is_set (set) == 0)
    return -1;
  if (set->n == 0) /* empty */
    return 0;
  for (k=0; k<set->n; ++k)
    if (set->a[k] == i)
      return 1;
  return 0;
}

/* Is set1 a subset of set2? */
/* yes => 1, no => 0, error => -1 */
int
set_is_subset (subset *set1, subset *set2) {
  int k;

  if (is_set (set1) == 0 || is_set (set2) == 0)
    return -1;
  if (set1->degree != set2->degree)
    return -1;
  for (k=0; k<set1->n; ++k)
    if (set_include (set2, set1->a[k]) == 0)
      return 0;
  return 1;
}

/* set1 >  set2 => 1 */
/* set1 == set2 => 0 */
/* set1 <  set2 => -1 */
/* error        => -2 */
int
set_cmp (const subset *set1, const subset *set2) {
  int i, n;

  if (is_set (set1) == 0 || is_set (set2) == 0)
    return -2;
  if (set1->degree != set2->degree)
    return -2;
  n = set1->n <= set2->n ? set1->n : set2->n;
  for (i=0; i<n; ++i)
    if (set1->a[i] != set2->a[i])
      return (set1->a[i] - set2->a[i] >0) ? 1 : -1;
  if (set1->n > n)
    return 1;
  if (set2->n > n)
    return -1;
  return 0;
}

/* error => return -1 */
static int
set_n_intersection (const subset *set1, const subset *set2) {
  int i, j, cnt;

  if (is_set (set1) == 0 || is_set (set2) == 0)
    return -1;
  if (set1->n == 0 || set2->n == 0) /* empty set */
    return 0;
  for (cnt=i=j=0; i < set1->n && j < set2->n; ) {
    if (*(set1->a+i) == *(set2->a+j)) {
      ++cnt;
      ++i;
      ++j;
    } else if (*(set1->a+i) > *(set2->a+j))
      ++j;
    else
      ++i;
  }
  return cnt;
}

/* The union of set1 and set2 => return */
subset *
set_union (const subset *set1, const subset *set2) {
  int degree, i, j, n, *p, *q, k;
  subset *set;

  if (is_set (set1) == 0 || is_set (set2) == 0)
    return NULL;
  if (set1->degree != set2->degree)
    return NULL;
  degree = set1->degree;
  n = set1->n + set2->n - set_n_intersection (set1, set2);
  p = q = (int *) malloc (sizeof(int)*n);
  for (i=j=0; i<set1->n || j<set2->n; )
    if (i<set1->n && j>=set2->n) {
      *p++ = *(set1->a+i);
      ++i;
    } else if (i>=set1->n && j<set2->n) {
      *p++ = *(set2->a+j);
      ++j;
    } else if (*(set1->a+i) == *(set2->a+j)) {
      *p++ = *(set1->a+i);
      ++i;
      ++j;
    } else if (*(set1->a+i) < *(set2->a+j)) {
      *p++ = *(set1->a+i);
      ++i;
    } else {
      *p++ = *(set2->a+j);
      ++j;
    }
  set = set_create_set (degree, n, q);
  free (q);
  return set;
}

/* The intersection of set1 and set2 => return */
subset *
set_intersection (const subset *set1, const subset *set2) {
  int degree, i, j, n, *p, *q;
  subset *set;

  if (is_set (set1) == 0 || is_set (set2) == 0)
    return NULL;
  if (set1->degree != set2->degree)
    return NULL;
  degree = set1->degree;
  n = set_n_intersection (set1, set2);
  p = q = (int *) malloc (sizeof(int)*(n));
  for (i=j=0; i < set1->n && j < set2->n; ) {
    if (*(set1->a+i) == *(set2->a+j)) {
      *p++ = *(set1->a+i);
      ++i;
      ++j;
    } else if (*(set1->a+i) > *(set2->a+j))
      ++j;
    else
      ++i;
  }
  set = set_create_set (degree, n, q);
  free (q);
  return set;
}

/* The difference of set1 and set2 => return */
subset *
set_subtract (const subset *set1, const subset *set2) {
  int i, j, l, m, n, *p, *q;
  subset *set;

  if ((n = set_n_intersection (set1, set2)) == -1)
    return NULL;
  l = set1->n;
  m = set2->n;
  n = l - n;
  p = q = (int *) malloc (sizeof(int)*n);
  if (m == 0) /* set2 is empty => set2->a can be NULL */
    for (i=0; i<l; ++i)
      *(p+i) = *(set1->a+i);
  else
    for (i=j=0; i<l; ) {
      if (j>=m) {
        *p++ = *(set1->a+i);
        ++i;
      } else if (*(set1->a+i) == *(set2->a+j)) {
        ++i;
        ++j;
      } else if (*(set1->a+i) < *(set2->a+j)) {
        *p++ = *(set1->a+i);
        ++i;
      } else
        ++j;
    }
  set = set_create_set (set1->degree, n, q);
  free (q);
  return set;
}

static int
num_cmp (int *a, int *b) {
  return *a-*b;
}

static void
q_sort (int a[], const int l, const int h) {
  qsort (a+l, h-l+1, sizeof(int), (int (*) (const void *, const void *)) num_cmp);
}

/* return the size of the subset*/
static int
uniq (int a[], const int n) {
  int i, j;

  if (n == 1)
    return 1;
  for (i=0,j=1; j<n; ++j)
    if (a[i] != a[j])
      a[++i] = a[j];
  return i + 1;
}

/* set * j (permutation) => return */
subset *
set_mul_sp (const subset *set, const int j) {
  int degree, i, f, *p, *q, *t;
  subset *set1;

  if (is_set (set) == 0)
    return NULL;
  degree = set->degree;
  f = fact(degree);
  if (j < 0 || j >= f)
    return NULL;
  if ((t = create_ctable (degree)) == NULL)
    return NULL;
  p = q = (int *) malloc (sizeof(int)*(set->n));
  for (i=0; i<set->n; ++i)
    *p++ = *(t+*(set->a+i)*f+j);
  q_sort (q, 0, set->n-1);
  set1 = set_create_set (degree, set->n, q);
  free (q);
  return set1;
}

/* i (permutation) * set => return */
subset *
set_mul_ps (const int i, const subset *set) {
  int degree, j, f, *p, *q, *t;
  subset *set1;

  if (is_set (set) == 0)
    return NULL;
  degree = set->degree;
  f = fact(degree);
  if (i < 0 || i >= f)
    return NULL;
  if ((t = create_ctable (degree)) == NULL)
    return NULL;
  p = q = (int *) malloc (sizeof(int)*(set->n));
  for (j=0; j<set->n; ++j)
    *p++ = *(t+i*f+*(set->a+j));
  q_sort (q, 0, set->n-1);
  set1 = set_create_set (degree, set->n, q);
  free (q);
  return set1;
}

/* set1 * set2 => return */
subset *
set_mul_ss (const subset *set1, const subset *set2) {
  int degree, i, j, n1, n2, f, n, *p, *q, *t;
  subset *set3;

  if (is_set (set1) == 0 || is_set (set2) == 0)
    return NULL;
  if (set1->degree != set2->degree)
    return NULL;
  degree = set1->degree;
  f = fact(degree);
  n1 = set1->n;
  n2 = set2->n;
  if ((t = create_ctable (degree)) == NULL)
    return NULL;
  p = q = (int *) malloc (sizeof(int)*n1*n2);
; for (i=0; i<n1; ++i)
    for (j=0; j<n2; ++j)
      *p++ = *(t+set1->a[i]*f+set2->a[j]);
  q_sort (q, 0, n1*n2-1);
  n = uniq (q, n1*n2);
  set3 = set_create_set (degree, n, q);
  free (q);
  return set3;
}

/* Returned value will have to be freed when it becomes useless. */
static char *
concat (const char *s1, const char *s2) {
  size_t len;
  char *s;

  len = strlen (s1) + strlen (s2) + 1;
  s = (char *) malloc (sizeof(char)*len);
  s = strcpy (s, s1);
  s = strcat (s, s2);
  return s;
}

/* The returned string will have to be freed when it becomes useless. */
char *
set_to_s (const subset *set) {
  int i;
  char *s, *t, *u;

  if (is_set (set) == 0)
    return NULL;
  if (set->n == 0) { /* empty set */
    s = (char *) malloc (sizeof(char));
    *s = '\0'; /* empty string */
  } else {
    t = "{";
    if ((u = i_to_s (set->degree, *(set->a))) == NULL)
      return NULL;
    s = concat (t, u);
    free (u);
    for (i=1; i<set->n; ++i) {
      t = concat (s, ",");
      free (s);
      if ((u = i_to_s (set->degree, *(set->a+i))) == NULL) {
        free (t);
        return NULL;
      }
      s = concat (t, u);
      free (t);
      free (u);
    }
    t = s;
    s = concat (t, "}");
    free (t);
  }
  return s;
}

/* The returned string will have to be freed when it becomes useless. */
char *
set_to_s_c (const subset *set) {
  int i;
  char *s, *t, *u;

  if (is_set (set) == 0)
    return NULL;
  if (set->n == 0) { /* empty set */
    s = (char *) malloc (sizeof(char));
    *s = '\0'; /* empty string */
  } else {
    t = "{";
    if ((u = i_to_s_c (set->degree, *(set->a))) == NULL)
      return NULL;
    else {
      s = concat (t, u);
      free (u);
    }
    for (i=1; i<set->n; ++i) {
      t = concat (s, ",");
      free (s);
      if ((u = i_to_s_c (set->degree, *(set->a+i))) == NULL) {
        free (t);
        return NULL;
      } else {
        s = concat (t, u);
        free (t);
        free (u);
      }
    }
    t = s;
    s = concat (t, "}");
    free (t);
  }
  return s;
}
