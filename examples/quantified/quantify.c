#include "klee/klee.h"

int main() {
        int n;
        klee_make_symbolic(&n, sizeof (n), "n");

        if (n < 10)
                n = 10;

        klee_quantify(n + 1 >= 2 * n);
}
