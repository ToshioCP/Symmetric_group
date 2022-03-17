gcc test_permutation.c permutation.c
./a.out
gcc test_cayley.c permutation.c
./a.out
gcc test_list.c subset.c cayley.c permutation.c
./a.out
gcc test_subset.c list.c cayley.c permutation.c
./a.out
gcc test_subgroup.c subset.c list.c cayley.c permutation.c
./a.out
