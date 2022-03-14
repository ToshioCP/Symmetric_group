
#include <stdlib.h>
#include <string.h>

#include "permutation.h"

int * cayley_tables[MAX_DEGREE];

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
static int *
gen_sym_group_1 (const int n) {
  int i, *p, *q;
  int a[MAX_DEGREE];

  for (i=0; i<MAX_DEGREE; ++i)
    a[i] = i + 1;
  p = q = (int *) malloc(sizeof(int)*n*fact(n));
  gen_sym_group_0 (n, 0, a, &p);
  return q;
}

static int*
cayley_table (const int n, int *p) {
  int f, *t;
  int i, j, k, l, found, *pi, *pj, *r;
 
  f = fact(n);
  t = (int *) malloc(sizeof(int)*f*f);
  r = (int *) malloc(sizeof(int)*n);
  for (i=0; i<f; ++i)
    for (j=0; j<f; ++j) {
      pi = p + i*n;
      pj = p + j*n;
      mul_0 (n, pi, pj, r);

      for (k=0; k<f; ++k) {
        found = 1;
        for (l=0; l<n; ++l)
          if (*(p+k*n+l) != *(r+l)) {
            found = 0;
            break;
          }
        if (found == 1) {
          *(t+i*f+j) = k;
          break;
        }
      }
    }
  free (r);
  return t;
}

/* Create Cayley table, which is a table of the multiplication of permutations. */

/* 乗算表の生成 */
int *
create_ctable (const int n) {
  int *symgr;

  if (n < 1 || n > MAX_DEGREE)
    return NULL;
  if (cayley_tables[n-1] != NULL)
    return cayley_tables[n-1];
  symgr = gen_sym_group_1 (n);
  cayley_tables[n-1] = cayley_table (n, symgr);
  free (symgr);
  return cayley_tables[n-1];
}

/* Free Cayley table. Users don't need to call this function. */

/* 乗算表のメモリを解放する。ユーザはこの関数を使う必要はない。 */
void
free_ctable (void) {
  int i;

  for (i=0; i<MAX_DEGREE; ++i)
    if (cayley_tables[i] != NULL) {
      free (cayley_tables[i]);
      cayley_tables[i] = NULL;
    }
}

/* i * j => return, All the integers are the indecies of permutations */

/* iとjの積を返す。これらの整数は置換のインデックス表現である。 */
int
mul_pp (const int n, const int i, const int j) {
  int f, *t;

  if (n < 1 || n > MAX_DEGREE)
    return -1;
  f = fact (n);
  if (i < 0 || i >= f)
    return -1;
  if (j < 0 || j >= f)
    return -1;
  if ((t = create_ctable (n)) == NULL)
    return -1;
  return *(t+i*f+j);
}

