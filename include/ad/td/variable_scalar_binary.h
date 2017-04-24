#pragma once

#include <type_traits>

#include "ad/td/variable.h"
#include "ad/td/variable_expression.h"
#include "ad/functor/binary_functor.h"
#include "ad/functor/derivative_functor.h"

#define DEFINE_SPECIFIC_VARIABLE_SCALAR_BINARY(OPERATOR, NAME)                  \
namespace ad { namespace td {                                                   \
    template <typename C, typename E> inline                                    \
    std::enable_if_t<                                                           \
        !is_variable_expression<C>::value,                                      \
        variable<                                                               \
            typename ad::functor::NAME##_functor<                               \
                typename E::value_type,                                         \
                C                                                               \
            >::result_type,                                                     \
            typename ad::functor::derivative_functor<                           \
                ad::functor::NAME##_functor<                                    \
                    typename E::value_type,                                     \
                    C                                                           \
                >                                                               \
            >::result0_type                                                     \
        >                                                                       \
    > operator OPERATOR(                                                        \
        const variable_expression<E>& e,                                        \
        const C& c)                                                             \
    {                                                                           \
        using functor_type = ad::functor::NAME##_functor<                       \
            typename E::value_type,                                             \
            C                                                                   \
        >;                                                                      \
        using derivative_functor_type                                           \
            = ad::functor::derivative_functor<functor_type>;                    \
                                                                                \
        const auto x = static_cast<typename E::value_type>(e());                \
        const auto y = e().generate_and_link(                                   \
            functor_type::apply(x, c),                                          \
            derivative_functor_type::apply0(x, c));                             \
        return y;                                                               \
    }                                                                           \
} }

DEFINE_SPECIFIC_VARIABLE_SCALAR_BINARY(+, plus);
DEFINE_SPECIFIC_VARIABLE_SCALAR_BINARY(-, minus);
DEFINE_SPECIFIC_VARIABLE_SCALAR_BINARY(*, multiply);
DEFINE_SPECIFIC_VARIABLE_SCALAR_BINARY(/, divide);

#undef DEFINE_SPECIFIC_VARIABLE_SCALAR_BINARY