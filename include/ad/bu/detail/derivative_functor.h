#pragma once

#include <cmath>
#include <type_traits>

#include "ad/bu/detail/unary_functor.h"
#include "ad/bu/detail/binary_functor.h"

namespace ad { namespace bu { namespace detail {
    template <typename F>
    struct derivative_functor;

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
            return - std::sin(x);
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
    struct derivative_functor<exp_functor<T>> {
        using result_type = decltype(std::exp(std::declval<T>()));
        static result_type apply(const T& x)
        {
            return std::exp(x);
        }
    };

    template <typename T0, typename T1>
    struct derivative_functor<plus_functor<T0, T1>>
    {
        using result0_type = T0;
        using result1_type = T1;

        static result0_type apply0(const T0& /*x0*/, const T1& /*x1*/)
        {
            return result0_type(1);
        }

        static result1_type apply1(const T0& /*x0*/, const T1& /*x1*/)
        {
            return result1_type(1);
        }
    };

    template <typename T0, typename T1>
    struct derivative_functor<minus_functor<T0, T1>>
    {
        using result0_type = T0;
        using result1_type = T1;

        static result0_type apply0(const T0& /*x0*/, const T1& /*x1*/)
        {
            return result0_type(1);
        }

        static result1_type apply1(const T0& /*x0*/, const T1& /*x1*/)
        {
            return result1_type(-1);
        }
    };

    template <typename T0, typename T1>
    struct derivative_functor<multiply_functor<T0, T1>>
    {
        using result0_type = T1;
        using result1_type = T0;

        static result0_type apply0(const T0& x0, const T1& x1)
        {
            return result0_type(x1);
        }

        static result1_type apply1(const T0& x0, const T1& x1)
        {
            return result1_type(x0);
        }
    };

    template <typename T0, typename T1>
    struct derivative_functor<divide_functor<T0, T1>>
    {
        using result0_type
            = decltype(std::declval<T0>() / std::declval<T1>());
        using result1_type 
            = decltype(- std::declval<T0>() / (std::declval<T1>() * std::declval<T1>()));

        static result0_type apply0(const T0& x0, const T1& x1)
        {
            return T0(1) / x1;
        }

        static result1_type apply1(const T0& x0, const T1& x1)
        {
            return x0 / (x1 * x1);
        }
    };
}}}