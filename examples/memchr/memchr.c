#include "klee/klee.h"
#include <stdio.h>

char* match(char* str, char c, uint32_t n) {
        uint32_t ret, impliesVar;
        klee_make_symbolic(&ret, sizeof(ret), "memchr_return_value");
        klee_make_symbolic(&impliesVar, sizeof(impliesVar), "implies_var");
        klee_make_symbolic(str, n, "str");
        klee_memchr(str, c, n, ret, impliesVar);
        return str + ret;
}

int main() {
        char str[5];
        char c;
        klee_make_symbolic(&c, sizeof(c), "c");
        uint32_t n = 5;
        char* res = match(str, c, n);
        if (res == str + n)
        {
                // klee_assume(str[0] == c); should fail on this, as expected
                printf("match not found\n");
        }
        else
        {
                /* klee_assume(str[0] != c);
                klee_assume(str[1] != c);
                klee_assume(str[2] != c);
                klee_assume(str[3] != c);
                klee_assume(str[4] != c); should fail on this, as expected */
                printf("match found\n");
        }
        
}
