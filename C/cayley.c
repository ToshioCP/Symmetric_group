
#include <stdlib.h>
#include <string.h>

#include "permutation.h"

int * cayley_tables[MAX_DEGREE];

static int*
cayley_table (const int n) {
  int f, *t;
  int i, j, p[MAX_DEGREE], q[MAX_DEGREE], r[MAX_DEGREE];

  f = fact(n);
  t = (int *) malloc(sizeof(int)*f*f);
  for (i=0; i<f; ++i)
    for (j=0; j<f; ++j) {
      i2p (n, i, p);
      i2p (n, j, q);
      mul_0 (n, p, q, r);
      *(t+i*f+j) = p2i (n, r);
    }
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
  cayley_tables[n-1] = cayley_table (n);
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
