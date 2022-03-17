#include "subset.h"

int
set_is_subgroup (const subset *set);

subset *
set_create_symgr (int degree);

subset *
set_append (const subset *set, const int i);

subset *
set_gen_group (subset *set);

void
find_subgroups (const int n);

int
n_subgroups (void);

void
print_subgroups (void);

