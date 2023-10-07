#ifndef INCLUDE_LIST_H_
#define INCLUDE_LIST_H_

#include "Cloak/cloak.h"
#include "args.h"
#include "common.h"

#define GET_NTH_IMPL(n, ...)                                                   \
    IF (NOT(BOOL(n)))                                                          \
    (GET_FIRST(__VA_ARGS__),                                                   \
     OBSTRUCT(GET_NTH_IMPL_INDIRECT)()(DEC(n), WO_FIRST(__VA_ARGS__)))

#define GET_NTH_IMPL_INDIRECT() GET_NTH_IMPL
#define GET_NTH(n, ...)         EVAL(GET_NTH_IMPL(n, __VA_ARGS__))
#define GET_LIST_NTH(n, list)   GET_NTH(n, EXPAND list)

#define GET_LAST_IMPL(...)                                                     \
    IF (HAS_ONE_ARG(__VA_ARGS__))                                              \
    (GET_FIRST(__VA_ARGS__),                                                   \
     OBSTRUCT(GET_LAST_IMPL_INDIRECT)()(WO_FIRST(__VA_ARGS__)))

#define GET_LAST_IMPL_INDIRECT() GET_LAST_IMPL
#define GET_LAST(...)            EVAL(GET_LAST_IMPL(__VA_ARGS__))

#define ZIP_IMPL(l1, l2)                                                       \
    IF (HAS_ONE_ARG l1)                                                        \
    ((GET_FIRST l1, GET_FIRST l2), (GET_FIRST l1, GET_FIRST l2),               \
     OBSTRUCT(ZIP_IMPL_INDIRECT)()((WO_FIRST l1), (WO_FIRST l2)))

#define ZIP_IMPL_INDIRECT() ZIP_IMPL
#define ZIP(l1, l2)         EVAL(ZIP_IMPL(l1, l2))
#define ZIP_TO_LIST(l1, l2) (ZIP(l1, l2))

#define LIST_HEAD_IMPL(x, ...) x

#define REMOVE_COMMAS(...) MAP_EACH_NO_COMMA(EXPAND, __VA_ARGS__)

#define LIST_LAST(l)             GET_LAST(EXPAND l)
#define LIST_HEAD(l)             LIST_HEAD_IMPL l
#define LIST_TAIL(l)             (WO_FIRST l)
#define LIST_SIZE(l)             PP_NARG l
#define LIST_GET(l, i)           GET_LIST_NTH(i, l)
#define LIST_MAP_NO_COMMA(op, l) MAP_EACH_NO_COMMA(op, EXPAND l)
#define LIST_UNLIST(l)           MAP_EACH_NO_COMMA(EXPAND, EXPAND l)
#define LIST_MAP(op, l)          MAP_EACH(op, EXPAND l)
/* Example:
    #define TOSTR_(x) #x
    #define TOSTR(x) TOSTR_(x)
    LIST_MAP(TOSTR, (1, 2, 3))
--> "1", "2", "3" <--
*/

/* These both produce the same thing:
    #define FUNC(a, b) a = b;
    #define OP(a) FUNC a
    LIST_UNLIST  ( (LIST_MAP(OP, ZIP_TO_LIST((1,2,3), (4,5,6)))) )
    REMOVE_COMMAS  (LIST_MAP(OP, ZIP_TO_LIST((1,2,3), (4,5,6))))
--> 1 = 4; 2 = 5; 3 = 6; <--
*/

#endif // INCLUDE_LIST_H_
