#pragma once

#include "ad/tape/variable.h"
#include "ad/functor/binary_functor.h"
#include "ad/functor/derivative_functor.h"

#define DEFINE_SPECIFIC_VARIABLE_VARIABLE_BINARY(OPERATOR, NAME)                \
namespace ad { namespace tape {                                                 \
    template <typename V, typename D, template<typename> typename G>            \
    variable<V, D, G> operator OPERATOR(                                        \
        const variable<V, D, G>& e0,                                            \
        const variable<V, D, G>& e1)                                            \
    {                                                                           \
        using variable_type = variable<V, D, G>;                                \
        using functor_type = ad::functor::NAME##_functor<                       \
            typename variable_type::value_type,                                 \
            typename variable_type::value_type                                  \
        >;                                                                      \
        using derivative_functor_type                                           \
            = ad::functor::derivative_functor<functor_type>;                    \
        const auto x0 = static_cast<typename variable_type::value_type>(e0);    \
        const auto x1 = static_cast<typename variable_type::value_type>(e1);    \
        const auto y = e0.generate_and_link(                                    \
            functor_type::apply(x0, x1),                                        \
            derivative_functor_type::apply0(x0, x1));                           \
        e1.link_from_this(y, derivative_functor_type::apply1(x0, x1));          \
        return y;                                                               \
    }                                                                           \
} }

DEFINE_SPECIFIC_VARIABLE_VARIABLE_BINARY(+, plus);
DEFINE_SPECIFIC_VARIABLE_VARIABLE_BINARY(-, minus);
DEFINE_SPECIFIC_VARIABLE_VARIABLE_BINARY(*, multiply);
DEFINE_SPECIFIC_VARIABLE_VARIABLE_BINARY(/, divide);

#undef DEFINE_SPECIFIC_VARIABLE_VARIABLE_BINARY