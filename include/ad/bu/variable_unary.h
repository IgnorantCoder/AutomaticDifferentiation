#pragma once

#include <type_traits>

#include "ad/bu/variable_expression.h"
#include "ad/bu/detail/unary_functor.h"
#include "ad/bu/detail/derivative_functor.h"

namespace ad { namespace bu {
    template <typename E, typename F>
    class variable_unary
        : public variable_expression<variable_unary<E, F>>{
    private:
        using functor_type = F;
        using derivative_functor_type
            = detail::derivative_functor<functor_type>;

    public:
        using value_type = typename functor_type::result_type;
        using derivative_value_type
            = decltype(
                std::declval<value_type>()
                * std::declval<typename derivative_functor_type::result_type>());
        using expression_type = variable_unary<E, F>;
        using index_map_type = typename E::index_map_type;

    public:
        variable_unary(const E& e);

    public:
        operator value_type() const;
        derivative_value_type operator()(const std::size_t i) const;
        const index_map_type& index_mapper() const;

    private:
        typename E::expression_type _e;
    };

    template<typename E, typename F>
    inline variable_unary<E, F>::variable_unary(const E & e)
        : _e(e)
    {
    }

    template<typename E, typename F>
    inline variable_unary<E, F>::operator typename variable_unary<E, F>::value_type() const
    {
        return functor_type::apply(_e);
    }

    template<typename E, typename F>
    inline typename variable_unary<E, F>::derivative_value_type
    variable_unary<E, F>::operator()(const std::size_t i) const
    {
        return _e(i) * derivative_functor_type::apply(_e);
    }

    template<typename E, typename F>
    inline const typename variable_unary<E, F>::index_map_type&
    variable_unary<E, F>::index_mapper() const
    {
        return _e.index_mapper();
    }

    /**
    @details \f$-x\f$
    */
    template <typename E>
    inline variable_unary<E, detail::negate_functor<typename E::value_type>>
    operator -(const variable_expression<E>& e)
    {
        using functor_type 
            = detail::negate_functor<typename E::value_type>;
        return variable_unary<E, functor_type>(e());
    }

    /**
    @details \f$sin(x)\f$
    */
    template <typename E>
    inline variable_unary<E, detail::sin_functor<typename E::value_type>>
    sin(const variable_expression<E>& e)
    {
        using functor_type
            = detail::sin_functor<typename E::value_type>;
        return variable_unary<E, functor_type>(e());
    }

    /**
    @details \f$cos(x)\f$
    */
    template <typename E>
    inline variable_unary<E, detail::cos_functor<typename E::value_type>>
    cos(const variable_expression<E>& e)
    {
        using functor_type
            = detail::cos_functor<typename E::value_type>;
        return variable_unary<E, functor_type>(e());
    }
}}

namespace std {
    using ad::bu::sin;
    using ad::bu::cos;
}