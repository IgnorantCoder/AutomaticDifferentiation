#pragma once

#include <algorithm>
#include <cassert>
#include <vector>
#include <utility>

namespace ad { namespace rt { namespace tape {
    template <typename W>
    class reverse_calculation_graph {
    public:
        using weight_type = W;
        using vertex_index_type = std::size_t;
        using gradient_type = std::vector<weight_type>;

    private:
        using this_type = reverse_calculation_graph;
        using arc_type = std::pair<vertex_index_type, weight_type>;
        using arc_set = std::vector<arc_type>;
        using vertex_type = arc_set;
        using vertex_set = std::vector<vertex_type>;

    private:
        static vertex_index_type get_start_of(const arc_type& arc);
        static weight_type get_weight_of(const arc_type& arc);
        static arc_set& get_arc_set_pointing_to(vertex_type& vertex);
        static const arc_set& get_arc_set_pointing_to(const vertex_type& vertex);

    public:
        reverse_calculation_graph();
        reverse_calculation_graph(reverse_calculation_graph&& other);
        reverse_calculation_graph(const reverse_calculation_graph& other);

    public:
        vertex_index_type add_vertex();
        vertex_index_type add_vertex(
            const vertex_index_type from,
            const weight_type w);
        void link(
            const vertex_index_type from,
            const vertex_index_type to,
            const weight_type w);
        gradient_type sweep(const vertex_index_type i) const;

    private:
        void sweep(
            gradient_type& ret,
            const vertex_index_type i) const;

    private:
        vertex_set _vertex_set;
    };

    template<typename W>
    inline typename reverse_calculation_graph<W>::vertex_index_type
    reverse_calculation_graph<W>::get_start_of(const arc_type & arc)
    {
        return arc.first;
    }

    template<typename W>
    inline typename reverse_calculation_graph<W>::weight_type
    reverse_calculation_graph<W>::get_weight_of(const arc_type & arc)
    {
        return arc.second;
    }

    template<typename W>
    inline typename reverse_calculation_graph<W>::arc_set&
    reverse_calculation_graph<W>::get_arc_set_pointing_to(vertex_type & vertex)
    {
        return vertex;
    }

    template<typename W>
    inline const typename reverse_calculation_graph<W>::arc_set&
    reverse_calculation_graph<W>::get_arc_set_pointing_to(const vertex_type& vertex)
    {
        return vertex;
    }

    template<typename W>
    inline reverse_calculation_graph<W>::reverse_calculation_graph()
        : _vertex_set()
    {
    }

    template<typename W>
    inline reverse_calculation_graph<W>::reverse_calculation_graph(
        reverse_calculation_graph<W>&& other)
        : _vertex_set(std::move(other._vertex_set))
    {
    }

    template<typename W>
    inline reverse_calculation_graph<W>::reverse_calculation_graph(
        const reverse_calculation_graph<W>& other)
        : _vertex_set(other._vertex_set)
    {
    }

    template<typename W>
    inline typename reverse_calculation_graph<W>::vertex_index_type 
    reverse_calculation_graph<W>::add_vertex()
    {
        _vertex_set.emplace_back();
        _vertex_set.back().reserve(2);
        return _vertex_set.size() - 1;
    }

    template<typename W>
    inline typename reverse_calculation_graph<W>::vertex_index_type
    reverse_calculation_graph<W>::add_vertex(
        const vertex_index_type from,
        const weight_type w)
    {
        const auto to = this->add_vertex();
        this->link(from, to, w);
        return to;
    }

    template<typename W>
    inline void reverse_calculation_graph<W>::link(
        const vertex_index_type from, 
        const vertex_index_type to, 
        const weight_type w)
    {
        this_type::get_arc_set_pointing_to(_vertex_set[to]).emplace_back(from, w);
        static const auto comp = [](const auto& x, const auto& y) {
            return this_type::get_start_of(x) > this_type::get_start_of(y);
        };
        std::sort(
            std::begin(_vertex_set[to]),
            std::end(_vertex_set[to]),
            comp);
        return;
    }

    template<typename W>
    inline typename reverse_calculation_graph<W>::gradient_type
    reverse_calculation_graph<W>::sweep(const vertex_index_type i) const
    {
        assert(i < _vertex_set.size());

        std::vector<weight_type> gradient(_vertex_set.size(), weight_type(0));
        gradient[i] = weight_type(1);
        this->sweep(gradient, i);

        return gradient;
    }

    template<typename W>
    inline void reverse_calculation_graph<W>::sweep(
        gradient_type & ret, 
        const vertex_index_type i) const
    {
        const auto& arcs = this_type::get_arc_set_pointing_to(_vertex_set[i]);
        for (const auto& arc : arcs) {
            ret[this_type::get_start_of(arc)]
                += this_type::get_weight_of(arc) * ret[i];
        }
        for (const auto& arc : arcs) {
            this->sweep(ret, this_type::get_start_of(arc));
        }
    }
} } }