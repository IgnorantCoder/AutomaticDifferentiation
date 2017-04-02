#pragma once

#include <type_traits>

#define DEFINE_BINARY_FUNCTOR_FOR_AD_BU(OPERATOR, NAME)                        \
namespace ad { namespace bu { namespace detail {                               \
    template <typename T0, typename T1>                                        \
    struct NAME##_functor {                                                    \
        using result_type                                                      \
            = decltype(std::declval<T0>() OPERATOR std::declval<T1>());        \
        static result_type apply(const T0& x0, const T1& x1)                   \
        {                                                                      \
            return x0 OPERATOR x1;                                             \
        }                                                                      \
    };                                                                         \
} } }

DEFINE_BINARY_FUNCTOR_FOR_AD_BU(+, plus);
DEFINE_BINARY_FUNCTOR_FOR_AD_BU(-, minus);
DEFINE_BINARY_FUNCTOR_FOR_AD_BU(*, multiply);
DEFINE_BINARY_FUNCTOR_FOR_AD_BU(/, divide);

#undef DEFINE_BINARY_FUNCTOR_FOR_AD_BU
