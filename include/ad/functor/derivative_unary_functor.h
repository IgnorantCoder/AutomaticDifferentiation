#pragma once

#include <cmath>
#include <type_traits>

#include "ad/functor/unary_functor.h"
#include "ad/functor/derivative_functor.h"

namespace ad { namespace functor {
    template <typename T>
    struct derivative_functor<negate_functor<T>> {
        using result_type = T;
        static result_type apply(const T& /*x*/)
        {
            return T(-1);
        }
    };

    template <typename T>
    struct derivative_functor<sin_functor<T>> {
        using result_type = decltype(std::cos(std::declval<T>()));
        static result_type apply(const T& x)
        {
            return std::cos(x);
        }
    };

    template <typename T>
    struct derivative_functor<cos_functor<T>> {
        using result_type = decltype(-std::sin(std::declval<T>()));
        static result_type apply(const T& x)
        {
            return -std::sin(x);
        }
    };

    template <typename T>
    struct derivative_functor<tan_functor<T>> {
        using result_type = decltype(
            std::declval<T>() / (std::cos(std::declval<T>()) * std::cos(std::declval<T>())));
        static result_type apply(const T& x)
        {
            return T(1) / (std::cos(x) * std::cos(x));
        }
    };

    template <typename T>
    struct derivative_functor<log_functor<T>> {
        using result_type = decltype(std::declval<T>() / std::declval<T>());
        static result_type apply(const T& x)
        {
            return T(1) / x;
        }
    };

    template <typename T>
    struct derivative_functor<erf_functor<T>> {
        using result_type = T;
        static result_type apply(const T& x)
        {
            static const auto pi = std::atan(1.0) * 4.0;
            static const auto c = 2.0 / std::sqrt(pi);
            const auto ret = c * std::exp(-(x * x));

            return ret;
        }
    };

    template <typename T>
    struct derivative_functor<sqrt_functor<T>> {
        using result_type = T;
        static result_type apply(const T& x)
        {
            return T(0.5) / std::sqrt(x);
        }
    };

    template <typename T>
    struct derivative_functor<exp_functor<T>> {
        using result_type = decltype(std::exp(std::declval<T>()));
        static result_type apply(const T& x)
        {
            return std::exp(x);
        }
    };
}}