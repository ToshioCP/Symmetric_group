#ifndef __SUBSET_H__
#define __SUBSET_H__

typedef struct _subset subset;
struct _subset {
  int degree;
  int n; /* size */
  int *a; /* array of permutations (indeces) */
};

#include "debug.h"
#ifdef debug
#include "list.h"
list *
set_start_address (void);
#endif

int
is_set (const subset *set);

void
set_free_set0 (subset *set);

void
sub_finalize (void);

subset *
set_create_set (const int degree, const int n, const int a[]);

void
set_free_set (subset *set);

int
set_get_degree (const subset *set);

int
set_get_size (const subset *set);

int *
set_get_array (const subset *set);

int
set_include (const subset *set, int i);

int
set_is_subset (subset *set1, subset *set2);

int
set_cmp (const subset *set1, const subset *set2);

subset *
set_union (const subset *set1, const subset *set2);

subset *
set_intersection (const subset *set1, const subset *set2);

subset *
set_subtract (const subset *set1, const subset *set2);

subset *
set_mul_sp (const subset *set, const int j);

subset *
set_mul_ps (const int i, const subset *set);

subset *
set_mul_ss (const subset *set1, const subset *set2);

char *
set_to_s (const subset *set);

char *
set_to_s_c (const subset *set);

#endif /* __SUBSET_H__ */
