#pragma once

#include <vector>

#include "ad/td/variable.h"

namespace ad { namespace td {
    template <typename V, typename D>
    class gradient_holder {
    public:
        using derivative_value_type = D;
        using gradient_data = std::vector<derivative_value_type>;

    public:
        gradient_holder(const variable<V, D>& v, gradient_data&& data);
        gradient_holder(const variable<V, D>& v, const gradient_data& data);

    public:
        derivative_value_type in_direction_of(const variable<V, D>& x) const;
        derivative_value_type in_direction_of(const V& x) const;

    private:
        const variable<V, D>& _v;
        gradient_data _data;
    };

    template<typename V, typename D>
    inline gradient_holder<V, D>::gradient_holder(
        const variable<V, D>& v,
        gradient_data && data)
        :_v(v),
        _data(std::move(data))
    {
    }

    template<typename V, typename D>
    inline gradient_holder<V, D>::gradient_holder(
        const variable<V, D>& v,
        const gradient_data & data)
        :_v(v),
        _data(data)
    {
    }

    template<typename V, typename D>
    inline typename gradient_holder<V, D>::derivative_value_type 
    gradient_holder<V, D>::in_direction_of(const variable<V, D>& x) const
    {
        return _data[x.get_id()];
    }

    template<typename V, typename D>
    inline typename gradient_holder<V, D>::derivative_value_type
    gradient_holder<V, D>::in_direction_of(const V & x) const
    {
        typename variable<V, D>::index_type i = 0;
        if (!_v.try_get_index(i, std::addressof(x))) {
            return derivative_value_type(0);
        }
        return _data[i];
    }

    template <typename V, typename D>
    gradient_holder<V, D> gradient(const variable<V, D>& y)
    {
        return gradient_holder<V, D>(y, y.get_tape().sweep(y.get_id()));
    }
} }