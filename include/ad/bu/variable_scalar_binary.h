#pragma once

#include <type_traits>

#include "ad/bu/variable_expression.h"
#include "ad/functor/binary_functor.h"
#include "ad/functor/derivative_functor.h"

namespace ad { namespace bu {
    template <typename C, typename E, typename F>
    class variable_scalar_binary
        : public variable_expression<variable_scalar_binary<C, E, F>> {
    private:
        using functor_type = F;
        using derivative_functor_type
            = ad::functor::derivative_functor<functor_type>;

    public:
        using scalar_value_type = C;
        using value_type = typename functor_type::result_type;
        using derivative_value_type = typename E::derivative_value_type;
        using closure_type = variable_scalar_binary<C, E, F>;
        using index_map_type = typename E::index_map_type;

    public:
        variable_scalar_binary(const C& c, const E& e);

    public:
        operator value_type() const;
        derivative_value_type operator()(const std::size_t i) const;
        const index_map_type& index_mapper() const;

    private:
        scalar_value_type _c;
        typename E::closure_type _e;
    };

    template<typename C, typename E, typename F>
    inline variable_scalar_binary<C, E, F>::variable_scalar_binary(
        const C & c,
        const E & e)
        : _c(c), _e(e)
    {
    }

    template<typename C, typename E, typename F>
    inline variable_scalar_binary<C, E, F>::operator
    typename variable_scalar_binary<C, E, F>::value_type() const
    {
        return functor_type::apply(_e, _c);
    }

    template<typename C, typename E, typename F>
    inline typename variable_scalar_binary<C, E, F>::derivative_value_type
    variable_scalar_binary<C, E, F>::operator()(const std::size_t i) const
    {
        return _e(i) * derivative_functor_type::apply0(_e, _c);
    }

    template<typename C, typename E, typename F>
    inline const typename variable_scalar_binary<C, E, F>::index_map_type&
    variable_scalar_binary<C, E, F>::index_mapper() const
    {
        return _e.index_mapper();
    }
}}

#define DEFINE_SPECIFIC_VARIABLE_SCALAR_BINARY(OPERATOR, NAME)                 \
namespace ad { namespace bu {                                                  \
    template <typename C, typename E> inline                                   \
    std::enable_if_t<                                                          \
        !is_variable_expression<C>::value,                                     \
        variable_scalar_binary<                                                \
            C,                                                                 \
            E,                                                                 \
        ad::functor::NAME##_functor<typename E::value_type, C>>>               \
    operator OPERATOR(const variable_expression<E>& e, const C& c)             \
    {                                                                          \
        using functor_type                                                     \
            = ad::functor::NAME##_functor<typename E::value_type, C>;          \
        return variable_scalar_binary<C, E, functor_type>(c, e());             \
    }                                                                          \
}}

DEFINE_SPECIFIC_VARIABLE_SCALAR_BINARY(+, plus);
DEFINE_SPECIFIC_VARIABLE_SCALAR_BINARY(-, minus);
DEFINE_SPECIFIC_VARIABLE_SCALAR_BINARY(*, multiply);
DEFINE_SPECIFIC_VARIABLE_SCALAR_BINARY(/, divide);

#undef DEFINE_SPECIFIC_VARIABLE_SCALAR_BINARY
