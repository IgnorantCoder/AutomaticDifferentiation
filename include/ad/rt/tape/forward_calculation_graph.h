#pragma once

#include <algorithm>
#include <cassert>
#include <numeric>
#include <vector>

namespace ad { namespace rt { namespace tape {
    template <typename W>
    class forward_calculation_graph {
    public:
        using weight_type = W;
        using vertex_index_type = std::size_t;

    private:
        using this_type = forward_calculation_graph;
        using arc_type = std::pair<vertex_index_type, weight_type>;
        using arc_set = std::vector<arc_type>;
        using vertex_type = arc_set;
        using vertex_set = std::vector<vertex_type>;

    private:
        static vertex_index_type get_end_of(const arc_type& arc);
        static weight_type get_weight_of(const arc_type& arc);
        static arc_set& get_arc_set_originating_from(vertex_type& vertex);
        static const arc_set& get_arc_set_originating_from(const vertex_type& vertex);

    public:
        forward_calculation_graph();
        forward_calculation_graph(forward_calculation_graph&& other);
        forward_calculation_graph(const forward_calculation_graph& other);

    public:
        vertex_index_type add_vertex();
        vertex_index_type add_vertex(
            const vertex_index_type from,
            const weight_type w);
        void link(
            const vertex_index_type from,
            const vertex_index_type to,
            const weight_type w);
        weight_type sweep(
            const vertex_index_type from,
            const vertex_index_type to) const;

    private:
        vertex_set _vertex_set;
    };

    template<typename W>
    inline typename forward_calculation_graph<W>::vertex_index_type
    forward_calculation_graph<W>::get_end_of(const arc_type & arc)
    {
        return arc.first;
    }

    template<typename W>
    inline typename forward_calculation_graph<W>::weight_type
    forward_calculation_graph<W>::get_weight_of(const arc_type & arc)
    {
        return arc.second;
    }

    template<typename W>
    inline typename forward_calculation_graph<W>::arc_set&
    forward_calculation_graph<W>::get_arc_set_originating_from(
        vertex_type& vertex)
    {
        return vertex;
    }

    template<typename W>
    inline const typename forward_calculation_graph<W>::arc_set&
    forward_calculation_graph<W>::get_arc_set_originating_from(
        const vertex_type& vertex)
    {
        return vertex;
    }

    template<typename W>
    inline forward_calculation_graph<W>::forward_calculation_graph()
        : _vertex_set()
    {
    }

    template<typename W>
    inline forward_calculation_graph<W>::forward_calculation_graph(
        forward_calculation_graph<W>&& other)
        : _vertex_set(std::move(other._vertex_set))
    {
    }

    template<typename W>
    inline forward_calculation_graph<W>::forward_calculation_graph(
        const forward_calculation_graph<W>& other)
        : _vertex_set(other._vertex_set)
    {
    }

    template<typename W>
    inline typename forward_calculation_graph<W>::vertex_index_type
    forward_calculation_graph<W>::add_vertex()
    {
        _vertex_set.emplace_back();
        return _vertex_set.size() - 1;
    }

    template<typename W>
    inline typename forward_calculation_graph<W>::vertex_index_type 
    forward_calculation_graph<W>::add_vertex(
        const vertex_index_type from,
        const weight_type w)
    {
        const auto to = this->add_vertex();
        this->link(from, to, w);
        return vertex_index_type();
    }

    template<typename W>
    inline void forward_calculation_graph<W>::link(
        const vertex_index_type from,
        const vertex_index_type to,
        const weight_type w)
    {
        this_type::get_arc_set_originating_from(_vertex_set[from]).emplace_back(to, w);
        static const auto comp = [](const auto& x, const auto& y) {
            return this_type::get_end_of(x) < this_type::get_end_of(y);
        };
        std::sort(
            std::begin(_vertex_set[to]),
            std::end(_vertex_set[to]),
            comp);
        return;
    }

    template<typename W>
    inline typename forward_calculation_graph<W>::weight_type
    forward_calculation_graph<W>::sweep(
        const vertex_index_type from,
        const vertex_index_type to) const
    {
        assert(from < _vertex_set.size());
        assert(to < _vertex_set.size());

        if (from > to) { //pruning
            return weight_type(0);
        }

        if (from == to) {
            return weight_type(1);
        }

        if (this_type::get_arc_set_originating_from(_vertex_set[from]).empty()) {
            return weight_type(0);
        }

        const auto& arc_set = get_arc_set_originating_from(_vertex_set[from]);
        const auto accumulator = [this, to](
            const weight_type acc, 
            const arc_type& arc) {
            return acc 
                + this_type::get_weight_of(arc)
                * this->sweep(this_type::get_end_of(arc), to);
        };

        const auto ret = std::accumulate(
            std::cbegin(arc_set),
            std::cend(arc_set),
            weight_type(0),
            accumulator);

        return ret;
    }

} } }