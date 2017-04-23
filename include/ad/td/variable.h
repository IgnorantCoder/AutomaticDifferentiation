#pragma once

#include "ad/td/detail/calculation_graph.h"

namespace ad { namespace td {
    template <typename V, typename D>
    class variable {
    public:
        using value_type = V;
        using derivative_value_type = D;
        using this_type = variable<value_type, derivative_value_type>;
        using tape_type = detail::calculation_graph<derivative_value_type>;
        using index_type = typename tape_type::vertex_index_type;
        
    public:
        variable(value_type&& f, tape_type& tape);
        variable(const value_type& f, tape_type& tape);

    public:
        operator value_type() const;
        derivative_value_type operator()(const index_type i) const;
        variable<V, D> generate_and_link(
            const value_type& f,
            const derivative_value_type& df) const;

        void link(
            const variable<value_type, derivative_value_type>& other,
            const derivative_value_type& df) const;

    private:
        value_type _f;
        index_type _id;
        tape_type& _tape;
    };

    template<typename V, typename D>
    inline variable<V, D>::variable(value_type && f, tape_type & tape)
        : _f(f), _id(tape.add_vertex()), _tape(tape)
    {
    }

    template<typename V, typename D>
    inline variable<V, D>::variable(const value_type & f, tape_type & tape)
        : _f(f), _id(tape.add_vertex()), _tape(tape)
    {
    }

    template<typename V, typename D>
    inline variable<V, D>::operator typename variable<V, D>::value_type() const
    {
        return _f;
    }

    template<typename V, typename D>
    inline typename variable<V, D>::derivative_value_type
    variable<V, D>::operator()(const index_type i) const
    {
        return _tape.sweep(i, _id);
    }

    template<typename V, typename D>
    inline typename variable<V, D>::this_type
    variable<V, D>::generate_and_link(
        const value_type & f,
        const derivative_value_type & df) const
    {
        const variable<value_type, derivative_value_type> r(f, _tape);
        this->link(r, df);
        return r;
    }

    template<typename V, typename D>
    inline void 
    variable<V, D>::link(
        const variable<value_type, derivative_value_type>& other, 
        const derivative_value_type & df) const
    {
        _tape.link(_id, other._id, df);
        return;
    }

}}