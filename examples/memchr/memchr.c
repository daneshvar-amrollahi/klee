#include "klee/klee.h"
#include <stdio.h>

int main() {
        char* str;
        int c, n;
        klee_memchr(str, c, n);
}
