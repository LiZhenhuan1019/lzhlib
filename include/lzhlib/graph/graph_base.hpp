#ifndef LZHLIB_GRAPH_GRAPH_BASE_HPP
#define LZHLIB_GRAPH_GRAPH_BASE_HPP

#include "vertex.hpp"
#include "edge.hpp"
#include "lzhlib/repository/value_repo.hpp"

namespace lzhlib
{
    template <class VertexValueT, class EdgeValueT>
    class graph_base
    {
    public:
        using vertex_t = detail::vertex<VertexValueT>;
        using vertex_value_t = typename vertex_t::vertex_value_t;
        using edge_ref_t = edge_ref;
        using edge_t = detail::edge<EdgeValueT>;
        using edge_value_t = typename edge_t::edge_value_t;
        using pair_t = typename edge_t::pair_t;

        bool associated(vertex_id v, edge_id e)
        {
            return get_vertex(v).associated(e);
        }
        std::vector<vertex_id> neighbors(vertex_id v) const              //不如让用户 auto const& s = graph.associated_edges(v);
        {
            auto const &edges = associated_edges(v);                     //在 for(auto e : s)中
            std::vector<vertex_id> ret;                                  //再     vertex_id v = e.opposite_vertex();
            ret.reserve(edges.size());
            for (edge_ref_t e : edges)
                ret.push_back(e.opposite_vertex());
            return ret;
        }
        std::set<edge_ref_t, std::less<void>> const &associated_edges(vertex_id v) const  //比neighbors更建议使用(出于效率考虑)
        {
            return get_vertex(v).associated_edges();
        }

        template <class ...Args>
        vertex_id add_vertex(Args &&...args)
        {
            return vertex_id(vertex_repository.assign_object(std::in_place, std::forward<Args>(args)...));
        }

        pair_t associated_vertices(edge_id e) const
        {
            return get_edge(e).associated_vertices();
        }

        vertex_value_t &value(vertex_id v)
        {
            return get_vertex(v).vertex_value();
        }
        vertex_value_t const &value(vertex_id v) const
        {
            return get_vertex(v).vertex_value();
        }
        edge_value_t &value(edge_id e)
        {
            return get_edge(e).edge_value();
        }
        edge_value_t const &value(edge_id e) const
        {
            return get_edge(e).edge_value();
        }

        vertex_id first_vertex() const
        {
            return vertex_id{vertex_repository.first_id()};
        }

        vertex_id end_vertex() const
        {
            return vertex_id{vertex_repository.end_id()};
        }

        vertex_id next_vertex(vertex_id id) const
        {
            return vertex_id(vertex_repository.next_id(id.id()));
        }

        bool is_id_valid(vertex_id id) const
        {
            return vertex_repository.is_id_used(id.id());
        }
    protected:

        vertex_t &get_vertex(vertex_id v)
        {
            return vertex_repository.get_object(v.id());
        }
        vertex_t const &get_vertex(vertex_id v) const
        {
            return vertex_repository.get_object(v.id());
        }
        edge_t &get_edge(edge_id e)
        {
            return edge_repository.get_object(e.id());
        }
        edge_t const &get_edge(edge_id e) const
        {
            return edge_repository.get_object(e.id());
        }

        vertex_id to_vertex_id(object_id i)    //preserved.Derived class don't need this just for now.
        {
            return vertex_id(i);
        }
        edge_id to_edge_id(object_id i)
        {
            return edge_id(i);
        }
        object_id to_stock_id(vertex_id v)
        {
            return v.id();
        }
        object_id to_stock_id(edge_id e)
        {
            return e.id();
        }
        value_repo<vertex_t> vertex_repository;
        value_repo<edge_t> edge_repository;
        friend std::istream &operator>>(std::istream &in, graph_base &graph)
        {
            in >> graph.vertex_repository;
            in >> graph.edge_repository;
            return in;
        }
        friend std::ostream &operator<<(std::ostream &out, graph_base const &graph)
        {
            out << graph.vertex_repository;
            out << graph.edge_repository;
            return out;
        }
    };
}
#endif //LZHLIB_GRAPH_GRAPH_BASE_HPP
