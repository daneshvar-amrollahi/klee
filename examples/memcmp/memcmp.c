#include "klee/klee.h"
#include <stdio.h>

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
                x = 2;
                printf("match = 1\n");
        }
        else
        {
                x = 3;
                printf("match != 1\n");
        }
        
}
