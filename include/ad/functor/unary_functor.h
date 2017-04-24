#pragma once

#include <cmath>
#include <type_traits>

namespace ad { namespace functor {
    template <typename T>
    struct negate_functor {
        using result_type = decltype(-std::declval<T>());
        static result_type apply(const T& x)
        {
            return -x;
        }
    };
}}

#define DEFINE_UNARY_FUNCTOR_FOR_AD_BU(NAME)                                   \
namespace ad { namespace functor {                                             \
    template <typename T>                                                      \
    struct NAME##_functor {                                                    \
        using result_type = decltype(std::NAME(std::declval<T>()));            \
        static result_type apply(const T& x)                                   \
        {                                                                      \
            return std::NAME(x);                                               \
        }                                                                      \
    };                                                                         \
}} 

DEFINE_UNARY_FUNCTOR_FOR_AD_BU(sin);
DEFINE_UNARY_FUNCTOR_FOR_AD_BU(cos);
DEFINE_UNARY_FUNCTOR_FOR_AD_BU(tan);
DEFINE_UNARY_FUNCTOR_FOR_AD_BU(sqrt);
DEFINE_UNARY_FUNCTOR_FOR_AD_BU(log);
DEFINE_UNARY_FUNCTOR_FOR_AD_BU(exp);
DEFINE_UNARY_FUNCTOR_FOR_AD_BU(erf);

#undef DEFINE_UNARY_FUNCTOR_FOR_AD_BU
