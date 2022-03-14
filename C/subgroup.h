#include "subset.h"

int
set_is_subgroup (const subset *set);

subset *
set_create_symgr (int degree);

subset *
set_append (const subset *set, const int i);

subset *
set_gen_group (subset *set);

subset *
list_lookup (const subset *set);

subset *
list_append (subset *set);

void
list_free_all (void);

void
list_free_full_all (void);

int
list_size (void);

subset **
list_2_array (void);

void
list_sort (void);

void
list_each (void (* func) (subset *));

void
find_subgroups (const int n);

