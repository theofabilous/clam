
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define USE_LOCAL_LABELS 1
#include "../include/lambda.h"

__attribute__((always_inline))
__attribute__((unused))
static inline void ptrswap(
    register unsigned char * restrict a,
    register unsigned char * restrict b,
    int size
) {
    typedef union {
        uint64_t * restrict u64;
        uint32_t * restrict u32;
        uint16_t * restrict u16;
        uint8_t  * restrict u8;
    } _anyptr;
    register _anyptr a_ptr = { .u8 = a };
    register _anyptr b_ptr = { .u8 = b };
    register uint64_t temp;
#define SW(n)   temp = *a_ptr.u ## n;                                       \
                *a_ptr.u ## n++ = *b_ptr.u ## n;                            \
                *b_ptr.u ## n++ = temp;
    register int n = (size + 7) >> 3;
    switch (size & 0b111) {
        case 0: do {    SW(64); continue;
        case 7:         SW(8);
        case 6:         SW(16); goto _case4;
        case 5:         SW(8);
        case 4: _case4: SW(32); continue;
        case 3:         SW(8);
        case 2:         SW(16); continue;
        case 1:         SW(8);
        } while(--n);
    }
#undef SW
}

#define PTRSWAP(_data, _i, _j) (                                            \
    ptrswap(                                                                \
        (unsigned char *)((_data) + (_i)),                                  \
        (unsigned char *)((_data) + (_j)),                                  \
        sizeof(*(_data))                                                    \
    )                                                                       \
)

/* *************** comp(a, b) *************** */
#define COMP_IS_LESS_THAN(x)    ((x) < 0)       /* a < b */
#define COMP_IS_GREATER_THAN(x) ((x) > 0)       /* a > b */
#define COMP_IS_EQUAL(x)        ((x) == 0)      /* a == b */

#define _macro_QSORT_PARTITION(_data, _lo, _hi, _comp, _pret) {             \
    int _mk_pivot_idx = (_hi);                                              \
    int _mk_i = (_lo)-1;                                                    \
    for (int _mk_j = (_lo); _mk_j < (_hi); ++_mk_j) {                       \
        BEGIN_BLOCK(int);                                                   \
        LAMBDA_CALL(_comp, ((_data)[_mk_j]), (*((_data) + _mk_pivot_idx))); \
        if (COMP_IS_LESS_THAN(RETURN_VALUE)) {                              \
            ++_mk_i;                                                        \
            PTRSWAP((_data), _mk_i, _mk_j);                                 \
        }                                                                   \
    }                                                                       \
    PTRSWAP((_data), _mk_i + 1, (_hi));                                     \
    _pret = _mk_i + 1;                                                      \
}

#define _macro_QSORT_DO(_data, _size, _comp) {                              \
    int _mk_lo = 0;                                                         \
    int _mk_hi = (_size) - 1;                                               \
    /*  VLA  */                                                             \
    int _mk_stack[(_size)];                                                 \
    int _mk_sp = -1;                                                        \
    _mk_stack[++_mk_sp] = _mk_lo;                                           \
    _mk_stack[++_mk_sp] = _mk_hi;                                           \
    while (_mk_sp >= 0) {                                                   \
        int _mk_hi = _mk_stack[_mk_sp--];                                   \
        int _mk_lo = _mk_stack[_mk_sp--];                                   \
        int _mk_p;                                                          \
        _macro_QSORT_PARTITION((_data), _mk_lo, _mk_hi, _comp, _mk_p)       \
        if (_mk_p - 1 > _mk_lo) {                                           \
            _mk_stack[++_mk_sp] = _mk_lo;                                   \
            _mk_stack[++_mk_sp] = _mk_p - 1;                                \
        }                                                                   \
        if (_mk_p + 1 < _mk_hi) {                                           \
            _mk_stack[++_mk_sp] = _mk_p + 1;                                \
            _mk_stack[++_mk_sp] = _mk_hi;                                   \
        }                                                                   \
    }                                                                       \
}

#define ARRAY_SORT(_data, _size, _comp) _macro_QSORT_DO(_data, _size, _comp)

#define ARRAY_SIZE(a) (sizeof a / sizeof(*a))

int main() {
    printf("Hello, world!\n");
    int array[] = { 5, 4, 3, 2, 1, 999, -100, 0, 1, 1, 12 };
    for (int i = 0; i < ARRAY_SIZE(array); ++i) {
        printf("%d ", array[i]);
    }
    putchar('\n');
    ARRAY_SORT(array, ARRAY_SIZE(array), (int a, int b) {
        yield(a-b);
    });
    for (int i = 0; i < ARRAY_SIZE(array); ++i) {
        printf("%d ", array[i]);
    }
    putchar('\n');
    putchar('\n');

    struct S { const char *name; int n; };

    struct S things[] = {
        { "abc", 1 },
        { "def", 8787 },
        { "z", 0 },
        { "a", 1 },
    };
    for (int i = 0; i < ARRAY_SIZE(things); ++i) {
        printf("{%s %d} ", things[i].name, things[i].n);
    }
    putchar('\n');
    ARRAY_SORT(things, ARRAY_SIZE(things), (struct S a, struct S b) {
        yield(strcmp(a.name, b.name));
    });
    for (int i = 0; i < ARRAY_SIZE(things); ++i) {
        printf("{%s %d} ", things[i].name, things[i].n);
    }
    putchar('\n');

    return 0;
}
