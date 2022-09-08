#include "klee/klee.h"
#include <stdio.h>

int match(char* a, char* b, uint32_t a_len, uint32_t b_len) {
        uint32_t ret;
        klee_make_symbolic(&ret, sizeof(ret), "strspn_return_value");
        klee_strspn(a, b, a_len, b_len, ret);
        return ret;
}

int main() {
        char a[5], b[3];
        uint32_t a_len = strlen(a), b_len = strlen(b);
        klee_make_symbolic(a, sizeof(a), "a");
        klee_make_symbolic(b, sizeof(b), "b");
        int ret = match(a, b, a_len, b_len);

        klee_assume(a[0] == 'a');
        klee_assume(a[1] == 'b');
        klee_assume(a[2] == 'c');
        klee_assume(a[3] == 'd');
        klee_assume(a[4] == 'e');

        klee_assume(b[0] == 'b');
        klee_assume(b[1] == 'a');
        klee_assume(b[2] == 'z');

        klee_assume(ret != 2); //should fail
}
