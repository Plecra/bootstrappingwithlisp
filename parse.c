#include <stdlib.h>
#include <inttypes.h>
#include "parse.h"

static int is_whitespace(char c) {
    return c == ' ' || c == '\n';
}
static int is_identifier(char c) {
    return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') || ('0' <= c && c <= '9');
}
static void strip(char **src) {
    while (is_whitespace(**src)) (*src)++;
}
struct value parse_list(char **src) {
    strip(src);

    if (**src == ')') {
        (*src)++;
        return NIL;
    } else {
        struct value *head = malloc(sizeof *head);
        struct value *tail = malloc(sizeof *tail);
        *head = parse_expr(src);
        *tail = parse_list(src);
        return (struct value){
            .type = nodet_pair,
            .value = {
                .pair = {
                    .head = head,
                    .tail = tail,
                }
            }
        };
    }
}
// struct value parse_file(char *src, struct arena *arena) {
//     struct value body = {
//         .type = nodet_pair,
//         .value = {
//             .pair = {
//                 .head = &IDENT(do),
//                 .tail = &NIL,
//             }
//         }
//     };
//     struct value **cursors[16] = {&body.value.pair.tail};
//     int depth = 0;
//     while(1) switch (*src++) {
//         case '\n':
//         case ' ': break;
//         case 0: return body;
//         case ')':
//             if (depth == 0) {
//                 struct value *err = alloc(arena, sizeof(struct value));
//                 *err = (struct value){
//                     .type = nodet_err_toomanycloseparen,
//                     .value = {
//                         .err_toomanycloseparen = src - 1
//                     }
//                 };
//                 struct value *link = alloc(arena, sizeof(struct value));
//                 *link = PAIR(*err, NIL);
//                 *cursors[0] = link;
//                 cursors[0] = &link->value.pair.tail;
//             }
//             depth -= 1;
//             break;
//         case '(':
//             if (depth == 15) exit(1); // todo: skip node and continue parsing
//             cursors[depth + 1] =

//     }
// }
struct value parse_expr(char **src) {
    strip(src);
    char c = *(*src)++;
    if (c == '(') {
        return parse_list(src);
    } else if (('A' < c && c < 'Z') || ('a' < c && c < 'z')) {
        char *start = *src - 1;
        while (is_identifier(**src)) (*src)++;
        return (struct value){
            .type = nodet_ident,
            .value = {
                .ident = {
                    .addr = start,
                    .len = *src - start,
                }
            }
        };
    } else if ('0' <= c && c <= '9') {
        int n = c - '0';
        while (c = **src, '0' <= c && c <= '9') {
            n = n * 10 + (c - '0');
            (*src)++;
        }
        return (struct value){
            .type = nodet_int,
            .value = {
                .i32 = n,
            }
        };
    } else {
        fprintf(stderr, "not a token");
        abort();
    }
}