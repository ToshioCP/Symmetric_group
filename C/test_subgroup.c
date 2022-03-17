/* test subgroup.c */

#include <stdio.h>

#include "subgroup.c"

void
each_print (subset *set) {
  printf ("%s\n",set_to_s (set));
}

void
each_test_1 (subset *set) {
  static int i;
  int sa[3][3] = {
    {1,3,5},
    {1,2,4},
    {2,7,13}
  };
  subset *set1;
  set1 = set_create_set (4,3,sa[i]);
  if (set_cmp (set, set1) != 0)
    printf ("list_each didn't work. (each_test_1 - %d)\n", i);
  ++i;
}

int
set_cmp_p (subset **a, subset **b) {
  return set_cmp (*a, *b);
}

void
test() {
printf ("set_is_subgroup\n");
  int a1[1]={0};
  int a2[2]={0,1};
  int a3[3]={0,3,4};
  int a4[6]={0,1,2,3,4,5};
  int a5[2]={0,3};
  subset *set1, *set2;
  if ((set1 = set_create_set (3,1,a1)) == NULL)
    printf ("set_create_set didn't work. (1)\n");
  if (set_is_subgroup (set1) != 1)
    printf ("set_is_subgroup didn't work. (1)\n");

  if ((set1 = set_create_set (3,2,a2)) == NULL)
    printf ("set_create_set didn't work. (2)\n");
  if (set_is_subgroup (set1) != 1)
    printf ("set_is_subgroup didn't work. (2)\n");

  if ((set1 = set_create_set (3,3,a3)) == NULL)
    printf ("set_create_set didn't work. (3)\n");
  if (set_is_subgroup (set1) != 1)
    printf ("set_is_subgroup didn't work. (3)\n");

  if ((set1 = set_create_set (3,6,a4)) == NULL)
    printf ("set_create_set didn't work. (4)\n");
  if (set_is_subgroup (set1) != 1)
    printf ("set_is_subgroup didn't work. (4)\n");

  if ((set1 = set_create_set (3,2,a5)) == NULL)
    printf ("set_create_set didn't work. (5)\n");
  if (set_is_subgroup (set1) != 0)
    printf ("set_is_subgroup didn't work. (5)\n");
  set_free_set (set1);
  int b1[2]={0,6}; /* id, (1,2) */
  int b2[2]={0,9}; /* id, (1,2,3,4) */
  int b3[8]={0,5,7,9,14,16,18,23}; /* Klein four-group */
  if ((set1 = set_create_set (4,2,b1)) == NULL)
    printf ("set_create_set didn't work. (6)\n");
  if (set_is_subgroup (set1) != 1)
    printf ("set_is_subgroup didn't work. (6)\n");

  if ((set1 = set_create_set (4,2,b2)) == NULL)
    printf ("set_create_set didn't work. (7)\n");
  if (set_is_subgroup (set1) != 0)
    printf ("set_is_subgroup didn't work. (7)\n");

  if ((set1 = set_create_set (4,8,b3)) == NULL)
    printf ("set_create_set didn't work. (8)\n");
  if (set_is_subgroup (set1) != 1)
    printf ("set_is_subgroup didn't work. (8)\n");
printf ("set_create_symgr\n");
  int i, j, f, *a;
  for (i=1; i<MAX_DEGREE; ++i) {
    if ((set1 = set_create_symgr (i)) == NULL)
      printf ("set_create_symgr didn't work. (%d)\n", i);
    f = 1;
    for (j=2; j<=i; ++j)
      f *= j;
    a = (int*) malloc (sizeof(int)*f);
    for (j=0; j<f; ++j)
      *(a+j) = j;
    if ((set2 = set_create_set (i,f,a)) == NULL)
      printf ("set_create_set didn't work. (symgr_set2-%d)\n", i);
    if (set_cmp (set1, set2) != 0)
      printf ("set_create_symgr didn't work. (%d)\n", i);
  }
printf ("set_append\n");
  int c1[3]={4,8,10};
  int c2[4]={4,8,10,20};
  int c3[5]={10,100,500,600,719};
  int c4[6]={10,100,300,500,600,719};
  if ((set1 = set_create_set (5,3,c1)) == NULL)
    printf ("set_create_set didn't work. (set_append 1)\n");
  set2 = set_append (set1, 20);
  if ((set1 = set_create_set (5,4,c2)) == NULL)
    printf ("set_create_set didn't work. (set_append 2)\n");
  if (set_cmp(set1, set2) != 0)
    printf ("set_append didn't work. (1)\n");

  if ((set1 = set_create_set (6,5,c3)) == NULL)
    printf ("set_create_set didn't work. (set_append 3)\n");
  set2 = set_append (set1, 300);
  if ((set1 = set_create_set (6,6,c4)) == NULL)
    printf ("set_create_set didn't work. (set_append 4)\n");
  if (set_cmp(set1, set2) != 0)
    printf ("set_append didn't work. (2)\n");
printf ("set_remove\n");
  if ((set1 = set_create_set (5,3,c1)) == NULL)
    printf ("set_create_set didn't work. (set_remove 1)\n");
  if ((set2 = set_create_set (5,4,c2)) == NULL)
    printf ("set_create_set didn't work. (set_remove 2)\n");
  if (set_cmp(set_remove (set2, 20), set1) != 0)
    printf ("set_remove didn't work. (1)\n");

  if ((set1 = set_create_set (6,5,c3)) == NULL)
    printf ("set_create_set didn't work. (set_remove 3)\n");
  if ((set2 = set_create_set (6,6,c4)) == NULL)
    printf ("set_create_set didn't work. (set_remove 4)\n");
  if (set_cmp(set_remove (set2, 300), set1) != 0)
    printf ("set_remove didn't work. (2)\n");
printf ("set_gen_group\n");
  int d1[1]={1};
  int d2[2]={0,1};
  int d3[2]={9,14}; /* cycles (1,2,3,4),(1,3) */
  int d4[8]={0,5,7,9,14,16,18,23}; /* Klein four-group */
  int d5[2]={6,9}; /* cycles (1,2),(1,2,3,4) */
  if ((set1 = set_create_set (5,1,d1)) == NULL)
    printf ("set_create_set didn't work. (set_gen_group 1)\n");
  if ((set2 = set_gen_group (set1)) == NULL)
    printf ("set_gen_group didn't work. (1)\n");
  if ((set1 = set_create_set (5,2,d2)) == NULL)
    printf ("set_create_set didn't work. (set_gen_group 2)\n");
  if (set_cmp(set1, set2) != 0)
    printf ("set_gen_group didn't work. (2)\n");

  if ((set1 = set_create_set (4,2,d3)) == NULL)
    printf ("set_create_set didn't work. (set_gen_group 3)\n");
  if ((set2 = set_gen_group (set1)) == NULL)
    printf ("set_gen_group didn't work. (3)\n");
  if ((set1 = set_create_set (4,8,d4)) == NULL)
    printf ("set_create_set didn't work. (set_gen_group 4)\n");
  if (set_cmp(set1, set2) != 0)
    printf ("set_gen_group didn't work. (4)\n");

  if ((set1 = set_create_set (4,2,d5)) == NULL)
    printf ("set_create_set didn't work. (set_gen_group 5)\n");
  if ((set2 = set_gen_group (set1)) == NULL)
    printf ("set_gen_group didn't work. (5)\n");
  if ((set1 = set_create_symgr (4)) == NULL)
    printf ("set_create_symgr didn't work. (set_gen_group 1)\n");
  if (set_cmp(set1, set2) != 0)
    printf ("set_gen_group didn't work. (6)\n");
printf ("gen_cyclic_group\n");
  int e1[2]={0,1};
  int e2[3]={0,30,48}; /* cycles id, (1,2,3),(1,3,2) */
  int e3[4]={0,32,60,72}; /* cycles id, (1,2,3,4),(1,3)(2,4), (1,4,3,2) */
  int e4[5]={0,33,64,90,96}; /* cycles id,(1,2,3,4,5),(1,3,5,2,4),(1,4,2,5,3),(1,5,4,3,2) */
  if ((set1 = gen_cyclic_group (5,1)) == NULL)
    printf ("gen_cyclic_group didn't work. 1\n");
  if ((set2 = set_create_set (5,2,e1)) == NULL)
    printf ("set_create_set didn't work. (gen_cyclic_group 1)\n");
  if (set_cmp(set1, set2) != 0)
    printf ("gen_cyclic_group didn't work. 2\n");

  if ((set1 = gen_cyclic_group (5,30)) == NULL)
    printf ("gen_cyclic_group didn't work. 3\n");
  if ((set2 = set_create_set (5,3,e2)) == NULL)
    printf ("set_create_set didn't work. (gen_cyclic_group 2)\n");
  if (set_cmp(set1, set2) != 0)
    printf ("gen_cyclic_group didn't work. 4\n");

  if ((set1 = gen_cyclic_group (5,32)) == NULL)
    printf ("gen_cyclic_group didn't work. 5\n");
  if ((set2 = set_create_set (5,4,e3)) == NULL)
    printf ("set_create_set didn't work. (gen_cyclic_group 3)\n");
  if (set_cmp(set1, set2) != 0)
    printf ("gen_cyclic_group didn't work. 6\n");

  if ((set1 = gen_cyclic_group (5,33)) == NULL)
    printf ("gen_cyclic_group didn't work. 7\n");
  if ((set2 = set_create_set (5,5,e4)) == NULL)
    printf ("set_create_set didn't work. (gen_cyclic_group 4)\n");
  if (set_cmp(set1, set2) != 0)
    printf ("gen_cyclic_group didn't work. 8\n");

/* list */
printf ("l_append & l_lookup in subgroup.c\n");
  int sa[3][3] = {
    {1,3,5},
    {1,2,4},
    {2,7,13}
  };
  subset *ss[3], *p;
  for (i=0; i<3; ++i)
    if ((ss[i] = set_create_set (4,3,sa[i])) == NULL)
      printf ("set_create_set didn't work. (l_append 1-%d)\n", i);
  if (l_append (&start, ss[0]) != ss[0])
    printf ("l_append didn't work. (1)\n");
  else if (l_lookup (&start, ss[0]) != ss[0])
    printf ("l_lookup didn't work. (1)\n");
  if (l_lookup (&start, ss[1]) != NULL)
    printf ("l_lookup didn't work. (2)\n");
  l_free_full_all (&start, (void (*) (void *)) set_free_set);
printf ("l_append_s\n");
  if ((ss[0] = set_create_set (4,3,sa[0])) == NULL)
    printf ("set_create_set didn't work. (l_append_s 1)\n");
  if (l_append_s (&start, ss[0]) != ss[0])
    printf ("l_append didn't work. (1)\n");
  if ((ss[1] = set_create_set (4,3,sa[0])) == NULL)
    printf ("set_create_set didn't work. (l_append_s 2)\n");
  if (l_append_s (&start, ss[0]) != ss[0])
    printf ("l_append didn't work. (3)\n");
  if (l_size (&start) != 1)
    printf ("l_append didn't work. (4)\n");
  if ((ss[3] = set_create_set (4,3,sa[2])) == NULL)
    printf ("set_create_set didn't work. (l_append 3)\n");
  if (l_append (&start, ss[3]) != ss[3])
    printf ("l_append didn't work. (4)\n");
  if (l_size (&start) != 2)
    printf ("l_append didn't work. (5)\n");
  l_free_full_all (&start, (void (*) (void *)) set_free_set);
printf ("l_size\n");
  for (i=0; i<3; ++i)
    if ((ss[i] = set_create_set (4,3,sa[i])) == NULL)
      printf ("set_create_set didn't work. (l_size - %d)\n", i);
  if (l_append (&start, ss[0]) != ss[0])
    printf ("l_append didn't work. (l_size)\n");
  if (l_size (&start) != 1)
    printf ("l_size didn't work. (1)\n");
  if (l_append (&start, ss[1]) != ss[1])
    printf ("l_append didn't work. (2)\n");
  if (l_size (&start) != 2)
    printf ("l_size didn't work. (2)\n");
  if (l_append (&start, ss[2]) != ss[2])
    printf ("l_append didn't work. (3)\n");
  if (l_size (&start) != 3)
    printf ("l_size didn't work. (3)\n");
printf ("l_free_full_all\n");
  l_free_full_all (&start, (void (*) (void *)) set_free_set);
  if (l_size (&start) != 0)
    printf ("l_free_full_all didn't work. (0)\n");
  if (start.next != NULL)
    printf ("l_free_full_all didn't work. (1)\n");
printf ("l_free_all\n");
  for (i=0; i<3; ++i)
    if ((ss[i] = set_create_set (4,3,sa[i])) == NULL)
      printf ("set_create_set didn't work. (l_free_all - %d)\n", i);
  if (l_append (&start, ss[0]) != ss[0])
    printf ("l_append didn't work. (l_free_all 1)\n");
  if (l_append (&start, ss[1]) != ss[1])
    printf ("l_append didn't work. (l_free_all 2)\n");
  if (l_append (&start, ss[2]) != ss[2])
    printf ("l_append didn't work. (l_free_all 3)\n");
  l_free_all (&start);
  if (l_size (&start) != 0)
    printf ("l_size didn't work. (l_free_all 1)\n");
  if (start.next != NULL)
    printf ("l_free_all didn't work. (start not NULL)\n");
  for (i=0; i<3; ++i)
    set_free_set (ss[i]);
printf ("l_l2a\n");
  subset **sarray;
  for (i=0; i<3; ++i) {
    if ((ss[i] = set_create_set (4,3,sa[i])) == NULL)
      printf ("set_create_set didn't work. (l_l2a 1-%d)\n", i);
    if (l_append (&start, ss[i]) != ss[i])
      printf ("l_append didn't work. (l_l2a 1-%d)\n", i);
  }
  if ((sarray = (subset **) l_l2a (&start)) == NULL)
    printf ("l_l2a didn't work. (1)\n");
  for (i=0; i<3; ++i)
    if (set_cmp (*(sarray+i), ss[i]) != 0)
      printf ("l_l2a didn't work. (2)\n");
  if (l_size (&start) != 0)
    printf ("l_size didn't work. (l_l2a 1)\n");
  free (sarray);
printf ("l_sort\n");
  for (i=0; i<3; ++i) {
    if ((ss[i] = set_create_set (4,3,sa[i])) == NULL)
      printf ("set_create_set didn't work. (l_sort - %d)\n", i);
    if (l_append (&start, ss[i]) != ss[i])
      printf ("l_append didn't work. (l_sort - %d)\n", i);
  }
  l_sort (&start, (int (*) (const void *, const void *)) set_cmp_p);
  if ((sarray = (subset **) l_l2a (&start)) == NULL)
    printf ("l_sort didn't work. (l_l2a 1)\n");
  if (set_cmp (*sarray, ss[1]) != 0)
    printf ("l_sort didn't work. (1)\n");
  if (set_cmp (*(sarray+1), ss[0]) != 0)
    printf ("l_sort didn't work. (2)\n");
  if (set_cmp (*(sarray+2), ss[2]) != 0)
    printf ("l_sort didn't work. (3)\n");
  free (sarray);
printf ("l_each\n");
  for (i=0; i<3; ++i) {
    if ((ss[i] = set_create_set (4,3,sa[i])) == NULL)
      printf ("set_create_set didn't work. (l_each - %d)\n", i);
    if (l_append (&start, ss[i]) != ss[i])
      printf ("l_append didn't work. (l_each - %d)\n", i);
  }
  l_each (&start, (void (*) (void *)) each_test_1);
  l_free_full_all (&start, (void (*) (void *)) set_free_set);
printf ("find_subgroups & find_intermediate_group\n");
  int sg1[1] = {0};
  set1 = set_create_set (1,1,sg1);
  find_subgroups (1);
  if (l_size (&start) != 1)
    printf ("find_subgroup didn't work. (degree=1)\n");
  if (l_lookup (&start, set1) != set1)
    printf ("find_subgroup didn't work. (degree=1)\n");
  l_free_full_all (&start, (void (*) (void *)) set_free_set);
  int sg2[2][2] = {
    {0},
    {0,1}
  };
  set1 = set_create_set (2,1,sg2[0]);
  set2 = set_create_set (2,2,sg2[1]);
  find_subgroups (2);
  if (l_size (&start) != 2)
    printf ("find_subgroup didn't work. (degree=2) 1\n");
  if (l_lookup (&start, set1) != set1)
    printf ("find_subgroup didn't work. (degree=2) 2\n");
  if (l_lookup (&start, set2) != set2)
    printf ("find_subgroup didn't work. (degree=2) 3\n");
  l_free_full_all (&start, (void (*) (void *)) set_free_set);
  int sg3[6][6] = {
    {0},
    {0,1},
    {0,2},
    {0,5},
    {0,3,4},
    {0,1,2,3,4,5}
  };
  int sg3_size[6] = {1,2,3,4,5,6};
  find_subgroups (3);
  if (l_size (&start) != 6)
    printf ("find_subgroup didn't work. (degree=3) 1\n");
  for (i=0; i<6; ++i) {
    set1 = set_create_set (3, sg3_size[i], sg3[i]);
    if (l_lookup (&start, set1) != set1)
      printf ("find_subgroup didn't work. (degree=3) %d\n", i+2);
  }
  l_free_full_all (&start, (void (*) (void *)) set_free_set);
  int sg4[30][24] = {
    {0},
    {0,1},
    {0,2},
    {0,5},
    {0,6},
    {0,7},
    {0,14},
    {0,16},
    {0,21},
    {0,23},
    {0,3,4},
    {0,8,12},
    {0,11,19},
    {0,15,20},
    {0,1,6,7},
    {0,2,21,23},
    {0,5,14,16},
    {0,7,16,23},
    {0,7,17,22},
    {0,9,16,18},
    {0,10,13,23},
    {0,1,2,3,4,5},
    {0,1,14,15,20,21},
    {0,2,6,8,12,14},
    {0,5,6,11,19,21},
    {0,1,6,7,16,17,22,23},
    {0,2,7,10,13,16,21,23},
    {0,5,7,9,14,16,18,23},
    {0,3,4,7,8,11,12,15,16,19,20,23},
    {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23}
  };
  int sg4_size[30] = {1,2,2,2,2,2,2,2,2,2,3,3,3,3,4,4,4,4,4,4,4,6,6,6,6,8,8,8,12,24};
  find_subgroups (4);
  if (l_size (&start) != 30)
    printf ("find_subgroup didn't work. (degree=4) 1\n");
  for (i=0; i<30; ++i) {
    set1 = set_create_set (4, sg4_size[i], sg4[i]);
    if (l_lookup (&start, set1) != set1)
      printf ("find_subgroup didn't work. (degree=4) %d\n", i+2);
  }
  l_free_full_all (&start, (void (*) (void *)) set_free_set0);
}

int
main (int argc, char **argv) {
  test();
}

