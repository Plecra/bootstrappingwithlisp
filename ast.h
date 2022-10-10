#include <string.h>
#include <stdio.h>
// #define SENTINEL(n) 3 << 61 | n
// #define vNIL SENTINEL(0)
// #undef SENTINEL
// // Layout of values:
// // FFFX_XXXX_XXXXXXXX_XXXXXXXX_XXXXXXXX_NNNNNNNN_NNNNNNNN_NNNNNNNN_NNNNNNNN
// // 1__ cons        X|F: head N: tail
// // 00_ 62 bit uint
// // 010 string      X: length, N: addr
// // 011 sentinels   X|N sentinel as above
// typedef uint64_t value;
// typedef struct {
//     value *values;
//     uint32_t n_values;
//     uint8_t *mem;
//     uint32_t mem_cap;
// } vm;
// static void debug(FILE* stream, vm m, value v) {
//     if (v >> 63) {
//         fprintf(stream, "(");
//         uint32_t head = (v >> 32) & UINT64_MAX >> 33;
//         if (head >= m.n_values) fprintf(stream, "@Error");
//         else debug(m, m.values[head]);
//         fprintf(stream, " . ");
//         uint32_t tail = v & UINT64_MAX >> 32;
//         if (tail >= m.n_values) fprintf(stream, "@Error");
//         else debug(m, m.values[tail]);
//         fprintf(stream, ")");
//     } else if (v >> 62 == 0) {
//         fprintf(stream, "%d", v);
//     } else if (v & 1 << 61) {
//         fprintf(stream, ":special(%d)", v & !(1 << 61));
//     } else {
//         fprintf(stream, "%.*s", (v >> 32) & UINT64_MAX >> 35, v & UINT64_MAX >> 32);
//     }
// }
// union value {
//     struct cons {
//         uint32_t head;
//         uint32_t tail;
//     } cons;
//     struct lambda {
//         uint32_t body;
//         uint32_t scope;
//     } lambda;
//     struct number {
//         uint64_t value;
//     } number;
//     struct ident {
//         uint32_t addr;
//         uint32_t len;
//     } ident;
//     struct {
//         uint32_t sentinel;
//         uint32_t variant;
//     } special;
// }

struct string {
    char *addr;
    int len;
};
static int ident_eq(struct string a, struct string b) {
    return a.len == b.len && !memcmp(a.addr, b.addr, a.len);
}
#define S(s) (struct string){ .addr = #s, .len = sizeof(#s) - 1 }
#define IDENT(s) (struct value){ .type = nodet_ident, .value = { .ident = S(s) } }
#define PAIR(_head, _tail) (struct value){ .type = nodet_pair, .value = { .pair = { .head = &_head, .tail = &_tail }}}
#define NIL (struct value){ .type = nodet_nil, }
struct value;
struct value {
    enum {
        nodet_nil = 0,

        nodet_ident,
        nodet_pair,
        nodet_int,

        nodet_macro,
        nodet_lambda,

        nodet_do,
        nodet_putc,


        nodet_err_toomanycloseparen,
    } type;
    union {
        struct string ident;
        int i32;
        struct pair {
            struct value *head;
            struct value *tail;
        } pair;

        char *err_toomanycloseparen;
    } value;
};

static void debug(struct value val) {
    switch (val.type) {
        case nodet_int: printf("%d", val.value.i32); return;
        case nodet_pair:
            printf("(");
            debug(*val.value.pair.head);
            printf(" . ");
            debug(*val.value.pair.tail);
            printf(")");
            return;
        case nodet_ident: printf("%.*s", val.value.ident.len, val.value.ident.addr); return;
        case nodet_nil: printf("()"); return;
        case nodet_do: printf(":do"); return;
        case nodet_putc: printf(":putc"); return;
        default: printf("???");
    }
}