/* Find subgroups of 1 to 5 degree symmetric groups. */

/* 1から5次までの対称群の部分群を求める。 */

#include <stdio.h>
#include <time.h>

#include "subgroup.h"

void
each_print_c (subset *set) {
  printf ("%s\n",set_to_s_c (set));
}

int
main (int argc, char **argv) {
  time_t t1, t2;
  double t;
  int n;

  t1 = time (NULL);
  for (n=1; n<=5; ++n) {
    find_subgroups (n);
    printf ("Subgroups of the %d-degree symmetric group.\n", n);
    printf ("The number of the subgroups is %d.\n", list_size ());
    list_each (each_print_c);
    list_free_full_all ();
  }
  t2 = time (NULL);
  t = difftime (t2, t1);
  printf ("%f\n", t);
}

