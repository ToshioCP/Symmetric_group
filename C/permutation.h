#define MAX_DEGREE 6
#define MAX_FACT_DEGREE 720

int
fact (const int n);

/* yes => 1, no => 0*/
int
is_perm (const int size, const int p[]);

int
mapsto (const int size, const int p[], const int x);

/* return value: 1 (p>q), 0 (p=q), -1 (p<q) */
int
p_cmp (const int size, const int p[], const int q[]);

/* r = p*q */
void
mul_0 (const int size, const int p[], const int q[], int r[]);

/* return p*q */
/* It returns a pointer to an array */
/* The array will have to be freed when it is useless. */
int *
mul (const int size, const int p[], const int q[]);

/* Example: p=[1,2,3] => "[1,2,3]" */
/* Returned pointer should be freed when it is useless. */
char *
to_s (const int size, const int p[]);

/* Returned value is a string that represents the products of cycles */
/* Example: p=[4,5,2,1,3] => "[1,4][2,5,3]" */
/* Returned pointer should be freed when it is useless. */
char *
to_s_c (const int size, const int p[]);

int
p2i (int size, const int p[]);

void
i2p (int size, int i, int p[]);

char *
i_to_s (int size, int i);

char *
i_to_s_c (int size, int i);

int *
gen_sym_group (const int n);
