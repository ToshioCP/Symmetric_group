/* Permutation */
/* A permutation is a bijection map on (1..n). */
/* In this program, it is an n-sized array and its elements are integers belongs to (1..n). */
/* For example, if the map p is: */
/* p: (1..3) -> (1..3) */
/* p(1)=2 */
/* p(2)=3 */
/* p(3)=1 */
/* Then, the array of the permutation p is p[]={2,3,1}. */

/* 置換 */
/* 置換とは、集合(1..n)上の全単射である。 */
/* このプログラムでは、それは(1..n)の要素による配列で表される。 */
/* 例えば、写像pが */
/* p: (1..3) -> (1..3) */
/* p(1)=2 */
/* p(2)=3 */
/* p(3)=1 */
/* であるとき、それを表す配列は、p[]={2,3,1}である。 */

#include <stdlib.h>
#include <string.h>
#include "permutation.h"

/* factorial */
int
fact (const int n) {
  int fact, i;

  for (fact=i=1; i<=n; ++i)
    fact *= i;
  return fact;
}

/* yes => 1, no => 0*/
int
is_perm (const int size, const int p[]) {
  int i, c[MAX_DEGREE];

  if (size < 1 || size > MAX_DEGREE || p == NULL)
    return 0;
  for (i=0; i<size; ++i)
    c[i] = 0;
  for (i=0; i<size; ++i) {
    if (p[i] < 1 || p[i] > size)
      return 0;
    c[p[i]-1] = 1;
  }
  for (i=0; i<size; ++i)
    if (c[i] != 1)
      return 0;
  return 1;
}

/* error => -1 */
int
mapsto (const int size, const int p[], const int x) {
  if (is_perm(size, p) != 1 || x < 1 || x > MAX_DEGREE)
    return -1;
  return p[x-1];
}

/* p>q => 1 */
/* p=q => 0 */
/* p<q => 11 */
/* error => -2 */
int
p_cmp (const int size, const int p[], const int q[]) {
  int i;

  if (is_perm (size, p) == 0 || is_perm (size, q) == 0)
    return -2;
  for (i=0; i<size; ++i) {
    if (p[i] > q[i])
      return 1;
    else if (p[i] < q[i])
      return -1;
    /* else continue */
  }
  return 0;
}

/* p*q => r */
void
mul_0 (const int size, const int p[], const int q[], int r[]) {
  int i;

  if (is_perm (size, p) == 0 || is_perm (size, q) == 0 || r == NULL)
    return;
  for (i=0; i<size; ++i)
    r[i] = p[q[i]-1];
}

/* p*q => return value */
int *
mul (const int size, const int p[], const int q[]) {
  int i, *r;

  if (is_perm (size, p) == 0 || is_perm (size, q) == 0)
    return NULL;
  r = (int *) malloc(sizeof(int)*size);
  for (i=0; i<size; ++i)
    r[i] = p[q[i]-1];
  return r;
}

/* Return the string of the permutation. */
/* For example, If p[]={1,2,3}, then the returned string is "[1,2,3]". */
/* The string will have to be freed when it becomes useless. */

/* 置換を文字列にして返す。 */
/* 例えば、p[]={1,2,3}のとき、文字列"[1,2,3]"が返される。 */
/* 文字列は不要になったら、そのメモリを解放すること。 */
#include <stdio.h>
char *
to_s (const int size, const int p[]) {
  char *s, *t;
  int i;

  if (is_perm (size, p) == 0)
    return NULL;
  t = s = (char *) malloc(sizeof(char)*(2*size+2));
  *t++ = '[';
  for (i=0; i<size; ++i) {
    *t++ = p[i]+'0';
    *t++ = ',';
  }
  *(t-1) = ']';
  *t = '\0';
  return s;
}

/* Return a string of the permutation. The string is a product of cycles. */
/* For example, If p[]={2,1,5,3,4}, then the returned string is "[1,2][3,5,4]". */
/* The identitty permutation is converted into the empty string. */
/* The string will have to be freed when it becomes useless. */

