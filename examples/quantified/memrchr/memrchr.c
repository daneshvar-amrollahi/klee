#include "klee/klee.h"
#include <stdio.h>

char* match(char* str, char c, uint32_t n) {
        uint32_t ret, impliesVar;
        klee_make_symbolic(&ret, sizeof(ret), "memrchr_return_value");
        klee_memrchr(str, c, n, ret);
        return str + ret;
}

int main() {
        char str[5];
        uint32_t n = 5;
        char c;
        klee_make_symbolic(str, n, "str");
        klee_make_symbolic(&c, sizeof(c), "c");
        char* res = match(str, c, n);
        if (res == str + n)
        {
                // klee_assume(str[1] == c); // should fail on this, as expected 
                printf("match not found\n");
        }
        else
        {
                // klee_assume(res == str + 2);
                // klee_assume(str[3] == c);        
                printf("match found\n");
        }
        
}
