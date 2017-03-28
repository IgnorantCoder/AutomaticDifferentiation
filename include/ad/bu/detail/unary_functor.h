#pragma once

#include <cmath>
#include <type_traits>

namespace ad { namespace bu { namespace detail {
    template <typename T>
    struct negate_functor {
        using result_type = decltype(-std::declval<T>());
        static result_type apply(const T& x)
        {
            return -x;
        }
    };

    template <typename T>
    struct sin_functor {
        using result_type = decltype(std::sin(std::declval<T>()));
        static result_type apply(const T& x)
        {
            return std::sin(x);
        }
    };

    template <typename T>
    struct cos_functor {
        using result_type = decltype(std::cos(std::declval<T>()));
        static result_type apply(const T& x)
        {
            return std::cos(x);
        }
    };

    template <typename T>
    struct tan_functor {
        using result_type = decltype(std::tan(std::declval<T>()));
        static result_type apply(const T& x)
        {
            return std::tan(x);
        }
    };

    template <typename T>
    struct log_functor {
        using result_type = decltype(std::log(std::declval<T>()));
        static result_type apply(const T& x)
        {
            return std::log(x);
        }
    };

    template <typename T>
    struct exp_functor {
        using result_type = decltype(std::exp(std::declval<T>()));
        static result_type apply(const T& x)
        {
            return std::exp(x);
        }
    };
} } }