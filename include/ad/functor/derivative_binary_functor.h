#pragma once

#include <cmath>
#include <type_traits>

#include "ad/functor/binary_functor.h"
#include "ad/functor/derivative_functor.h"

namespace ad { namespace functor {
    template <typename T0, typename T1>
    struct derivative_functor<plus_functor<T0, T1>> {
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
    struct derivative_functor<minus_functor<T0, T1>> {
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
    struct derivative_functor<multiply_functor<T0, T1>> {
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
    struct derivative_functor<divide_functor<T0, T1>> {
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
            return - x0 / (x1 * x1);
        }
    };
}}