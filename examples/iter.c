#define USE_LOCAL_LABELS 0

#include "../include/lambda.h"
#include <stdio.h>

#define array_size(_array) (sizeof _array / sizeof _array[0])

#define foreach(_array, _lambda) {\
    int size = array_size(_array);                        \
    for(int j=0;j<size;j++){                \
        LAMBDA_CALL(_lambda, _array[j])    \
    }\
}\

int main() {
    int arr[] = {1, 2, 3, 4, 5};

    foreach(arr, (int i) {
        printf("%d\n", i);
        if (!(i % 2))
            printf("Even number!\n");
        else
            printf("Not an even number!\n");
    });
}
