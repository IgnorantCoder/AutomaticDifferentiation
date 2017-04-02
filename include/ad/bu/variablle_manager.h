#pragma once

#include <map>

#include "ad/type_traits/head.h"
#include "ad/bu/variable.h"
#include "ad/bu/detail/create_mapping.h"

namespace ad { namespace bu {
    template <typename V>
    class variable_manager {
    public:
        using value_type = V;

    public:
        variable_manager(
            std::map<value_type const*, std::size_t>&& index_mapper);

    public:
        variable<V> get_variable(const value_type& x) const;

    private:
        std::map<value_type const*, std::size_t> _index_mapper;
    };

    template<typename V>
    inline variable_manager<V>::variable_manager(
        std::map<value_type const*, std::size_t>&& index_mapper)
        : _index_mapper(std::move(index_mapper))
    {
    }

    template<typename V>
    inline variable<V> variable_manager<V>::get_variable(
        const value_type& x) const
    {
        std::vector<value_type> dx(_index_mapper.size(), 0);
        const auto it = _index_mapper.find(std::addressof(x));
        if (it != _index_mapper.cend()) {
            dx[it->second] = 1.0;
        }

        return variable<value_type>(x, dx, _index_mapper);
    }

    /**
    @brief dispatcher of variable_manager
    */
    template <typename ... T>
    variable_manager<typename type_traits::head<T...>::type>
    create_variable_manager(const T& ...t)
    {
        auto&& index_mapper = detail::create_mapping(t...);
        return variable_manager<
            typename type_traits::head<T...>::type
        >(std::move(index_mapper));
    }

    /**
    @brief dispatcher of variable_manager
    */
    template<typename V>
    variable_manager<V>
    create_variable_manager(const std::vector<V>& data)
    {
        std::map<V const*, std::size_t>&& index_mapper 
            = detail::create_mapping(data);
        return variable_manager<V>(std::move(index_mapper));
    }

    /**
    @brief dispatcher of variable_manager
    */
    template<typename V, std::size_t N>
    variable_manager<V>
    create_variable_manager(const std::array<V, N>& data)
    {
        std::map<V const*, std::size_t>&& index_mapper
            = detail::create_mapping(data);
        return variable_manager<V>(std::move(index_mapper));
    }

} }