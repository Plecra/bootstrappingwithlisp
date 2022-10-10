#include <inttypes.h>
#include "ast.h"

// struct arena {
//     void *buffer;
//     size_t filled;
//     size_t capacity;
// };
// static char *alloc(struct arena *arena, size_t bytes) {
//     size_t new_filled = arena->filled + bytes;
//     if (new_filled > arena->capacity) return NULL;
//     char *ptr = arena->buffer + arena->filled;
//     arena->filled = new_filled;
//     return ptr;
// }

struct value parse_expr(char **src);