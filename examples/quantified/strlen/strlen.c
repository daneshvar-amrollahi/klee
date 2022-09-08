#include "klee/klee.h"

int main() {
        char str[5];
        klee_make_symbolic(str, sizeof(str), "str");
        int len_str;
        klee_make_symbolic(&len_str, sizeof(len_str), "strlen_return_value");
        klee_strlen(str, len_str);   

        for (int i = 0; i < 5; i++)
                klee_assume(str[i] != 0);

        // klee_assume(len_str < 5); //should fail on this
}
