/* Find all the subgroups of the 6-degree symmetric group. */

/* 6次対称群の部分群をすべて求める。 */
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
    find_subgroups (6);
    printf ("Subgroups of the 6-degree symmetric group.\n");
    printf ("The number of the subgroups is %d.\n", n_subgroups ());
    print_subgroups ();
    printf ("\n");
  t2 = time (NULL);
  t = difftime (t2, t1);
  printf ("%f\n", t);
}

