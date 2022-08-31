#include "klee/klee.h"
#include <stdio.h>

int match(char* a, char* b, uint32_t a_len, uint32_t b_len) {
        uint32_t ret;
        klee_make_symbolic(&ret, sizeof(ret), "strspn_return_value");
        klee_strspn(a, b, a_len, b_len, ret);
        return ret;
}

int main() {
        char a[5], b[2];
        uint32_t a_len = 5, b_len = 2;
        klee_make_symbolic(a, a_len, "a_len");
        klee_make_symbolic(b, b_len, "b_len");

        int ret = match(a, b, a_len, b_len);

}
