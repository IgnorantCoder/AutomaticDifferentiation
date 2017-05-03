#pragma once

#include "ad/rt/bu/variable.h"

namespace ad { namespace rt { namespace bu {
    template <typename V, typename D>
    class derivative {
    public:
        using derivative_value_type = D;

    public:
        derivative(const variable<V, D>& v);

    public:
        derivative_value_type d(const variable<V, D>& x) const;
        derivative_value_type d(const V& x) const;

    private:
        const variable<V, D>& _v;
    };

    template<typename V, typename D>
    inline derivative<V, D>::derivative(const variable<V, D>& v)
        : _v(v)
    {
    }

    template<typename V, typename D>
    inline typename derivative<V, D>::derivative_value_type
    derivative<V, D>::d(const variable<V, D>& x) const
    {
        return _v.get_tape().sweep(x.get_id(), _v.get_id());
    }

    template<typename V, typename D>
    inline typename derivative<V, D>::derivative_value_type
    derivative<V, D>::d(const V & x) const
    {
        typename variable<V, D>::index_type i = 0;
        if (!_v.try_get_index(i, std::addressof(x))) {
            return derivative_value_type(0);
        }
        return _v.get_tape().sweep(i, _v.get_id());;
    }

    template <typename V, typename D>
    derivative<V, D> d(const variable<V, D>& y)
    {
        return derivative<V, D>(y);
    }
} } }