#pragma once
#include <type_traits>

#include "ad/tape/variable.h"
#include "ad/tape/variable_expression.h"
#include "ad/functor/binary_functor.h"
#include "ad/functor/derivative_functor.h"


#define DEFINE_SPECIFIC_SCALAR_VARIABLE_BINARY(OPERATOR, NAME)                  \
namespace ad { namespace tape {                                                 \
    template <typename C, typename V, typename D, template<typename> typename G>\
    std::enable_if_t <                                                          \
        !is_variable_expression<C>::value,                                      \
        variable<                                                               \
            typename ad::functor::NAME##_functor<C, V>::result_type,            \
            typename ad::functor::derivative_functor<                           \
                ad::functor::NAME##_functor<C, V>                               \
            >::result1_type,                                                    \
            G                                                                   \
        >                                                                       \
    > operator OPERATOR(const C& c, const variable<V, D, G>& e)                 \
    {                                                                           \
        using functor_type = ad::functor::NAME##_functor<C, V>;                 \
        using derivative_functor_type                                           \
            = ad::functor::derivative_functor<functor_type>;                    \
        const auto x = static_cast<V>(e);                                       \
        const auto y = e.generate_and_link(                                     \
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