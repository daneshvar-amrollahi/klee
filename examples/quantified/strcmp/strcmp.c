#include "klee/klee.h"
#include <stdio.h>

int match(char* a, char* b, uint32_t a_len, uint32_t b_len) {
        uint32_t ret;
        klee_make_symbolic(&ret, sizeof(ret), "strspn_return_value");
        klee_strcmp(a, b, a_len, b_len, ret);
        return ret;
}

int main() {
        char a[5], b[3];
        uint32_t a_len = strlen(a), b_len = strlen(b);
        klee_make_symbolic(a, sizeof(a), "a");
        klee_make_symbolic(b, sizeof(b), "b");
        int ret = match(a, b, a_len, b_len);

        // Test #0: a < b
        // klee_assume(a[0] == b[0]);
        // klee_assume(a[1] == b[1]);
        // klee_assume(a[2] < b[2]);
        // klee_assume(!(ret < 0)); //should fail

        // Test #1: a > b
        // klee_assume(a[0] == b[0]);
        // klee_assume(a[1] == b[1]);
        // klee_assume(a[2] > b[2]);
        // klee_assume(!(ret > 0)); //should fail

        // Test #2: a > b
        // klee_assume(a[0] == b[0]);
        // klee_assume(a[1] == b[1]);
        // klee_assume(a[2] == b[2]);
        // klee_assume(a[3] > 0);
        // klee_assume(a[4] > 0);
        // klee_assume(!(ret > 0)); //should fail
}
