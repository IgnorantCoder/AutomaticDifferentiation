#pragma once

#include <map>

namespace ad { namespace tape {
    template <typename V, typename D, template <typename> typename G>
    class variable {
    public:
        using value_type = V;
        using derivative_value_type = D;
        using tape_type = G<derivative_value_type>;
        using index_type = typename tape_type::vertex_index_type;
        using expression_type = variable<value_type, derivative_value_type, G>;
        using index_mapper_type = std::map<value_type const*, index_type>;

    public:
        variable();
        variable(
            const value_type& val,
            tape_type& tape,
            const index_mapper_type& index_mapper);
        variable(variable&& other);
        variable(const variable& other);

    public:
        explicit operator value_type() const;
        expression_type generate_and_link(
            const value_type& v,
            const derivative_value_type& d) const;
        void link_from_this(
            const expression_type& to,
            const derivative_value_type& d) const;
        index_type get_id() const;
        const tape_type& get_tape() const;
        bool try_get_index(
            index_type& index,
            const typename index_mapper_type::key_type& key) const;

    private:
        value_type _val;
        index_type _id;
        tape_type& _tape;
        const index_mapper_type& _index_mapper;
    };

    template<typename V, typename D, template <typename> typename G>
    inline variable<V, D, G>::variable()
        : _val(),
        _id(),
        _tape(tape_type()),
        _index_mapper(index_mapper_type()) 
    {
    }

    template<typename V, typename D, template <typename> typename G>
    inline variable<V, D, G>::variable(
        const value_type & val, 
        tape_type & tape, 
        const index_mapper_type & index_mapper)
        : _val(val),
        _id(tape.add_vertex()),
        _tape(tape),
        _index_mapper(index_mapper)
    {
    }

    template<typename V, typename D, template <typename> typename G>
    inline variable<V, D, G>::variable(variable&& other)
        : _val(std::move(other._val)),
        _id(std::move(other._id)),
        _tape(other._tape),
        _index_mapper(other._index_mapper)
    {
    }

    template<typename V, typename D, template <typename> typename G>
    inline variable<V, D, G>::variable(const variable& other)
        : _val(other._val),
        _id(other._id),
        _tape(other._tape),
        _index_mapper(other._index_mapper)
    {
    }

    template<typename V, typename D, template <typename> typename G>
    inline variable<V, D, G>::operator typename variable<V, D, G>::value_type() const
    {
        return _val;
    }

    template<typename V, typename D, template <typename> typename G>
    inline typename variable<V, D, G>::expression_type 
    variable<V, D, G>::generate_and_link(
        const value_type& v,
        const derivative_value_type& d) const
    {
        const expression_type ret(v, _tape, _index_mapper);
        _tape.link(this->get_id(), ret.get_id(), d);
        return ret;
    }

    template<typename V, typename D, template <typename> typename G>
    inline void variable<V, D, G>::link_from_this(
        const expression_type& to,
        const derivative_value_type & d) const
    {
        _tape.link(this->get_id(), to.get_id(), d);
        return;
    }

    template<typename V, typename D, template <typename> typename G>
    inline typename variable<V, D, G>::index_type
    variable<V, D, G>::get_id() const
    {
        return _id;
    }

    template<typename V, typename D, template <typename> typename G>
    inline const typename variable<V, D, G>::tape_type&
    variable<V, D, G>::get_tape() const
    {
        return _tape;
    }

    template<typename V, typename D, template <typename> typename G>
    inline bool variable<V, D, G>::try_get_index(
        index_type& index,
        const typename index_mapper_type::key_type & key) const
    {
        const auto it = _index_mapper.find(key);
        if (it != _index_mapper.cend()) {
            index = it->second;
            return true;
        }
        return false;
    }
}}