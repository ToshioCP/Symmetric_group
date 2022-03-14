gcc test_permutation.c permutation.c
./a.out
gcc test_cayley.c permutation.c
./a.out
gcc test_subset.c cayley.c permutation.c
./a.out
gcc test_subgroup.c subset.c cayley.c permutation.c
./a.out
