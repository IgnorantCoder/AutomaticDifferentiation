#pragma once

namespace ad { namespace bu {
    template <typename E>
    class variable_expression {
    public:
        using expression_type = E;

    public:
        expression_type& operator()();
        const expression_type& operator()() const;
    };

    template <typename E> 
    inline typename variable_expression<E>::expression_type& 
    variable_expression<E>::operator()()
    {
        return static_cast<expression_type&>(*this);
    }

    template <typename E>
    inline const typename variable_expression<E>::expression_type&
    variable_expression<E>::operator()() const
    {
        return static_cast<const expression_type&>(*this);
    }

    template <typename E>
    struct is_variable_expression 
        : public std::is_base_of<variable_expression<E>, E>
    {};

    template <typename E>
    struct is_variable_expression<variable_expression<E>> 
        : public std::true_type
    {};

} }