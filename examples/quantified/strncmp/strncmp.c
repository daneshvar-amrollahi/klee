#include "klee/klee.h"
#include <stdio.h>


int match(char *a, char* b, int n) 
{
        uint32_t ret;
        klee_make_symbolic(&ret, sizeof(ret), "strncmp_return_value");
        klee_strncmp(a, b, n, ret);
        return ret;
}

int main() {
        char a[5], b[5];
        klee_make_symbolic(a, sizeof(a), "a");
        klee_make_symbolic(b, sizeof(b), "b");
        int n = 5, res = match(a, b, n);

        // klee_assume(res == 0);
        // klee_assume(a[2] != b[2]);

        // klee_assume(res < 0);
        // klee_assume(a[0] == b[0]);
        // klee_assume(a[1] == b[1]);
        // klee_assume(a[2] == b[2]);
        // klee_assume(a[3] > b[3]);

        // klee_assume(res > 0);
        // klee_assume(a[0] == b[0]);
        // klee_assume(a[0] < b[0]);
}
