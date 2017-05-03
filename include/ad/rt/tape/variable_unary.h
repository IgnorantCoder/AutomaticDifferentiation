#pragma once

#include "ad/rt/tape/variable.h"
#include "ad/functor/derivative_functor.h"
#include "ad/functor/unary_functor.h"

namespace ad { namespace rt { namespace tape {
    template <typename V, typename D, template<typename> typename G>
    inline variable<V, D, G> operator -(const variable<V, D, G>& e)
    {
        using variable_type = variable<V, D, G>;
        using functor_type
            = ad::functor::negate_functor<typename variable_type::value_type>;
        using derivative_functor_type
            = ad::functor::derivative_functor<functor_type>;
        const auto x = static_cast<typename variable_type::value_type>(e);
        const auto y = e.generate_and_link(
            functor_type::apply(x),
            derivative_functor_type::apply(x));
        return y;
    }
} } }


#define DEFINE_SPECIFIC_VARIABLE_UNARY(NAME)                                    \
namespace ad { namespace rt { namespace tape {                                  \
    template <typename V, typename D, template<typename> typename G>            \
    inline variable<V, D, G> NAME(const variable<V, D, G>& e)                   \
    {                                                                           \
        using variable_type = variable<V, D, G>;                                \
        using functor_type                                                      \
            = ad::functor::NAME##_functor<typename variable_type::value_type>;  \
        using derivative_functor_type                                           \
            = ad::functor::derivative_functor<functor_type>;                    \
        const auto x = static_cast<typename variable_type::value_type>(e);      \
        const auto y = e.generate_and_link(                                     \
            functor_type::apply(x),                                             \
            derivative_functor_type::apply(x));                                 \
        return y;                                                               \
    }                                                                           \
} } }                                                                           \
namespace std {                                                                 \
    using ad::rt::tape::NAME;                                                       \
}

DEFINE_SPECIFIC_VARIABLE_UNARY(sqrt);
DEFINE_SPECIFIC_VARIABLE_UNARY(sin);
DEFINE_SPECIFIC_VARIABLE_UNARY(cos);
DEFINE_SPECIFIC_VARIABLE_UNARY(tan);
DEFINE_SPECIFIC_VARIABLE_UNARY(log);
DEFINE_SPECIFIC_VARIABLE_UNARY(exp);
DEFINE_SPECIFIC_VARIABLE_UNARY(erf);

#undef DEFINE_SPECIFIC_VARIABLE_UNARY