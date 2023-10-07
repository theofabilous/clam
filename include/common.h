#ifndef INCLUDE_COMMON_H_
#define INCLUDE_COMMON_H_

#include "Cloak/cloak.h"
#include "args.h"

#define PP_NARG_CHECK_0_ARG_0 PROBE(~)
#define PP_NARG_CHECK_1_ARG_1 PROBE(~)
#define ONE_ARG(...)          CAT(PP_NARG_CHECK_1_ARG_, PP_NARG(__VA_ARGS__))
#define HAS_ONE_ARG(...)      BOOL(CHECK(ONE_ARG(__VA_ARGS__)))
#define ZERO_ARGS(...)        CAT(PP_NARG_CHECK_0_ARG_, PP_NARG(__VA_ARGS__))
#define HAS_ZERO_ARGS(...)    BOOL(CHECK(ZERO_ARGS(__VA_ARGS__)))

#define GET_FIRST(x, ...) x
#define WO_FIRST(x, ...)  __VA_ARGS__
#define VA_OPT_COMPAT(...)                                                     \
    IF (PP_NARG(__VA_ARGS__))                                                  \
    (EXPAND, EAT)

#define MAP_EACH_IMPL(op, ...)                                                 \
    IF (HAS_ONE_ARG(__VA_ARGS__))                                              \
    (op(__VA_ARGS__), op(GET_FIRST(__VA_ARGS__)),                              \
     OBSTRUCT(MAP_EACH_IMPL_INDIRECT)()(op, WO_FIRST(__VA_ARGS__)))

#define MAP_EACH_IMPL_INDIRECT()    MAP_EACH_IMPL
#define MAP_EACH_NOT_EMPTY(op, ...) EVAL(MAP_EACH_IMPL(op, __VA_ARGS__))
#define MAP_EACH(op, ...)                                                      \
    IF (HAS_ZERO_ARGS(__VA_ARGS__))                                            \
    (, MAP_EACH_NOT_EMPTY(op, __VA_ARGS__))

#define MAP_EACH_NO_COMMA_IMPL(op, ...)                                        \
    IF (HAS_ONE_ARG(__VA_ARGS__))                                              \
    (op(__VA_ARGS__),                                                          \
     op(GET_FIRST(__VA_ARGS__)                                                 \
     ) OBSTRUCT(MAP_EACH_NO_COMMA_IMPL_INDIRECT)()(op, WO_FIRST(__VA_ARGS__)))

#define MAP_EACH_NO_COMMA_IMPL_INDIRECT() MAP_EACH_NO_COMMA_IMPL
#define MAP_EACH_NO_COMMA_NOT_EMPTY(op, ...)                                   \
    EVAL(MAP_EACH_NO_COMMA_IMPL(op, __VA_ARGS__))
#define MAP_EACH_NO_COMMA(op, ...)                                             \
    IF (HAS_ZERO_ARGS(__VA_ARGS__))                                            \
    (, MAP_EACH_NO_COMMA_NOT_EMPTY(op, __VA_ARGS__))

/* Example usage:
 *  #define MY_OP_IMPL(x) # x
 *  #define MY_OP(x) MY_OP_IMPL(x)
 *  const char* A[] = { MAP_EACH(MY_OP, l, l, l) };
 *
 *  const char* A[] = { "l", "l", "l" };
 */

#endif // INCLUDE_COMMON_H_
