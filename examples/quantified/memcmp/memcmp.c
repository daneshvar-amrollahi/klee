#include "klee/klee.h"
#include <stdio.h>

/*
match(char *a, char* b, int n) returns 0 if a and b are equal, and 1 otherwise.
*/

int match(char *a, char* b, int n) 
{
        klee_assert(!klee_is_symbolic(&n));
        uint32_t fqv, i;
        klee_make_symbolic(&i, sizeof(i), "memcmp_return_value");
        // klee_make_symbolic(&fqv, sizeof(fqv), "fqv");
        klee_memcmp(a, b, n, NULL, i);
        klee_assume(a[0] != b[0]); //should fail on this
        return i;
}

int main() {
        char a[5], b[5];
        klee_make_symbolic(a, sizeof(a), "a");
        klee_make_symbolic(b, sizeof(b), "b");
        int n = 5, x;
        match(a, b, n);
}
