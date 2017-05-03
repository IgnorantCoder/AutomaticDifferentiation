#pragma once

#include <map>
#include <memory>

#include "ad/rt/tape/variable.h"

namespace ad { namespace rt { namespace tape {
    template <typename V, typename D, template <typename> typename G>
    class variable_manager {
    public:
        using value_type = V;
        using variable_type = variable<V, D, G>;
        using tape_type = typename variable_type::tape_type;
        using index_type = typename variable_type::index_type;
        using index_mapper_type = std::map<value_type const*, index_type>;

    public:
        variable_manager();
        variable_manager(variable_manager<V, D, G>&& other);
        variable_manager(const variable_manager<V, D, G>& other);

    public:
        variable_type to_variable(const value_type& x);

    private:
        tape_type _tape;
        index_mapper_type _index_mapper;
    };

    template <typename V, typename D, template <typename> typename G>
    inline variable_manager<V, D, G>::variable_manager(
        variable_manager<V, D, G>&& other)
        : _tape(std::move(other._tape)),
        _index_mapper(std::move(other._index_mapper))
    {
    }

    template <typename V, typename D, template <typename> typename G>
    inline variable_manager<V, D, G>::variable_manager(
        const variable_manager<V, D, G>& other)
        : _tape(other._tape),
        _index_mapper(other._index_mapper)
    {
    }

    template <typename V, typename D, template <typename> typename G>
    inline variable_manager<V, D, G>::variable_manager()
        : _tape(), _index_mapper()
    {
    }

    template <typename V, typename D, template <typename> typename G>
    inline typename variable_manager<V, D, G>::variable_type
    variable_manager<V, D, G>::to_variable(const value_type & x)
    {
        const variable_type v(x, _tape, _index_mapper);
        _index_mapper.emplace(std::addressof(x), v.get_id());
        return v;
    }
} } }