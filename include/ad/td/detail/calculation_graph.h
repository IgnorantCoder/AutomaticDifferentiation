#pragma once

#include <deque>
#include <numeric>
#include <cassert>
#include <map>
#include <set>
#include <tuple>
#include <type_traits>
#include <vector>

#include <functional>
#include <memory>

namespace ad { namespace td { namespace detail {
    template <typename W>
    class calculation_graph {
    public:
        using vertex_index_type = std::size_t;
        using arc_index_type = std::size_t;
        using weight_type = W;

    private:
        using this_type = calculation_graph;
        using arc_type = std::tuple<
            vertex_index_type, //from 
            vertex_index_type, //to
            weight_type
        >;
        using arc_set = std::vector<arc_type>;

        using arc_ref_set
            = std::vector<arc_index_type>;
        using vertex_type 
            = std::pair<
                arc_ref_set,    //pointing to me
                arc_ref_set     //originating from me
            >;
        using vertex_set = std::vector<vertex_type>;

    private:
        static vertex_index_type get_start_vertex(const arc_type& arc);
        static vertex_index_type get_end_vertex(const arc_type& arc);
        static weight_type get_vertex_weight(const arc_type& arc);
        static arc_ref_set& get_arc_set_pointing_to(vertex_type& vertex);
        static arc_ref_set& get_arc_set_originating_from(vertex_type& vertex);
        static const arc_ref_set& get_arc_set_pointing_to(const vertex_type& vertex);
        static const arc_ref_set& get_arc_set_originating_from(const vertex_type& vertex);

    public:
        calculation_graph();
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
        arc_set _arc_set;
        vertex_set _vertex_set;
    };

    template<typename W>
    inline typename calculation_graph<W>::vertex_index_type 
    calculation_graph<W>::get_start_vertex(const arc_type & arc)
    {
        return std::get<0>(arc);
    }

    template<typename W>
    inline typename calculation_graph<W>::vertex_index_type 
    calculation_graph<W>::get_end_vertex(const arc_type & arc)
    {
        return std::get<1>(arc);
    }

    template<typename W>
    inline typename calculation_graph<W>::weight_type 
    calculation_graph<W>::get_vertex_weight(const arc_type & arc)
    {
        return std::get<2>(arc);
    }

    template<typename W>
    inline typename calculation_graph<W>::arc_ref_set&
    calculation_graph<W>::get_arc_set_pointing_to(vertex_type& vertex)
    {
        return vertex.first;
    }

    template<typename W>
    inline const typename calculation_graph<W>::arc_ref_set&
    calculation_graph<W>::get_arc_set_pointing_to(const vertex_type& vertex)
    {
        return vertex.first;
    }

    template<typename W>
    inline typename calculation_graph<W>::arc_ref_set&
    calculation_graph<W>::get_arc_set_originating_from(vertex_type& vertex)
    {
        return vertex.second;
    }

    template<typename W>
    inline const typename calculation_graph<W>::arc_ref_set&
    calculation_graph<W>::get_arc_set_originating_from(const vertex_type& vertex)
    {
        return vertex.second;
    }

    template<typename W>
    inline calculation_graph<W>::calculation_graph()
        : _arc_set(), _vertex_set()
    {
    }

    template<typename W>
    inline typename calculation_graph<W>::vertex_index_type 
    calculation_graph<W>::add_vertex()
    {
        _vertex_set.emplace_back(vertex_type());
        return _vertex_set.size() - 1;
    }

    template<typename W>
    inline typename calculation_graph<W>::vertex_index_type 
    calculation_graph<W>::add_vertex(
        const vertex_index_type from,
        const weight_type w)
    {
        const auto n = this->add_vertex();
        this->link(from, n, w);

        return n;
    }

    template<typename W>
    inline void calculation_graph<W>::link(
        const vertex_index_type from,
        const vertex_index_type to, 
        const weight_type w)
    {
        assert(from < _vertex_set.size());
        assert(to < _vertex_set.size());

        this->_arc_set.emplace_back(from, to, w);

        get_arc_set_originating_from(this->_vertex_set[from])
            .emplace_back(this->_arc_set.size() - 1);
        get_arc_set_pointing_to(this->_vertex_set[to])
            .emplace_back(this->_arc_set.size() - 1);

        return;
    }
    
    template<typename W>
    inline typename calculation_graph<W>::weight_type 
    calculation_graph<W>::sweep(
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

        if (get_arc_set_originating_from(_vertex_set.at(from)).empty()) {
            return weight_type(0);
        }

        const auto r = std::accumulate(
            std::cbegin(get_arc_set_originating_from(_vertex_set.at(from))),
            std::cend(get_arc_set_originating_from(_vertex_set.at(from))),
            weight_type(0),
            [this, from, to](const weight_type acc, const arc_index_type i){
                return acc 
                    + this_type::get_vertex_weight(_arc_set[i])
                    * this->sweep(this_type::get_end_vertex(_arc_set[i]), to); });

        return r;
    }
} } }