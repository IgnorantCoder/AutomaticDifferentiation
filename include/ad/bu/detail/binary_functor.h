#pragma once

#include <type_traits>

namespace ad { namespace bu { namespace detail {
    template <typename T0, typename T1>
    struct plus_functor {
        using result_type
            = decltype(std::declval<T0>() + std::declval<T1>());
        static result_type apply(const T0& x0, const T1& x1)
        {
            return x0 + x1;
        }
    };

    template <typename T0, typename T1>
    struct minus_functor {
        using result_type
            = decltype(std::declval<T0>() - std::declval<T1>());
        static result_type apply(const T0& x0, const T1& x1)
        {
            return x0 - x1;
        }
    };

    template <typename T0, typename T1>
    struct multiply_functor {
        using result_type
            = decltype(std::declval<T0>() * std::declval<T1>());
        static result_type apply(const T0& x0, const T1& x1)
        {
            return x0 * x1;
        }
    };

    template <typename T0, typename T1>
    struct divide_functor {
        using result_type
            = decltype(std::declval<T0>() / std::declval<T1>());
        static result_type apply(const T0& x0, const T1& x1)
        {
            return x0 / x1;
        }
    };
} } }