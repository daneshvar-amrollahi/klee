#include "klee/klee.h"
#include <stdio.h>

/*
match(char *a, char* b, int n) returns 0 if a and b are equal, and 1 otherwise.
*/

int match(char *a, char* b, int n) 
{
        klee_assert(!klee_is_symbolic(&n));
        uint32_t eqv, i;
        klee_make_symbolic(&i, sizeof(i), "memcmp_return_value");
        klee_make_symbolic(&eqv, sizeof(eqv), "eqv");
        klee_memcmp(a, b, n, eqv, i);
        return i;
}

int main() {
        char a[5], b[5];
        klee_make_symbolic(a, sizeof(a), "a");
        klee_make_symbolic(b, sizeof(b), "b");
        int n = 5, x;
        int res = match(a, b, n);
        if (res == 1)
        {
                /* klee_assume(a[0] == b[0]);
                klee_assume(a[1] == b[1]);
                klee_assume(a[2] == b[2]);
                klee_assume(a[3] == b[3]);
                klee_assume(a[4] == b[4]); // must fail on this */
                x = 2;
                printf("match = 1\n");
        }
        else
        {
                x = 3;
                /* klee_assume(a[0] != b[0]); // must fail on this */
                printf("match != 1\n");
        }
        
}
