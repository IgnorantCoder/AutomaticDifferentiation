#pragma once

#include <algorithm>
#include <array>
#include <map>
#include <memory>
#include <vector>
#include <functional>

#include "ad/type_traits/head.h"

namespace ad { namespace bu { namespace detail {
    template <typename T>
    inline std::map<T const*, std::size_t>
    create_mapping(const std::vector<T const*>& d)
    {
        std::map<T const*, std::size_t> index;
        for (std::size_t i = 0; i < d.size(); ++i) {
            index.emplace(d[i], i);
        }
        return index;
    }

    template <typename T>
    inline std::map<T const*, std::size_t>
    create_mapping(const std::initializer_list<T const*>& d)
    {
        return create_mapping(std::vector<T const*>(d));
    }

    template <typename ... T>
    inline std::map<typename type_traits::head<T...>::type const*, std::size_t>
    create_mapping(const T& ...t)
    {
        const auto d = { std::addressof(t)... };
        return create_mapping(d);
    }

    template <typename T>
    inline std::map<T const*, std::size_t>
    create_mapping(const std::vector<T>& v)
    {
        std::vector<T const*> d;
        d.reserve(v.size());
        for (const auto& e : v)
        {
            d.emplace_back(std::addressof(e));
        }
        return create_mapping(d);
    }

    template <typename T, std::size_t N>
    inline std::map<T const*, std::size_t>
        create_mapping(const std::array<T, N>& v)
    {
        std::vector<T const*> d;
        d.reserve(v.size());
        for (const auto& e : v)
        {
            d.emplace_back(std::addressof(e));
        }
        return create_mapping(d);
    }
}}}