/* test subset.c */

#include <stdio.h>
#include <string.h>

#include "subset.c"

void
p_set (subset *set) {
  int i;

  for (i=0; i<set->n; ++i)
    printf ("%d ", *(set->a+i));
  printf ("\n");
}

void
test () {
printf ("is_set0\n");
  int a[6][7] = {
    {0},
    {0,1},
    {1,3,5},
    {4,5,9,12,23},
    {0,14,18,32,65,119},
    {0,40,98,123,245,566,719}
  };
  int b[6][7] = {
    {1},
    {1,0},
    {-1,3,5},
    {3,6,8,13,24},
    {0,24,18,102,65,119},
    {719,566,245,123,98,40,0}
  };
  int size[6] = {1,2,3,5,6,7};
  int i, j;
  for (i=0; i<6;++i)
    if (is_set0 (i+1, size[i], a[i]) != 1)
      printf ("is_set0 didn't work(1), %d.\n", i);
  for (i=0; i<6;++i)
    if (is_set0 (i, size[i], b[i]) != 0)
      printf ("is_set0 didn't work(2).\n");
printf ("set_create_set\n");
  subset *set, *set0;
  set = set_create_set (5, 5, a[4]);
  if (set == NULL)
    printf ("set_create_set didn't work(1).\n");
  if (set->degree != 5)
    printf ("set_create_set didn't work(2).\n");
  if (set->n != 5)
    printf ("set_create_set didn't work(3).\n");
  for (i=0; i<5; ++i)
    if ((set->a)[i] != a[4][i])
    printf ("set_create_set didn't work(4).\n");
  set0 = set_create_set (5, 5, a[4]);
  if (set != set0)
    printf ("set_create_set didn't work(5).\n");
printf ("set_finalize\n");
  for (i=0; i<6; ++i) {
    if ((set = set_create_set (6, size[i], a[i])) == NULL)
      printf ("set_create_set didn't work. (set_finalize %d)\n", i);
    set_finalize ();
    set = (subset *) malloc (sizeof(subset));
    set->degree = 6;
    set->n = size[i];
    set->a = a[i];
    if (h_lookup_with_cmp (ht, set, (int (*) (const void *, const void *)) set_cmp) != NULL)
      printf ("set_finalize didn't work. %d\n", i);
    free (set);
  }
printf ("is_set\n");
  set = set_create_set (3, 3, a[2]);
  if (is_set (set) != 1)
    printf ("is_set didn't work(1).\n");
  set->a[2] = 0;
  if (is_set (set) != 0)
    printf ("is_set didn't work(2).\n");
  set->a[2] = 5;
printf ("set_include\n");
  set = set_create_set (3,3,a[2]);
  for (i=0; i<6; ++i)
    if ((i == 1 || i == 3 || i == 5) && set_include (set, i) != 1)
      printf ("set_include didn't work (1).\n");
    else if ((i == 0 || i == 2 || i == 4) && set_include (set, i) != 0)
      printf ("set_include didn't work (2).\n");
printf ("set_is_subset\n");
  int a01[1] = {1};
  int a02[3] = {1,3,5};
  int a03[3] = {1,3,4};
  int a04[6] = {0,1,2,3,4,5};
  subset *set1, *set2, *set3, *set4;
  set0 = set_create_set (3, 0, NULL); /* empty */
  set1 = set_create_set (3, 1, a01);
  set2 = set_create_set (3, 3, a02);
  set3 = set_create_set (3, 3, a03);
  set4 = set_create_set (3, 6, a04);
  if (set_is_subset (set0, set1) != 1)
    printf ("set_is_subset didn't work 1.\n");
  if (set_is_subset (set1, set2) != 1)
    printf ("set_is_subset didn't work 2.\n");
  if (set_is_subset (set2, set3) != 0)
    printf ("set_is_subset didn't work 3.\n");
  if (set_is_subset (set3, set4) != 1)
    printf ("set_is_subset didn't work 4.\n");
printf ("set_cmp\n");
  int a1[4] = {0, 1, 3, 5};
  int a2[4] = {0, 1, 2, 5};
  int a3[5] = {3, 4, 6, 12, 20};
  int a4[5] = {1, 4, 5, 7, 23};
  set1 = set_create_set (4, 4, a1);
  set2 = set_create_set (4, 4, a2);
  set3 = set_create_set (5, 5, a3);
  set4 = set_create_set (5, 5, a4);
  if (set_cmp (set1, set1) != 0)
    printf ("set_cmp didn't work. (11)\n");
  if (set_cmp (set1, set2) != 1)
    printf ("set_cmp didn't work. (12)\n");
  if (set_cmp (set3, set4) != 1)
    printf ("set_cmp didn't work. (34)\n");
  if (set_cmp (set4, set4) != 0)
    printf ("set_cmp didn't work. (44)\n");
printf ("set_n_intersection\n");
  if (set_n_intersection (set1, set2) != 3)
    printf ("set_n_intersection didn't work. (1)\n");
  if (set_n_intersection (set3, set4) != 1)
    printf ("set_n_intersection didn't work. (2)\n");
  (set4->a)[1] = 2;
  if (set_n_intersection (set3, set4) != 0)
    printf ("set_n_intersection didn't work. (3)\n");
  (set4->a)[1] = 4;
printf ("set_union\n");
  int a5[5] = {0,1,2,3,5};
  int a6[9] = {1,3,4,5,6,7,12,20,23};
  subset *set5, *set6;
  set5 = set_create_set (4,5,a5);
  set6 = set_union (set1, set2);
  if (set_cmp (set5, set6) != 0)
    printf ("set_union didn't work. (1)\n");
  set5 = set_create_set (5,9,a6);
  set6 = set_union (set3, set4);
  if (set_cmp (set5, set6) != 0)
    printf ("set_union didn't work. (2)\n");
printf ("set_intersection\n");
  int a7[3] = {0,1,5};
  int a8[1] = {4};
  set5 = set_create_set (4,3,a7);
  set6 = set_intersection (set1, set2);
  if (set_cmp (set5, set6) != 0)
    printf ("set_intersection didn't work. (1)\n");
  set5 = set_create_set (5,1,a8);
  set6 = set_intersection (set3, set4);
  if (set_cmp (set5, set6) != 0)
    printf ("set_intersection didn't work. (2)\n");
printf ("set_subtract\n");
  int b1[5] = {0,1,3,4,6};
  int b2[3] = {0,3,4};
  int b3[1] = {2};
  int b4[5] = {1,2,5,6,7};
  int c1[2] = {1,6};
  int c2[3] = {0,3,4};
  set1 = set_create_set (4,5,b1);
  set2 = set_create_set (4,3,b2);
  set3 = set_create_set (4,2,c1);
  set4 = set_subtract (set1, set2);
  if (set_cmp (set3, set4) != 0)
    printf ("set_subtract didn't work. (1)\n");
  set2 = set_create_set (4,1,b3);
  set4 = set_subtract (set1, set2);
  if (set_cmp (set1, set4) != 0)
    printf ("set_subtract didn't work. (2)\n");
  set2 = set_create_set (4,5,b4);
  set3 = set_create_set (4,3,c2);
  set4 = set_subtract (set1, set2);
  if (set_cmp (set3, set4) != 0)
    printf ("set_subtract didn't work. (3)\n");
printf ("q_sort\n");
  int sort0[] = {07, 11, 51, 83, 35, 12, 35, 37, 59, 13};
  int sort1[] = {07, 11, 12, 13, 35, 35, 37, 51, 59, 83};
  q_sort (sort0, 0, 9);
  for (i=0; i<10; ++i)
    if (sort0[i] != sort1[i]) {
      printf ("qsort didn't work(1).\n");
      break;
    }
  int sort2[] = {12, 2, 0, 1, 4, 3, 5, 4, 1, 4, 1, 0, 2};
  int sort3[] = {12, 0, 0, 1, 1, 1, 2, 2, 3, 4, 4, 4, 5};
  q_sort (sort2, 1, 12);
  for (i=0; i<=12; ++i)
    if (sort2[i] != sort3[i]) {
      printf ("qsort didn't work(2).\n");
      break;
    }
printf ("uniq\n");
  int a9[] = {2, 2, 3, 3, 4, 4};
  int a10[] = {2, 3, 4, -1, -1, -1};
  i = uniq (a9, 6);
  if (i != 3)
    printf ("uniq didn't work. (returned value)\n");
  for (i=0; i<3; ++i)
    if (a9[i] != a10[i])
      printf ("uniq didn't work.\n");
printf ("set_mul_sp\n");
  int a11[4] = {0,1,3,5};
  int a12[4] = {0,1,2,4};
  set1 = set_create_set (3, 4, a11);
  set2 = set_create_set (3, 4, a12);
  set3 = set_mul_sp (set1, 1); /* {1,0,2,4}=>sort=>{0,1,2,4} */
  if (set_cmp (set2, set3) != 0)
    printf ("set_mul_sp didn't work. (n=3)\n");
  int a13[4] = {5,12,20,23};
  int a14[4] = {0,7,15,22};
  set1 = set_create_set (4, 4, a13);
  set2 = set_create_set (4, 4, a14);
  set3 = set_mul_sp (set1, 8); /* {22,0,7,15}=>sort=>{0,7,15,22} */
  if (set_cmp (set2, set3) != 0)
    printf ("set_mul_sp didn't work. (n=4)\n");
printf ("set_mul_ps\n");
  int a15[4] = {0,1,3,5};
  set1 = set_create_set (3, 4, a11);
  set2 = set_create_set (3, 4, a15);
  set3 = set_mul_ps (1, set1); /* {1,0,5,3}=>sort=>{0,1,3,5} */
  if (set_cmp (set2, set3) != 0)
    printf ("set_mul_ps didn't work. (n=3)\n");
  int a16[4] = {0,10,19,23};
  set1 = set_create_set (4, 4, a13);
  set2 = set_create_set (4, 4, a16);
  set3 = set_mul_ps (8, set1); /* {10,0,23,13}=>sort=>{0,10,13,23} */
  if (set_cmp (set2, set3) != 0)
    printf ("set_mul_ps didn't work. (n=4)\n");
printf ("set_mul_ss\n");
  int a17[3] = {0,2,3};
  int a18[2] = {1,4};
  int a19[6] = {0,1,2,3,4,5};
  set1 = set_create_set (3, 3, a17);
  set2 = set_create_set (3, 2, a18);
  set3 = set_create_set (3, 6, a19);
  set4 = set_mul_ss (set1, set2); /* {1,3,3,1,2,4}=>sort & uniq =>{1,2,3,4} */
  if (set_cmp (set3, set4) != 0)
    printf ("set_mul_ss didn't work. (n=3)\n");
  int a20[6] = {2,46,243,253,274,691};
  int a21[6] = {5,74,88,216,599,704};
  int a22[36] = {
    3, 4, 14, 44, 50, 54, 56, 64, 94, 153,
    185, 201, 217, 241, 244, 255, 262, 266, 272, 293,
    337, 340, 344, 358, 394, 406, 456, 461, 479, 482,
    648, 661, 680, 682, 692, 693
  };
  set1 = set_create_set (6, 6, a20);
  set2 = set_create_set (6, 6, a21);
  set3 = set_create_set (6, 36, a22);
  set4 = set_mul_ss (set1, set2);
  if (set_cmp (set3, set4) != 0)
    printf ("set_mul_ss didn't work. (n=6)\n");
printf ("concat\n");
  char *str1 = "I love";
  char *str2 = " Linux.";
  char *str3 = "I love Linux.";
  char *str4;
  str4 = concat (str1, str2);
  if (strcmp (str3, str4) != 0)
    printf ("concat didn't work. (n=6)\n");
  free (str4);
printf ("set_to_s\n");
  int a23[2] = {1,2}; /* [1,3,2], [2,1,3] */
  char *s;
  set = set_create_set (3,2,a23);
  s = set_to_s (set);
  if (s == NULL || strcmp(s, "{[1,3,2],[2,1,3]}") != 0)
    printf ("set_to_s didn't work. (n=3)\n");
  if (s != NULL)
    free (s);
printf ("set_to_s_c\n");
  s = set_to_s_c (set);
  if (s == NULL || strcmp (s, "{[2,3],[1,2]}") != 0)
    printf ("set_to_s_c didn't work. (n=3)\n");
  if (s != NULL)
    free (s);
  int a24[4] = {0,10,50,100};
  set = set_create_set (5, 4, a24);
  s = set_to_s_c (set);
  if (s == NULL || strcmp (s, "{id,[2,3,5,4],[1,3,4,2],[1,5,3,4,2]}") != 0)
    printf ("set_to_s_c didn't work. (n=5)\n");
  if (s != NULL)
    free (s);
  set_finalize ();
}

int
main (int argvc, char **argv) {
  test();
  return 0;
}
