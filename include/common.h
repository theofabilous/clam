#ifndef INCLUDE_COMMON_H_
#define INCLUDE_COMMON_H_

#define GET_FIRST(x, ...) x
#define WO_FIRST(x, ...)  __VA_ARGS__
#define VA_OPT_COMPAT(...)                                                     \
    IF (PP_NARG(__VA_ARGS__))                                                  \
    (EXPAND, EAT)

#define EXPAND1(...) __VA_ARGS__
#define EXPAND2(...) __VA_ARGS__
#define EXPAND3(...) __VA_ARGS__
#define EXPAND4(...) __VA_ARGS__
#define EXPAND5(...) __VA_ARGS__
#define EXPAND6(...) __VA_ARGS__
#define EXPAND7(...) __VA_ARGS__
#define EXPAND8(...) __VA_ARGS__
#define EXPAND9(...) __VA_ARGS__
#define EXPAND0(...) __VA_ARGS__

#define MAP_EACH_IMPL(op, ...)                                                 \
    IF (COMPL(BOOL(DEC(PP_NARG(__VA_ARGS__)))))                                \
    (op(__VA_ARGS__), op(GET_FIRST(__VA_ARGS__)),                              \
     OBSTRUCT(MAP_EACH_IMPL_INDIRECT)()(op, WO_FIRST(__VA_ARGS__)))

#define MAP_EACH_IMPL_INDIRECT() MAP_EACH_IMPL
#define MAP_EACH(op, ...)        EVAL(MAP_EACH_IMPL(op, __VA_ARGS__))

#define MAP_EACH_NO_COMMA_IMPL(op, ...)                                        \
    IF (COMPL(BOOL(DEC(PP_NARG(__VA_ARGS__)))))                                \
    (op(__VA_ARGS__),                                                          \
     op(GET_FIRST(__VA_ARGS__)                                                 \
     ) OBSTRUCT(MAP_EACH_NO_COMMA_IMPL_INDIRECT)()(op, WO_FIRST(__VA_ARGS__)))

#define MAP_EACH_NO_COMMA_IMPL_INDIRECT() MAP_EACH_NO_COMMA_IMPL
#define MAP_EACH_NO_COMMA(op, ...)        EVAL(MAP_EACH_NO_COMMA_IMPL(op, __VA_ARGS__))

/* Example usage:
 *  #define MY_OP_IMPL(x) # x
 *  #define MY_OP(x) MY_OP_IMPL(x)
 *  const char* A[] = { MAP_EACH(MY_OP, l, l, l) };
 *
 *  const char* A[] = { "l", "l", "l" };
 */

#define CHECK_EQUAL_0_0 PROBE(~)
#define CHECK_EQUAL_1_1 PROBE(~)
#define CHECK_EQUAL_2_2 PROBE(~)
#define CHECK_EQUAL_3_3 PROBE(~)
#define CHECK_EQUAL_4_4 PROBE(~)
#define CHECK_EQUAL_5_5 PROBE(~)
#define CHECK_EQUAL_6_6 PROBE(~)
#define CHECK_EQUAL_7_7 PROBE(~)
#define CHECK_EQUAL_8_8 PROBE(~)
#define CHECK_EQUAL_9_9 PROBE(~)

#define CHECK_NUM_ARGS(n, ...)                                                 \
    CHECK(CAT(CAT(CHECK_EQUAL_, n), CAT(_, PP_NARG(__VA_ARGS__))))

#define CHECK_LIST_NUM_ARGS(n, list) CHECK_NUM_ARGS(n, EXPAND list)
#define CHECK_LAMBDA_NUM_ARGS(n, ...)                                          \
    CHECK_NUM_ARGS(n, LAMBDA_GET_ARGS(__VA_ARGS__))

#endif // INCLUDE_COMMON_H_
