#pragma once

#include <vector>
#include <map>

#include "ad/bu/variable_expression.h"

namespace ad { namespace bu {
    template <typename V>
    class variable : public variable_expression<variable<V>> {
    public:
        using value_type = V;
        using derivative_value_type = value_type;
        using expression_type = const variable<value_type>&;
        using index_map_type = std::map<value_type const*, std::size_t>;

    public:
        variable(
            value_type&& f,
            std::vector<value_type>&& df,
            const index_map_type& index_mapper);
        variable(
            const value_type& f,
            const std::vector<value_type>& df,
            const index_map_type& index_mapper);

    public:
        operator value_type() const;
        derivative_value_type operator()(const std::size_t i) const;
        const index_map_type& index_mapper() const;

    private:
        value_type _f;
        std::vector<value_type> _df;
        const index_map_type& _index_mapper;
    };

    template<typename V>
    inline variable<V>::variable(
        value_type&& f,
        std::vector<value_type>&& df,
        const index_map_type& index_mapper)
        : _f(std::move(f)),
        _df(std::move(df)),
        _index_mapper(index_mapper)
    {
    }

    template<typename V>
    inline variable<V>::variable(
        const value_type & f,
        const std::vector<value_type>& df,
        const index_map_type& index_mapper)
        : _f(f),
        _df(df),
        _index_mapper(index_mapper)
    {
    }

    template<typename V>
    inline variable<V>::operator typename variable<V>::value_type() const
    {
        return _f;
    }
    
    template<typename V>
    inline typename variable<V>::derivative_value_type
    variable<V>::operator()(const std::size_t i) const
    {
        return _df[i];
    }

    template<typename V>
    inline const typename variable<V>::index_map_type&
    variable<V>::index_mapper() const
    {
        return _index_mapper;
    }
}}