/* 置換を巡回置換の積の形の文字列にして返す。 */
/* 例えば、p[]={2,1,5,3,4}のとき、文字列"[1,2][3,5,4]"が返される。 */
/* 恒等置換は空文字列になる */
/* 文字列は不要になったら、そのメモリを解放すること。 */
char *
to_s_c (const int size, const int p[]) {
  char *s, *t;
  int i, j, len, len1;
  int *c;

  if (is_perm (size, p) == 0)
    return NULL;

  c = (int *) malloc (sizeof(int)*size);
  for (i=0; i<size; ++i)
    *(c+i) = 0;
  for (len=i=0; i<size; ++i)
    if (*(c+i) == 1)
      ;
    else if (p[i] - 1 == i)
      *(c+i) = 1;
    else {
      for (j=i,len1=0; ;j=p[j]-1) {
        *(c+j) = 1;
        len1++;
        if (p[j]-1 == i)
          break;
      }
      len += 2*len1+1; /* for [], */
    }
  t = s = (char *) malloc(sizeof(char)*(len+1));
  for (i=0; i<size; ++i)
    *(c+i) = 0;
  for (i=0; i<size; ++i)
    if (*(c+i) == 1)
      ;
    else if (p[i]-1 == i)
      *(c+i) = 1;
    else {
      *t++ = '[';
      for (j=i; ;j=p[j]-1) {
        *(c+j) = 1;
        *t++ = j+1 + '0';
        *t++ = ',';
        if (p[j]-1 == i)
          break;
      }
      *(t-1) = ']';
    }
  *t = '\0';
  free (c);
  return s;
}

/* permutation => the index of the permutation */
/* error => -1 */
int
p2i (int size, const int p[]) {
  int i=0, c[MAX_DEGREE], d[MAX_DEGREE];
  int j, k, l;

  if (is_perm (size, p) == 0)
    return -1;
  for (k=0; k<size; ++k)
    c[k] = k;
  d[size-1] = 0;
  for (k=size-2,j=1; k>=0; --k) {
    d[k] = j;
    j *= size - k;
  }
  for (k=0; k<size; ++k) {
    i += c[p[k]-1] * d[k];
    for (j=p[k]; j<size; ++j)
      c[j] -= 1;
  }
  return i;
}

/* index => permutation */
/* The size of the array p[] must be at least "size". */
void
i2p (int size, int i, int p[]) {
  int c[MAX_DEGREE], d[MAX_DEGREE];
  int j, k, l, f, q, r;

  if (size < 1 || size > MAX_DEGREE || p == NULL)
    return;
  f = fact (size);
  if (i < 0 || i >= f)
    return;
  for (k=0; k<size; ++k)
    c[k] = k + 1;
  d[size-1] = 1;
  for (k=size-2,j=1; k>=0; --k) {
    d[k] = j;
    j *= size - k;
  }
  for (k=0,r=i; k<size; ++k) {
    q = r / d[k];
    r = r - q * d[k];
    p[k] = c[q];
    for (j=q; j<size-1; ++j)
      c[j] = c[j+1];
  }
}

/* index => string */
char *
i_to_s (const int size, int i) {
  int p[MAX_DEGREE];

  if (size < 1 || size > MAX_DEGREE || p == NULL)
    return NULL;
  i2p (size, i, p);
  if (is_perm (size, p) != 1)
    return NULL;
  return to_s (size, p);
}

/* index => string which is a product of cycles */
char *
i_to_s_c (const int size, int i) {
  int p[MAX_DEGREE];
  char *s;

  if (size < 1 || size > MAX_DEGREE || p == NULL)
    return NULL;
  i2p (size, i, p);
  if (is_perm (size, p) != 1)
    return NULL;
  if ((s = to_s_c (size, p)) == NULL)
    return NULL;
  if (strcmp (s, "") == 0) {
    free (s);
    s = (char *) malloc (sizeof(char)*3);
    *s = 'i'; *(s+1)='d'; *(s+2)='\0';
  }
  return s;
}

/* a: n-sized array of int. */
/* *p: an array of int. */

/* a: 大きさが n の整数の配列 */
/* *p: 整数の配列 */
static void
gen_sym_group_0 (const int n, int j, int a[], int **p) {
  int i, swap;

  if (j == n-1)
    for (i=0; i<n; ++i)
      *(*p)++ = a[i];
  else {
    for (i=j; i<n; ++i) {
      swap = a[j];
      a[j] = a[i];
      a[i] = swap;
      gen_sym_group_0 (n, j+1, a, p);
    }
    for (i=j; i<n-1; ++i) {
      swap = a[i];
      a[i] = a[i+1];
      a[i+1] = swap;
    }
  }
}

/* Returns an array that contains the whole sorted permutations. */
/* The array will have to be freed by the caller when it becomes useless. */

/* n次の置換全体を昇順に並べた配列を返す。 */
/* 配列は呼び出し側でメモリ解放する。 */
int *
gen_sym_group (const int n) {
  int i, *p, *q;
  int a[MAX_DEGREE];

  for (i=0; i<MAX_DEGREE; ++i)
    a[i] = i + 1;
  p = q = (int *) malloc(sizeof(int)*n*fact(n));
  gen_sym_group_0 (n, 0, a, &p);
  return q;
}
