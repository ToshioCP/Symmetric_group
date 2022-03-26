/* test permutation.c */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "permutation.h"
#include "sym_gr_1_6.c"
void
test () {
printf ("mapsto\n");
  if (mapsto(3, s3[2], 1) != 2)
    printf ("Mapsto didn't work.\n");
  if (mapsto(5, s5[10], 4) != 2) /*13524*/
    printf ("Mapsto didn't work.\n");
printf ("p_cmp\n");
  if (p_cmp(3, s3[0], s3[0]) != 0)
    printf ("P_cmp didn't work.\n");
  if (p_cmp(5, s5[4], s5[9]) == 0)
    printf ("P_cmp didn't work.\n");
printf ("mul_0\n");
  int pq3[3], pq5[5];
  mul_0 (3, s3[2], s3[3], pq3);
  if (p_cmp(3, pq3, s3[1]) != 0)
    printf ("Mul_0 didn't work. s3[2]*s3[3]!=s3[1]\n");
  mul_0 (5, s5[20], s5[30], pq5);
  if (p_cmp(5, pq5, s5[108]) != 0)
    printf ("Mul_0 didn't work. s5[20]*s5[30]!=s5[108]\n");
printf ("mul\n");
  int *r;
  r = mul (2, s2[0], s2[1]);
  if (p_cmp(2, r, s2[1]) != 0)
    printf ("Mul didn't work. s2[0]*s2[1]!=s2[1]\n");
  free (r);
  r = mul (4, s4[8], s4[12]);
  if (p_cmp(4, r, s4[0]) != 0)
    printf ("Mul didn't work. s4[8]*s4[12]!=s4[0]\n");
  free (r);
printf ("to_s\n");
  char *s;
  if (strcmp((s = to_s(3, s3[3])), "[2,3,1]") != 0)
    printf ("to_s didn't work.\n");
  free (s);
printf ("to_s_c\n");
  if ((s = to_s_c(5, s5[65])) == NULL || strcmp(s, "[1,3,5][2,4]") != 0)
    printf ("to_s_c didn't work. (1)\n");
  free (s);
  int a[] = {3,6,1,5,4,2};
  if ((s = to_s_c(6, a)) == NULL || strcmp(s, "[1,3][2,6][4,5]") != 0)
    printf ("to_s_c didn't work. (2)\n");
  free (s);
printf ("p2i\n");
  int n, i, f;
  for (n=1; n<= MAX_DEGREE; ++n) {
    for (i=f=1; i<=n; ++i)
      f *= i;
    for (i=0; i<f; ++i)
      switch (n) {
      case 1:
        if (p2i (n, s1[i]) != i)
          printf ("p2i didn't work. (n=1)\n");
        break;
      case 2:
        if (p2i (n, s2[i]) != i)
          printf ("p2i didn't work. (n=2)\n");
        break;
      case 3:
        if (p2i (n, s3[i]) != i)
          printf ("p2i didn't work. (n=3)\n");
        break;
      case 4:
        if (p2i (n, s4[i]) != i)
          printf ("p2i didn't work. (n=4)\n");
        break;
      case 5:
        if (p2i (n, s5[i]) != i)
          printf ("p2i didn't work. (n=5)\n");
        break;
      case 6:
        if (p2i (n, s6[i]) != i)
          printf ("p2i didn't work. (n=6)\n");
        break;
      default:
        break;
      }
  }
printf ("i2p\n");
  int j, p[MAX_DEGREE];
  for (n=1; n<= MAX_DEGREE; ++n) {
    for (i=f=1; i<=n; ++i)
      f *= i;
    for (i=0; i<f; ++i) {
      i2p (n, i, p);
      switch (n) {
      case 1:
        for (j=0; j<n; ++j)
          if (p[j] != s1[i][j]) {
            printf ("i2p didn't work. (n=1)\n");
            break;
          }
        break;
      case 2:
        for (j=0; j<n; ++j)
          if (p[j] != s2[i][j]) {
            printf ("i2p didn't work. (n=2)\n");
            break;
          }
        break;
      case 3:
        for (j=0; j<n; ++j)
          if (p[j] != s3[i][j]) {
            printf ("i2p didn't work. (n=3)\n");
            break;
          }
        break;
      case 4:
        for (j=0; j<n; ++j)
          if (p[j] != s4[i][j]) {
            printf ("i2p didn't work. (n=4)\n");
            break;
          }
        break;
      case 5:
        for (j=0; j<n; ++j)
          if (p[j] != s5[i][j]) {
            printf ("i2p didn't work. (n=5)\n");
            break;
          }
        break;
      case 6:
        for (j=0; j<n; ++j)
          if (p[j] != s6[i][j]) {
            printf ("i2p didn't work. (n=6)\n");
            break;
          }
        break;
      default:
        break;
      }
    }
  }
printf ("i_to_s\n");
  if (strcmp((s = i_to_s(3, 3)), "[2,3,1]") != 0)
    printf ("i_to_s didn't work.\n");
  free (s);
printf ("i_to_s_c\n");
  if ((s = i_to_s_c(5, 65)) == NULL || strcmp(s, "[1,3,5][2,4]") != 0)
    printf ("i_to_s_c didn't work. (1)\n");
  free (s);
  if ((s = i_to_s_c(6, 341)) == NULL || strcmp(s, "[1,3][2,6][4,5]") != 0) /* {3,6,1,5,4,2} */
    printf ("i_to_s_c didn't work. (2)\n");
  free (s);
printf ("gen_sym_group\n");
  int *sym;
  for (n=1; n<=MAX_DEGREE; ++n) {
    sym = gen_sym_group (n);
    for (i=0; i<fact(n); ++i)
      for (j=0; j<n; ++j)
        switch (n) {
        case 1:
          if (*(sym+i*n+j) != s1[i][j])
            printf ("gen_sym_group_1 didn't work. (n=1)\n");
          break;
        case 2:
          if (*(sym+i*n+j) != s2[i][j])
            printf ("gen_sym_group_1 didn't work. (n=2)\n");
          break;
        case 3:
          if (*(sym+i*n+j) != s3[i][j])
            printf ("gen_sym_group_1 didn't work. (n=3)\n");
          break;
        case 4:
          if (*(sym+i*n+j) != s4[i][j])
            printf ("gen_sym_group_1 didn't work. (n=4)\n");
          break;
        case 5:
          if (*(sym+i*n+j) != s5[i][j])
            printf ("gen_sym_group_1 didn't work. (n=5)\n");
          break;
        case 6:
          if (*(sym+i*n+j) != s6[i][j])
            printf ("gen_sym_group_1 didn't work. (n=6)\n");
          break;
        default:
          break;
        }
    free (sym);
  }
}

int
main (int argvc, char **argv) {
  test();
}
