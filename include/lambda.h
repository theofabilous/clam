#ifndef INCLUDE_LAMBDA_H
#define INCLUDE_LAMBDA_H

#include "Cloak/cloak.h"
#include "args.h"
#include "common.h"
#include "list.h"

/*
 * MSVC's default preprocessor does not conform to the C standard (LMAO)
 * so throw a compilation error if the bad one is being used.
 *
 * In 2022, Microsoft *finally* implemented a standard compliant
 * preprocessor. Yes, a company with a literal trillion-dollar market
 * cap took 30 years to do this. Let that sink in.
 *
 * MSVC defines the _MSVC_TRADITIONAL predefined macro to check for this
 * but I think Clang (and maybe Intel?) defines _MSC_VER when using the
 * MSVC backend, and I'm not sure which preprocessor it uses or if it
 * appropriately defines that macro. So instead just check for a known
 * pitfall of MSVC's traditional preprocessor, and this may also
 * catch other invalid preprocessors.
 */
#define INVALID_PP(...)            INVALID_PP_IMPL(__VA_ARGS__, 1)
#define INVALID_PP_IMPL(a, b, ...) b
/* A valid preprocessor will forward ~, 0 as two arguments to INVALID_PP_IMPL,
 * thus making the argument 'b' take the value 0.
 * MSVC's traditional preprocessor will forward those as a single one, so
 * the 'b' argument will take the value 1.
 */
#if INVALID_PP(~, 0)
#    if defined(_MSC_VER)
#        error "Invalid preprocessor. Please use the /Zc:preprocessor switch"
#    else
#        error "Invalid preprocessor"
#    endif
#endif
#undef INVALID_PP
#undef INVALID_PP_IMPL

#define PARENS_DESTROY(...)
#define PARENS_ADD_COMMA_IMPL(...) (__VA_ARGS__),
#define PARENS_ADD_COMMA(...)      PARENS_ADD_COMMA_IMPL __VA_ARGS__
#define PARENS_ADD_COMMA_SURR(...) (PARENS_ADD_COMMA(__VA_ARGS__))

#define PARENS_EAT(...)  PARENS_DESTROY __VA_ARGS__
#define PARENS_KEEP(...) EXPAND(GET_FIRST PARENS_ADD_COMMA_SURR(__VA_ARGS__))

#define ID_LEFT_PARENS(...) (__VA_ARGS__

#if !defined(USE_LOCAL_LABELS) && !defined(USE_STATEMENT_EXPRESSIONS) &&       \
    !defined(USE_TRY_FINALLY)
#    if defined(__GNUC__) || defined(__GNUG__)
#        define USE_LOCAL_LABELS          1
#        define USE_STATEMENT_EXPRESSIONS 1
#        define USE_TRY_FINALLY           0
#    elif defined(_MSC_VER) && defined(_MSC_EXTENSIONS)
#        define USE_LOCAL_LABELS          0
#        define USE_STATEMENT_EXPRESSIONS 0
#        define USE_TRY_FINALLY           1
#    endif
#endif

#if USE_LOCAL_LABELS

#    define LAMBDA_EXPAND_BODY(_lambda)                                        \
        PARENS_EAT(_lambda)                                                    \
    _returnlabel:;
#    if USE_STATEMENT_EXPRESSIONS
#        define LAMBDA_EXPAND_BODY_R(rtype, _lambda)                           \
            rtype retval;                                                      \
            PARENS_EAT(_lambda)                                                \
        _returnlabel:;                                                         \
            retval;
#    endif
#    define yield(...)                                                         \
        do {                                                                   \
            VA_OPT_COMPAT(__VA_ARGS__)                                         \
            (retval = __VA_ARGS__;) goto _returnlabel;                         \
        } while (0)
#    define BEGIN_BLOCK(...)                                                   \
        __label__ _returnlabel;                                                \
        VA_OPT_COMPAT(__VA_ARGS__)(; __VA_ARGS__ retval)

#elif USE_TRY_FINALLY

#    define LAMBDA_EXPAND_BODY(_lambda)                                        \
        __try {                                                                \
            PARENS_EAT(_lambda)                                                \
        } __finally {                                                          \
            ((void)0);                                                         \
        }
#    define yield(...)                                                         \
        do {                                                                   \
            VA_OPT_COMPAT(__VA_ARGS__)(retval = __VA_ARGS__;) __leave;         \
        } while (0)
#    define BEGIN_BLOCK(...)                                                   \
        ((void)0) VA_OPT_COMPAT(__VA_ARGS__)(; __VA_ARGS__ retval)

#else
/* If neither local labels nor try-finally blocks are used,
 * AFAIK there's no reliable way to support the yield() macro
 * without it accepting a label identifier as an argument which
 * it jumps to. The label should be placed at the end of
 * the lambda's body so that yield correctly 'returns' from the
 * lambda.
 */
#    define LAMBDA_EXPAND_BODY(_lambda, ...)                                   \
        PARENS_EAT(_lambda)                                                    \
        VA_OPT_COMPAT(__VA_ARGS__)(__VA_ARGS__:;)
#    define yield(x, ...)                                                      \
        IF (PP_NARG(__VA_ARGS__))                                              \
        (                                                                      \
            do {                                                               \
                retval = x;                                                    \
                goto __VA_ARGS__;                                              \
            } while (0),                                                       \
            goto x                                                             \
        )
#    define BEGIN_BLOCK(...) ((void)0)

#endif


#define ASSIGN_FUNC(a, b) a = b;
#define ASSIGN_OP(a) ASSIGN_FUNC a

#define LAMBDA_ASSIGN_IMPL(_lambda, args)      \
    LIST_MAP(                               \
        ASSIGN_OP,                          \
        ZIP_TO_LIST(                        \
            PARENS_KEEP(_lambda),           \
            args                            \
        )                                   \
    )

#define LAMBDA_ASSIGN(_lambda, args) REMOVE_COMMAS(LAMBDA_ASSIGN_IMPL(_lambda, args))

#define LAMBDA_CALL_LIST(_lambda, args)         \
    LAMBDA_ASSIGN(_lambda, args)           \
    LAMBDA_EXPAND_BODY(_lambda)

#define LAMBDA_CALL(_lambda, ...)                   \
    LAMBDA_CALL_LIST(_lambda, (__VA_ARGS__))

#if USE_STATEMENT_EXPRESSIONS && USE_LOCAL_LABELS
#    define LAMBDA_CALL_R(rtype, _lambda, args)                                 \
        ({                                                                     \
            __label__ _returnlabel;                                            \
            LAMBDA_ASSIGN(_lambda, args)                                       \
            LAMBDA_EXPAND_BODY_R(rtype, _lambda)                               \
        })
#endif

#endif // INCLUDE_LAMBDA_H
