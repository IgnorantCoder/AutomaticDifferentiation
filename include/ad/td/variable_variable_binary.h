#pragma once

#include "ad/td/variable.h"
#include "ad/td/variable_expression.h"
#include "ad/functor/binary_functor.h"
#include "ad/functor/derivative_functor.h"

#define DEFINE_SPECIFIC_VARIABLE_VARIABLE_BINARY(OPERATOR, NAME)               \
namespace ad { namespace td {                                                  \
    template <typename E0, typename E1>                                        \
    variable<                                                                  \
        typename ad::functor::NAME##_functor<                                  \
            typename E0::value_type,                                           \
            typename E1::value_type                                            \
        >::result_type,                                                        \
        typename ad::functor::derivative_functor<                              \
            ad::functor::NAME##_functor<                                       \
                typename E0::value_type,                                       \
                typename E1::value_type                                        \
            >                                                                  \
        >::result0_type                                                         \
    > operator OPERATOR(                                                       \
        const variable_expression<E0>& e0,                                     \
        const variable_expression<E1>& e1)                                     \
    {                                                                          \
        using functor_type = ad::functor::NAME##_functor<                      \
            typename E0::value_type,                                           \
            typename E1::value_type                                            \
        >;                                                                     \
        using derivative_functor_type                                          \
            = ad::functor::derivative_functor<functor_type>;                   \
                                                                               \
        const auto x0 = static_cast<typename E0::value_type>(e0());            \
        const auto x1 = static_cast<typename E1::value_type>(e1());            \
        const auto y = e0().generate_and_link(                                 \
            functor_type::apply(x0, x1),                                       \
            derivative_functor_type::apply0(x0, x1));                          \
        e1().link(y, derivative_functor_type::apply1(x0, x1));                 \
                                                                               \
        return y;                                                              \
    }                                                                          \
} }

DEFINE_SPECIFIC_VARIABLE_VARIABLE_BINARY(+, plus);
DEFINE_SPECIFIC_VARIABLE_VARIABLE_BINARY(-, minus);
DEFINE_SPECIFIC_VARIABLE_VARIABLE_BINARY(*, multiply);
DEFINE_SPECIFIC_VARIABLE_VARIABLE_BINARY(/ , divide);

#undef DEFINE_SPECIFIC_VARIABLE_VARIABLE_BINARY
