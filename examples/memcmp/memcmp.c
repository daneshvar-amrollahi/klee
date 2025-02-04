#include "klee/klee.h"

int match(int *a, int* b, int n) 
{
        int i = 0;
        while (a[i] == b[i] && i < n)
	{
		klee_daneshvar();
                i++;
	}

return i;
}

int main() {
        int n;
        klee_make_symbolic(&n, sizeof (n), "n");
        klee_assume(n >= 0);
        klee_assume(n <= 5);

        for (int i = 0; i <= 5; i++){
                if(i == n){
                        n = i;
                        break;
                }
        }
        int a[10];
        klee_make_symbolic(a, sizeof (a), "a");

        int b[10];
        klee_make_symbolic(b, sizeof (b), "b");

        return match(a, b, n);
}
