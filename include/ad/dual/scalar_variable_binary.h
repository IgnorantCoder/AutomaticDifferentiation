#pragma once

#include <type_traits>

#include "ad/dual/variable_expression.h"
#include "ad/functor/binary_functor.h"
#include "ad/functor/derivative_functor.h"

namespace ad { namespace dual {
    template <typename C, typename E, typename F>
    class scalar_variable_binary
        : public variable_expression<scalar_variable_binary<C, E, F>> {
    private:
        using functor_type = F;
        using derivative_functor_type
            = ad::functor::derivative_functor<functor_type>;

    public:
        using scalar_value_type = C;
        using value_type = typename functor_type::result_type;
        using derivative_value_type = typename E::derivative_value_type;
        using closure_type = scalar_variable_binary<C, E, F>;
        using index_map_type = typename E::index_map_type;

    public:
        scalar_variable_binary(const C& c, const E& e);

    public:
        operator value_type() const;
        derivative_value_type operator()(const std::size_t i) const;
        const index_map_type& index_mapper() const;

    private:
        scalar_value_type _c;
        typename E::closure_type _e;
    };

    template<typename C, typename E, typename F>
    inline scalar_variable_binary<C, E, F>::scalar_variable_binary(
        const C & c,
        const E & e)
        : _c(c), _e(e)
    {
    }

    template<typename C, typename E, typename F>
    inline scalar_variable_binary<C, E, F>::operator 
    typename scalar_variable_binary<C, E, F>::value_type() const
    {
        return functor_type::apply(_c, _e);
    }

    template<typename C, typename E, typename F>
    inline typename scalar_variable_binary<C, E, F>::derivative_value_type
    scalar_variable_binary<C, E, F>::operator()(const std::size_t i) const
    {
        return _e(i) * derivative_functor_type::apply1(_c, _e);
    }

    template<typename C, typename E, typename F>
    inline const typename scalar_variable_binary<C, E, F>::index_map_type&
    scalar_variable_binary<C, E, F>::index_mapper() const
    {
        return _e.index_mapper();
    }
}}

#define DEFINE_SPECIFIC_SCALAR_VARIABLE_BINARY(OPERATOR, NAME)                 \
namespace ad { namespace dual {                                                  \
    template <typename C, typename E> inline                                   \
    std::enable_if_t<                                                          \
        !is_variable_expression<C>::value,                                     \
        scalar_variable_binary<                                                \
            C,                                                                 \
            E,                                                                 \
            ad::functor::NAME##_functor<C, typename E::value_type>>>                \
    operator OPERATOR(const C& c, const variable_expression<E>& e)             \
    {                                                                          \
        using functor_type                                                     \
            = ad::functor::NAME##_functor<C, typename E::value_type>;               \
        return scalar_variable_binary<C, E, functor_type>(c, e());             \
    }                                                                          \
}}

DEFINE_SPECIFIC_SCALAR_VARIABLE_BINARY(+, plus);
DEFINE_SPECIFIC_SCALAR_VARIABLE_BINARY(-, minus);
DEFINE_SPECIFIC_SCALAR_VARIABLE_BINARY(*, multiply);
DEFINE_SPECIFIC_SCALAR_VARIABLE_BINARY(/, divide);

#undef DEFINE_SPECIFIC_SCALAR_VARIABLE_BINARY
