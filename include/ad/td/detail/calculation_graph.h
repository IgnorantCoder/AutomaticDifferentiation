#pragma once

#include <iostream>
#include <numeric>
#include <cassert>
#include <map>
#include <set>
#include <tuple>
#include <type_traits>
#include <utility>

namespace ad { namespace td { namespace detail {
    template <typename W>
    class calculation_graph {
    public:
        using vertex_index_type = std::size_t;
        using weight_type = W;

    private:
        using this_type = calculation_graph;
        using arc_type = std::tuple<
            vertex_index_type, //from 
            vertex_index_type, //to
            weight_type
        >;
        using arc_set = std::set<arc_type>;

        struct comp_arc_ref {
            bool operator()(
                const arc_type& x,
                const arc_type& y) const;
            bool operator()(
                const std::reference_wrapper<const arc_type>& x,
                const std::reference_wrapper<const arc_type>& y) const;
        };

        using arc_ref_set
            = std::set<std::reference_wrapper<const arc_type>, comp_arc_ref>;
        using vertex_type 
            = std::pair<
                arc_ref_set,    //pointing to me
                arc_ref_set     //originating from me
            >;
        using vertex_set = std::map<vertex_index_type, vertex_type>;

    private:
        static vertex_index_type get_start_vertex(const arc_type& arc);
        static vertex_index_type get_end_vertex(const arc_type& arc);
        static weight_type get_vertex_weight(const arc_type& arc);
        static arc_ref_set& get_arc_set_pointing_to(vertex_type& vertex);
        static arc_ref_set& get_arc_set_originating_from(vertex_type& vertex);
        static const arc_ref_set& get_arc_set_pointing_to(const vertex_type& vertex);
        static const arc_ref_set& get_arc_set_originating_from(const vertex_type& vertex);

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
        vertex_index_type get_new_index();

    private:
        vertex_index_type _counter;
        arc_set _arc_set;
        vertex_set _vertex_set;
    };

    template<typename W>
    inline bool calculation_graph<W>::comp_arc_ref::operator()(
        const arc_type& x,
        const arc_type& y) const
    {
        return std::addressof(x) < std::addressof(y);
    }

    template<typename W>
    inline bool calculation_graph<W>::comp_arc_ref::operator()(
        const std::reference_wrapper<const arc_type>& x,
        const std::reference_wrapper<const arc_type>& y) const
    {
        return (*this)(x.get(), y.get());
    }

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
        return get_arc_set_pointing_to(*const_cast<vertex_type*>(&vertex));
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
        return get_arc_set_originating_from(*const_cast<vertex_type*>(&vertex));
    }

    template<typename W>
    inline typename calculation_graph<W>::vertex_index_type 
    calculation_graph<W>::add_vertex()
    {
        const auto n = this->get_new_index();
        _vertex_set.emplace(n, vertex_type());

        return n;
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
        assert(_vertex_set.find(from) != _vertex_set.cend());
        assert(_vertex_set.find(to) != _vertex_set.cend());

        const auto it = this->_arc_set.emplace(from, to, w);
        const auto arc_ref = std::cref(*it.first);

        get_arc_set_originating_from(this->_vertex_set[from]).insert(arc_ref);
        get_arc_set_pointing_to(this->_vertex_set[to]).insert(arc_ref);

        return;
    }
    
    template<typename W>
    inline typename calculation_graph<W>::weight_type 
    calculation_graph<W>::sweep(
        const vertex_index_type from, 
        const vertex_index_type to) const
    {
        if (_vertex_set.find(from) == _vertex_set.cend()) {
            std::cout << from << "," << to << std::endl;
        }

        assert(_vertex_set.find(from) != _vertex_set.cend());
        assert(_vertex_set.find(to) != _vertex_set.cend());

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
            [this, from, to](const weight_type acc, const auto& arc){
                return acc 
                    + this_type::get_vertex_weight(arc)
                    * this->sweep(this_type::get_end_vertex(arc), to); });

        return r;
    }

    template<typename W>
    inline typename calculation_graph<W>::vertex_index_type 
    calculation_graph<W>::get_new_index()
    {
        return _counter++;
    }
} } }