#pragma once

#include <cassert>
#include <type_traits>
#include <utility>

#include "ad/bu/variable_expression.h"
#include "ad/bu/detail/binary_functor.h"
#include "ad/bu/detail/derivative_functor.h"

namespace ad { namespace bu { 
    template <typename E0, typename E1, typename F>
    class variable_binary
        : public variable_expression<variable_binary<E0, E1, F>> {
    private:
        using functor_type = F;
        using derivative_functor_type
            = detail::derivative_functor<functor_type>;

    public:
        using value_type = typename functor_type::result_type;
        using derivative_value_type
            = decltype(
                std::declval<typename derivative_functor_type::result0_type>()
                * std::declval<typename E0::derivative_value_type>()
                + std::declval<typename derivative_functor_type::result1_type>()
                * std::declval<typename E1::derivative_value_type>());
        using expression_type = variable_binary<E0, E1, F>;
        using index_map_type = typename E0::index_map_type;

    public:
        variable_binary(const E0& e0, const E1& e1);

    public:
        operator value_type() const;
        derivative_value_type operator()(const std::size_t i) const;
        const index_map_type& index_mapper() const;

    private:
        typename E0::expression_type _e0;
        typename E1::expression_type _e1;
    };

    template<typename E0, typename E1, typename F>
    inline variable_binary<E0, E1, F>::variable_binary(
        const E0 & e0,
        const E1 & e1)
        : _e0(e0), 
        _e1(e1)
    {
        assert(std::addressof(e0.index_mapper()) == std::addressof(e1.index_mapper()));
    }

    template<typename E0, typename E1, typename F>
    inline variable_binary<E0, E1, F>::operator typename variable_binary<E0, E1, F>::value_type() const
    {
        return functor_type::apply(_e0, _e1);
    }

    template<typename E0, typename E1, typename F>
    inline typename variable_binary<E0, E1, F>::derivative_value_type
    variable_binary<E0, E1, F>::operator()(const std::size_t i) const
    {
        return _e0(i) * derivative_functor_type::apply0(_e0, _e1)
            + _e1(i) * derivative_functor_type::apply1(_e0, _e1);
    }

    template<typename E0, typename E1, typename F>
    inline const typename variable_binary<E0, E1, F>::index_map_type&
    variable_binary<E0, E1, F>::index_mapper() const
    {
        return _e0.index_mapper();
    }

    /**
    @details \f$x0 + x1\f$
    */
    template <typename E0, typename E1>
    inline variable_binary<
        E0, 
        E1, 
        detail::plus_functor<
            typename E0::value_type,
            typename E1::value_type>>
    operator +(
        const variable_expression<E0>& e0,
        const variable_expression<E1>& e1)
    {
        using functor_type
            = detail::plus_functor<typename E0::value_type, typename E1::value_type>;
        return variable_binary<E0, E1, functor_type>(e0(), e1());
    }

    /**
    @details \f$x0 - x1\f$
    */
    template <typename E0, typename E1>
    inline variable_binary<
        E0,
        E1,
        detail::minus_functor<
            typename E0::value_type,
            typename E1::value_type>>
    operator -(
        const variable_expression<E0>& e0,
        const variable_expression<E1>& e1)
    {
        using functor_type
            = detail::minus_functor<typename E0::value_type, typename E1::value_type>;
        return variable_binary<E0, E1, functor_type>(e0(), e1());
    }

    /**
    @details \f$x0 * x1\f$
    */
    template <typename E0, typename E1>
    inline variable_binary<
        E0,
        E1,
        detail::multiply_functor<
            typename E0::value_type,
            typename E1::value_type>>
    operator *(
        const variable_expression<E0>& e0,
        const variable_expression<E1>& e1)
    {
        using functor_type
            = detail::multiply_functor<typename E0::value_type, typename E1::value_type>;
        return variable_binary<E0, E1, functor_type>(e0(), e1());
    }

    /**
    @details \f$x0 / x1\f$
    */
    template <typename E0, typename E1>
    inline variable_binary<
        E0,
        E1,
        detail::divide_functor<
            typename E0::value_type,
            typename E1::value_type>>
    operator /(
        const variable_expression<E0>& e0,
        const variable_expression<E1>& e1)
    {
        using functor_type
            = detail::divide_functor<typename E0::value_type, typename E1::value_type>;
        return variable_binary<E0, E1, functor_type>(e0(), e1());
    }

