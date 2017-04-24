#pragma once

#include <type_traits>

#include "ad/td/variable.h"
#include "ad/td/variable_expression.h"
#include "ad/functor/binary_functor.h"
#include "ad/functor/derivative_functor.h"

#define DEFINE_SPECIFIC_SCALAR_VARIABLE_BINARY(OPERATOR, NAME)                  \
namespace ad { namespace td {                                                   \
    template <typename C, typename E> inline                                    \
    std::enable_if_t<                                                           \
        !is_variable_expression<C>::value,                                      \
        variable<                                                               \
            typename ad::functor::NAME##_functor<                               \
                C,                                                              \
                typename E::value_type                                          \
            >::result_type,                                                     \
            typename ad::functor::derivative_functor<                           \
                ad::functor::NAME##_functor<                                    \
                    C,                                                          \
                    typename E::value_type                                      \
                >                                                               \
            >::result1_type                                                     \
        >                                                                       \
    > operator OPERATOR(                                                        \
        const C& c,                                                             \
        const variable_expression<E>& e)                                        \
    {                                                                           \
        using functor_type = ad::functor::NAME##_functor<                       \
            C,                                                                  \
            typename E::value_type                                              \
        >;                                                                      \
        using derivative_functor_type                                           \
            = ad::functor::derivative_functor<functor_type>;                    \
                                                                                \
        const auto x = static_cast<typename E::value_type>(e());                \
        const auto y = e().generate_and_link(                                   \
            functor_type::apply(c, x),                                          \
            derivative_functor_type::apply1(c, x));                             \
        return y;                                                               \
    }                                                                           \
} }

DEFINE_SPECIFIC_SCALAR_VARIABLE_BINARY(+, plus);
DEFINE_SPECIFIC_SCALAR_VARIABLE_BINARY(-, minus);
DEFINE_SPECIFIC_SCALAR_VARIABLE_BINARY(*, multiply);
DEFINE_SPECIFIC_SCALAR_VARIABLE_BINARY(/ , divide);

#undef DEFINE_SPECIFIC_SCALAR_VARIABLE_BINARY