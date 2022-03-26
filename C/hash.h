/* list.c */

#include <stdlib.h>
#include "list.h"

#define HASHSIZE 2003

list **
h_init (void);

void *
h_lookup (list **ht, const void *o);

void *
h_lookup_with_cmp (list **ht, const void *o, int (*cmp) (const void *, const void *));

void
h_install (list **ht, void *o);

void
h_reset (list **ht);

void
h_reset_full (list **ht, void (*free_o) (void *));

int
h_size (list **ht);

void
h_each (list **ht, void (*func) (void *));
