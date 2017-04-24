#pragma once

#include <cassert>
#include <type_traits>

#include "ad/bu/variable_expression.h"
#include "ad/functor/binary_functor.h"
#include "ad/functor/derivative_functor.h"

namespace ad { namespace bu { 
    template <typename E0, typename E1, typename F>
    class variable_variable_binary
        : public variable_expression<variable_variable_binary<E0, E1, F>> {
    private:
        using functor_type = F;
        using derivative_functor_type
            = ad::functor::derivative_functor<functor_type>;

    public:
        using value_type = typename functor_type::result_type;
        using derivative_value_type
            = decltype(
                std::declval<typename derivative_functor_type::result0_type>()
                * std::declval<typename E0::derivative_value_type>()
                + std::declval<typename derivative_functor_type::result1_type>()
                * std::declval<typename E1::derivative_value_type>());
        using closure_type = variable_variable_binary<E0, E1, F>;
        using index_map_type = typename E0::index_map_type;

    public:
        variable_variable_binary(const E0& e0, const E1& e1);

    public:
        operator value_type() const;
        derivative_value_type operator()(const std::size_t i) const;
        const index_map_type& index_mapper() const;

    private:
        typename E0::closure_type _e0;
        typename E1::closure_type _e1;
    };

    template<typename E0, typename E1, typename F>
    inline variable_variable_binary<E0, E1, F>::variable_variable_binary(
        const E0 & e0,
        const E1 & e1)
        : _e0(e0), 
        _e1(e1)
    {
        assert(std::addressof(e0.index_mapper()) == std::addressof(e1.index_mapper()));
    }

    template<typename E0, typename E1, typename F>
    inline variable_variable_binary<E0, E1, F>::operator typename variable_variable_binary<E0, E1, F>::value_type() const
    {
        return functor_type::apply(_e0, _e1);
    }

    template<typename E0, typename E1, typename F>
    inline typename variable_variable_binary<E0, E1, F>::derivative_value_type
    variable_variable_binary<E0, E1, F>::operator()(const std::size_t i) const
    {
        return _e0(i) * derivative_functor_type::apply0(_e0, _e1)
            + _e1(i) * derivative_functor_type::apply1(_e0, _e1);
    }

    template<typename E0, typename E1, typename F>
    inline const typename variable_variable_binary<E0, E1, F>::index_map_type&
    variable_variable_binary<E0, E1, F>::index_mapper() const
    {
        return _e0.index_mapper();
    }
}}

#define DEFINE_SPECIFIC_VARIABLE_VARIABLE_BINARY(OPERATOR, NAME)               \
namespace ad { namespace bu {                                                  \
    template <typename E0, typename E1>                                        \
    inline variable_variable_binary<                                           \
        E0,                                                                    \
        E1,                                                                    \
        ad::functor::NAME##_functor<                                           \
            typename E0::value_type,                                           \
            typename E1::value_type>>                                          \
    operator OPERATOR(                                                         \
        const variable_expression<E0>& e0,                                     \
        const variable_expression<E1>& e1)                                     \
    {                                                                          \
        using functor_type                                                     \
            = ad::functor::NAME##_functor<                                     \
                typename E0::value_type,                                       \
                typename E1::value_type>;                                      \
        return variable_variable_binary<E0, E1, functor_type>(e0(), e1());     \
    }                                                                          \
}}

DEFINE_SPECIFIC_VARIABLE_VARIABLE_BINARY(+, plus);
DEFINE_SPECIFIC_VARIABLE_VARIABLE_BINARY(-, minus);
DEFINE_SPECIFIC_VARIABLE_VARIABLE_BINARY(*, multiply);
DEFINE_SPECIFIC_VARIABLE_VARIABLE_BINARY(/, divide);

#undef DEFINE_SPECIFIC_VARIABLE_VARIABLE_BINARY
