#include <string.h>

int get_used_ghost_vars(char *function_name) {
    if (strcmp(function_name, "klee_memcmp") == 0) {
        return 2;
    }
    return 0;
}

#define KLEE_SETUP_QUANTIFIERS                                                    \
  const int MAX_GHOST_SIZE = 128;                                                 \
  uint64_t ghost_variables[MAX_GHOST_SIZE];                                       \
  uint64_t ghost_variable_counter = 0;  