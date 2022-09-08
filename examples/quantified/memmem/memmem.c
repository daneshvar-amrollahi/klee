#include "klee/klee.h"
#include <stdio.h>

char* match(char* haystack, char* needle, uint32_t haystack_len, uint32_t needle_len) {
        uint32_t ret;
        klee_make_symbolic(&ret, sizeof(ret), "memmem_return_value");
        klee_memmem(haystack, needle, haystack_len, needle_len, ret);
        return haystack + ret;
}

int main() {
        char haystack[5], needle[2];
        uint32_t haystack_len = 5, needle_len = 2;
        klee_make_symbolic(haystack, haystack_len, "haystack");
        klee_make_symbolic(needle, needle_len, "needle");
        char* res = match(haystack, needle, haystack_len, needle_len);
        
        
        // Test #0: match found on index 2 - check for no occurences before that
        // klee_assume(res == haystack + 2);
        // klee_assume(haystack[1] == needle[0]); 
        // klee_assume(haystack[2] == needle[1]); // should fail on this
        

        
        // Test #1: match found - check if match is actually found
        // klee_assume(res == haystack + 2);
        // klee_assume(haystack[2] == needle[0]);
        // klee_assume(haystack[3] != needle[1]); // should fail on this
        

       
        // Test #2: match not found 
        // klee_assume(res == haystack + haystack_len);
        // klee_assume(haystack[2] == needle[0]);
        // klee_assume(haystack[3] == needle[1]);
}
