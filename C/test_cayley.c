/* test  cayley.c */
#include <stdio.h>

#include "permutation.h"
#include "cayley.c"
#include "sym_gr_1_6.c"

void
test () {

printf ("gen_sym_group1\n");
  int *p, i, j, n;
  for (n=1; n<=MAX_DEGREE; ++n) {
    p = gen_sym_group_1 (n);
    for (i=0; i<fact(n); ++i)
      for (j=0; j<n; ++j)
        switch (n) {
        case 1:
          if (*(p+i*n+j) != s1[i][j])
            printf ("gen_sym_group_1 didn't work. (n=1)\n");
          break;
        case 2:
          if (*(p+i*n+j) != s2[i][j])
            printf ("gen_sym_group_1 didn't work. (n=2)\n");
          break;
        case 3:
          if (*(p+i*n+j) != s3[i][j])
            printf ("gen_sym_group_1 didn't work. (n=3)\n");
          break;
        case 4:
          if (*(p+i*n+j) != s4[i][j])
            printf ("gen_sym_group_1 didn't work. (n=4)\n");
          break;
        case 5:
          if (*(p+i*n+j) != s5[i][j])
            printf ("gen_sym_group_1 didn't work. (n=5)\n");
          break;
        case 6:
          if (*(p+i*n+j) != s6[i][j])
            printf ("gen_sym_group_1 didn't work. (n=6)\n");
          break;
        default:
          break;
        }
    free (p);
  }
printf ("cayley_table & create_ctable\n");
  int *t;
  for (n=1; n<=MAX_DEGREE; ++n) {
    if ((t = create_ctable (n)) == NULL)
      printf ("create_ctable didn't work. 1 - %d\n", n);
    for (i=0; i<fact(n); ++i)
      for (j=0; j<fact(n); ++j)
        switch (n) {
        case 1:
          if (*(t+i*fact(n)+j) != c1[i][j])
            printf ("cayley_table didn't work. (n=1)\n");
          break;
        case 2:
          if (*(t+i*fact(n)+j) != c2[i][j])
            printf ("cayley_table didn't work. (n=2)\n");
          break;
        case 3:
          if (*(t+i*fact(n)+j) != c3[i][j])
            printf ("cayley_table didn't work. (n=3)\n");
          break;
        case 4:
          if (*(t+i*fact(n)+j) != c4[i][j])
            printf ("cayley_table didn't work. (n=4)\n");
          break;
        case 5:
          if (*(t+i*fact(n)+j) != c5[i][j])
            printf ("cayley_table didn't work. (n=5)\n");
          break;
        case 6:
          if (*(t+i*fact(n)+j) != c6[i][j])
            printf ("cayley_table didn't work. (n=6)\n");
          break;
        default:
          break;
        }
    if (create_ctable (n) != t)
      printf ("create_ctable didn't work. 2 - %d\n", n);
  }
printf ("free_ctable\n");
  free_ctable ();
  for (n=1; n<=MAX_DEGREE; ++n)
    if (cayley_tables[n] != NULL)
      printf ("free_ctable didn't work.\n");
printf ("mul_pp\n");
  if (mul_pp (5, 3, 2) != 5)
    printf ("mul_pp didn't work. 1\n");
  if (mul_pp (5, 33, 25) != 56)
    printf ("mul_pp didn't work. 2\n");

}

int
main (int argvc, char **argv) {
  test();
}

