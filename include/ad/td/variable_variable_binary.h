#pragma once

#include "ad/td/variable.h"

namespace ad { namespace td {
    template <typename V, typename D>
    variable<V, D> operator +(
        const variable<V, D>& e0,
        const variable<V, D>& e1)
    {
        const auto x0 = static_cast<V>(e0);
        const auto x1 = static_cast<V>(e1);
        const auto y = e0.generate_and_link(x0 + x1, D(1));
        e1.link(y, D(1));

        return y;
    }

    template <typename V, typename D>
    variable<V, D> operator -(
        const variable<V, D>& e0,
        const variable<V, D>& e1)
    {
        const auto x0 = static_cast<V>(e0);
        const auto x1 = static_cast<V>(e1);
        const auto y = e0.generate_and_link(x0 - x1, D(1));
        e1.link(y, D(-1));

        return y;
    }

    template <typename V, typename D>
    variable<V, D> operator *(
        const variable<V, D>& e0,
        const variable<V, D>& e1)
    {
        const auto x0 = static_cast<V>(e0);
        const auto x1 = static_cast<V>(e1);
        const auto y = e0.generate_and_link(x0 * x1, D(x1));
        e1.link(y, D(x0));

        return y;
    }

    template <typename V, typename D>
    variable<V, D> operator /(
        const variable<V, D>& e0,
        const variable<V, D>& e1)
    {
        const auto x0 = static_cast<V>(e0);
        const auto x1 = static_cast<V>(e1);
        const auto y = e0.generate_and_link(x0 / x1, D(1) / x1);
        e1.link(y, - x0 / (x1 * x1));

        return y;
    }
}}