
## CLAM: Lambdas in `C`

```c
#define ARRAY_SIZE(a) (sizeof a / sizeof(*a))
struct S {
    const char *name;
    int n;
} things[] = {
    { "abc", 1 },
    { "def", 8787 },
    { "z", 0 },
    { "a", 1 },
};
ARRAY_SORT(
    things,
    ARRAY_SIZE(things),
    (struct S a, struct S b) {
        yield(strcmp(a.name, b.name));
    }
);
```
