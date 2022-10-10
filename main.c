#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "parse.h"
#define fail(e) {fprintf(stderr, e " on line %d", __LINE__); abort();}
#define expect(value, t) if (value.type != nodet_ ## t) fail("value is wrong type");

struct scope {
    struct string name;
    struct value value;
    struct scope *next;
};
struct value scope_get(struct scope *scope, struct string name) {
    if (scope) {
        if (ident_eq(scope->name, name)) return scope->value;
        else return scope_get(scope->next, name);
    } else return NIL;
}
struct value eval(struct value program, struct scope *scope);
struct value apply(struct value receiver, struct value args, struct scope *scope) {
    struct value f = eval(receiver, scope);
    switch (f.type) {
        case nodet_putc: {
            expect(args, pair);
            struct value value = eval(*args.value.pair.head, scope);
            expect(value, int);
            printf("%c", args.value.pair.head->value.i32);
            return NIL;
        }
        case nodet_do: {
            int newscopeslots = 0;
            struct value head = args;
            while (head.type == nodet_pair) {
                struct value arg = *head.value.pair.head;
                head = *head.value.pair.tail;
                if (arg.type == nodet_pair && arg.value.pair.head->type == nodet_ident
                  && ident_eq(arg.value.pair.head->value.ident, S(def))) {
                    struct value definition = *arg.value.pair.tail;
                    fprintf(stderr, "definition");
                    abort();
                } else eval(arg, scope);
            }
            return NIL;
        }
            
        default:
            fprintf(stderr, "unimpld function receiver");
            abort();
    }
}
struct value eval(struct value program, struct scope *scope) {
    switch (program.type) {
        case nodet_int: return program;
        case nodet_ident: return scope_get(scope, program.value.ident);
        case nodet_pair: return apply(*program.value.pair.head, *program.value.pair.tail, scope);
        default: fprintf(stderr, "eval'd non-ast");
                 abort();
    }
}

int main() {
    struct scope dovar = {
        .name = S(do),
        .value = { .type = nodet_do },
        .next = NULL,
    };
    struct scope putcvar = {
        .name = S(putc),
        .value = { .type = nodet_putc },
        .next = &dovar,
    };
    // struct arena arena = {
    //     .buffer = malloc(1024 * 1024 * 32),
    //     .capacity = 1024 * 1024 * 32,
    //     .filled = 0,
    // };
    char *src = "(putc 66) (putc 65) (putc 67))";
    struct value program = parse_expr(&src);
    struct value body = PAIR(program, NIL);
    struct value _do = IDENT(do);
    struct value fullprog = PAIR(_do, body);
    eval(fullprog, &putcvar);
}