    template <typename C, typename E, typename F>
    class scalar_variable_binary
        : public variable_expression<scalar_variable_binary<C, E, F>> {
    private:
        using functor_type = F;
        using derivative_functor_type
            = detail::derivative_functor<functor_type>;

    public:
        using scalar_value_type = C;
        using value_type = typename functor_type::result_type;
        using derivative_value_type = typename E::derivative_value_type;
        using expression_type = scalar_variable_binary<C, E, F>;
        using index_map_type = typename E::index_map_type;

    public:
        scalar_variable_binary(const C& c, const E& e);

    public:
        operator value_type() const;
        derivative_value_type operator()(const std::size_t i) const;
        const index_map_type& index_mapper() const;

    private:
        scalar_value_type _c;
        typename E::expression_type _e;
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

    /**
    @details \f$c + x\f$
    */
    template <typename C, typename E> inline
    std::enable_if_t<
        !is_variable_expression<C>::value,
        scalar_variable_binary<
            C,
            E,
            detail::plus_functor<C, typename E::value_type>>>
    operator +(const C& c, const variable_expression<E>& e)
    {
        using functor_type
            = detail::plus_functor<C, typename E::value_type>;
        return scalar_variable_binary<C, E, functor_type>(c, e());
    }

    /**
    @details \f$c - x\f$
    */
    template <typename C, typename E> inline 
    std::enable_if_t<
        !is_variable_expression<C>::value,
        scalar_variable_binary<
            C,
            E,
            detail::minus_functor<C, typename E::value_type>>>
    operator -(const C& c, const variable_expression<E>& e)
    {
        using functor_type
            = detail::minus_functor<C, typename E::value_type>;
        return scalar_variable_binary<C, E, functor_type>(c, e());
    }

    /**
    @details \f$c * x\f$
    */
    template <typename C, typename E> inline
    std::enable_if_t<
        !is_variable_expression<C>::value,
        scalar_variable_binary<
            C,
            E,
            detail::multiply_functor<C, typename E::value_type>>>
    operator *(const C& c, const variable_expression<E>& e)
    {
        using functor_type
            = detail::multiply_functor<C, typename E::value_type>;
        return scalar_variable_binary<C, E, functor_type>(c, e());
    }

    /**
    @details \f$c / x\f$
    */
    template <typename C, typename E> inline 
    std::enable_if_t<
        !is_variable_expression<C>::value,
        scalar_variable_binary<
            C,
            E,
            detail::divide_functor<C, typename E::value_type>>>
    operator /(const C& c, const variable_expression<E>& e)
    {
        using functor_type
            = detail::divide_functor<C, typename E::value_type>;
        return scalar_variable_binary<C, E, functor_type>(c, e());
    }

    template <typename C, typename E, typename F>
    class variable_scalar_binary
        : public variable_expression<variable_scalar_binary<C, E, F>> {
    private:
        using functor_type = F;
        using derivative_functor_type
            = detail::derivative_functor<functor_type>;

    public:
        using scalar_value_type = C;
        using value_type = typename functor_type::result_type;
        using derivative_value_type = typename E::derivative_value_type;
        using expression_type = variable_scalar_binary<C, E, F>;
        using index_map_type = typename E::index_map_type;

    public:
        variable_scalar_binary(const C& c, const E& e);

    public:
        operator value_type() const;
        derivative_value_type operator()(const std::size_t i) const;
        const index_map_type& index_mapper() const;

    private:
        scalar_value_type _c;
        typename E::expression_type _e;
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

    /**
    @details \f$x + c\f$
    */
    template <typename C, typename E> inline 
    std::enable_if_t<
        !is_variable_expression<C>::value,
        variable_scalar_binary<
            C,
            E,
            detail::plus_functor<typename E::value_type, C>>>
    operator +(const variable_expression<E>& e, const C& c)
    {
        using functor_type
            = detail::plus_functor<typename E::value_type, C>;
        return variable_scalar_binary<C, E, functor_type>(c, e());
    }

    /**
    @details \f$x - c\f$
    */
    template <typename C, typename E> inline 
    std::enable_if_t<
        !is_variable_expression<C>::value,
        variable_scalar_binary<
            C,
            E,
            detail::minus_functor<typename E::value_type, C>>>
    operator -(const variable_expression<E>& e, const C& c)
    {
        using functor_type
            = detail::minus_functor<typename E::value_type, C>;
        return variable_scalar_binary<C, E, functor_type>(c, e());
    }

    /**
    @details \f$x * c\f$
    */
    template <typename C, typename E> inline 
    std::enable_if_t<
        !is_variable_expression<C>::value,
        variable_scalar_binary<
            C,
            E,
            detail::multiply_functor<typename E::value_type, C>>>
    operator *(const variable_expression<E>& e, const C& c)
    {
        using functor_type
            = detail::multiply_functor<typename E::value_type, C>;
        return variable_scalar_binary<C, E, functor_type>(c, e());
    }

    /**
    @details \f$x / c\f$
    */
    template <typename C, typename E> inline 
    std::enable_if_t<
        !is_variable_expression<C>::value,
        variable_scalar_binary<
            C,
            E,
            detail::divide_functor<C, typename E::value_type>>>
    operator /(const variable_expression<E>& e, const C& c)
    {
        using functor_type
            = detail::divide_functor<typename E::value_type, C>;
        return variable_scalar_binary<C, E, functor_type>(c, e());
    }
} }