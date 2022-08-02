#include "klee/klee.h"
#include <stdio.h>

int match(int *a, int* b, int n) 
{
        int i;
        klee_make_symbolic(&i, sizeof(i), "memcmp_return_value");
        int forall_quantified_var[1], exists_quantified_var[1];
        klee_make_symbolic(forall_quantified_var, sizeof(forall_quantified_var), "fqv");
        klee_make_symbolic(exists_quantified_var, sizeof(exists_quantified_var), "eqv");
        klee_assume(forall_quantified_var[0] < n);
        klee_assume(forall_quantified_var[0] >= 0);
        klee_assume(exists_quantified_var[0] < n);
        klee_assume(exists_quantified_var[0] >= 0);
        klee_assume(i == 0 || i == 1);
        klee_memcmp(forall_quantified_var, exists_quantified_var, a, b, n, i);
        return i;
}

// int match(int *a, int* b, int n) 
// {
//         int i = 0;
//         while (i < n && a[i] == b[i])
//                 i++;
//         if (i == n)
//                 return 0;
//         return 1;
// }



int main() {
        int a[5], b[5];
        klee_make_symbolic(a, sizeof(a), "a");
        klee_make_symbolic(b, sizeof(b), "b");

        if (!match(a, b, 5)) 
                if(a[0] != b[0])
                        klee_daneshvar();
}
