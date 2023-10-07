#define USE_LOCAL_LABELS 0

#include "../include/lambda.h"
#include <stdio.h>

#define array_size(_array) (sizeof _array / sizeof _array[0])

#define foreach(_array, _lambda) {\
    int size = array_size(_array);                        \
    for(int i=0;i<size;i++){                \
        LAMBDA_CALL(_lambda, _array[i])    \
    }\
}\

#define pipe(_iter) {\
    struct pipe_t {\
        int size;\
        /*union {*/\
            /* GET_LIST_NTH(0, _predicate)[64]*/ \
        /*} arr;*/\
        int acc[array_size(_iter)];\
    };\
}

#define fart() foo()

int foofunc(void) {
    return 1;
}

struct bar {
    int (*foo)(void);
};

int main() {
    int arr[] = {1, 2, 3, 4, 5};

    foreach(arr, (int i) {
        if (!(i % 2))
            printf("%d\n", i);
        else
            printf("Not an even number!\n");
    });

    struct bar b = { .foo = foofunc };
    b.fart();
}
