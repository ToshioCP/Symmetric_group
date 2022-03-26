gcc test_permutation.c permutation.c
./a.out
gcc test_cayley.c permutation.c
./a.out
gcc test_list.c subset.c hash.c cayley.c permutation.c
./a.out
gcc test_subset.c hash.c list.c cayley.c permutation.c
./a.out
gcc test_hash.c subset.c list.c cayley.c permutation.c
./a.out
gcc test_subgroup.c subset.c hash.c list.c cayley.c permutation.c
./a.out
