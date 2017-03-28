#pragma once

#include "ad/bu/variable.h"
#include "ad/bu/variable_expression.h"

namespace ad { namespace bu {
    template <typename E>
    class derivative {
    public:
        using variable_expression_type = typename E::expression_type;
        using value_type = typename variable_expression_type::value_type;

    public:
        derivative(const E& var);

    public:
        value_type d(const value_type& x) const;

    private:
        variable_expression_type _var;
    };

    template<typename E>
    inline derivative<E>::derivative(const E& var)
        : _var(var)
    {
    }

    template<typename E>
    inline typename derivative<E>::value_type 
    derivative<E>::d(const value_type& x) const
    {
        const auto it
            = _var.index_mapper().find(std::addressof(x));
        return it == std::cend(_var.index_mapper())
            ? value_type(0)
            : _var(it->second);
    }

    template<typename E>
    derivative<E> d(const variable_expression<E>& v)
    {
        return derivative<E>(v());
    }
} }