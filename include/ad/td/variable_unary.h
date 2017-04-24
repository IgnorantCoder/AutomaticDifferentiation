#pragma once

#include "ad/td/variable.h"
#include "ad/td/variable_expression.h"
#include "ad/functor/derivative_functor.h"
#include "ad/functor/unary_functor.h"

namespace ad { namespace td {
    template <typename E>
    inline variable<
        typename ad::functor::negate_functor<typename E::value_type>::result_type,
        typename ad::functor::derivative_functor<
            ad::functor::negate_functor<typename E::value_type>
        >::result_type
    >
    operator -(const variable_expression<E>& e)
    {
        using functor_type 
            = ad::functor::negate_functor<typename E::value_type>;
        using derivative_functor_type
            = ad::functor::derivative_functor<functor_type>;

        const auto x = static_cast<typename E::value_type>(e());
        const auto y = e().generate_and_link(
            functor_type::apply(x),
            derivative_functor_type::apply(x));

        return y;
    }
}}

#define DEFINE_SPECIFIC_VARIABLE_UNARY(NAME)                                    \
namespace ad { namespace td {                                                   \
    template <typename E>                                                       \
    inline variable<                                                            \
        typename ad::functor::NAME##_functor<                                   \
            typename E::value_type                                              \
        >::result_type,                                                         \
        typename ad::functor::derivative_functor<                               \
            ad::functor::NAME##_functor<typename E::value_type>                 \
        >::result_type                                                          \
    >                                                                           \
    NAME(const variable_expression<E>& e)                                       \
    {                                                                           \
        using functor_type                                                      \
            = ad::functor::NAME##_functor<typename E::value_type>;              \
        using derivative_functor_type                                           \
            = ad::functor::derivative_functor<functor_type>;                    \
                                                                                \
        const auto x = static_cast<typename E::value_type>(e());                \
        const auto y = e().generate_and_link(                                   \
            functor_type::apply(x),                                             \
            derivative_functor_type::apply(x));                                 \
                                                                                \
        return y;                                                               \
    }                                                                           \
}}                                                                              \
namespace std {                                                                 \
    using ad::td::NAME;                                                         \
}

DEFINE_SPECIFIC_VARIABLE_UNARY(sqrt);
DEFINE_SPECIFIC_VARIABLE_UNARY(sin);
DEFINE_SPECIFIC_VARIABLE_UNARY(cos);
DEFINE_SPECIFIC_VARIABLE_UNARY(tan);
DEFINE_SPECIFIC_VARIABLE_UNARY(log);
DEFINE_SPECIFIC_VARIABLE_UNARY(exp);
DEFINE_SPECIFIC_VARIABLE_UNARY(erf);

#undef DEFINE_SPECIFIC_VARIABLE_UNARY