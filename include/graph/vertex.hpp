#ifndef LZHLIB_GRAPH_VERTEX_HPP
#define LZHLIB_GRAPH_VERTEX_HPP

#include <functional>    //for std::less
#include <set>           //for std::set
#include <stdexcept>     //for std::logic_error
#include "edge_id.hpp"
#include "vertex_id.hpp"

namespace lzhlib
{
    namespace exceptions
    {
        class require_edge_that_does_not_exist : public std::logic_error
        {
        public:
            require_edge_that_does_not_exist()
                : std::logic_error(std::string("require edge that doesn't exist!"))
            {

            }
        };
    }

    class null_value_tag
    {
    };
    namespace detail
    {
        class edge_ref
        {
        public:
            edge_ref() = default;
            edge_ref(edge_id e, vertex_id opposite)
                : edge_(e), vertex_(opposite)
            {}
            edge_id edge() const
            {
                return edge_;
            }
            vertex_id opposite_vertex() const
            {
                return vertex_;
            }
            bool is_connected(vertex_id v) const
            {
                return vertex_ == v;
            }

            bool operator<(edge_ref rhs) const  //默认忽略opposite_vertex()
            {
                return edge_ < rhs.edge_;
            }
            bool operator==(edge_ref rhs) const
            {
                return edge_ == rhs.edge_;
            }
            bool operator!=(edge_ref rhs) const
            {
                return !(*this == rhs);
            }

        private:
            edge_id edge_;
            vertex_id vertex_;

            friend std::istream &operator>>(std::istream &in, edge_ref &r)
            {
                in >> r.edge_ >> r.vertex_;
                return in;
            }
            friend std::ostream &operator<<(std::ostream &out, edge_ref r)
            {
                out << r.edge_ << r.vertex_;
                return out;
            }
        };

        inline bool operator<(edge_ref r, edge_id rhs)
        {
            return r.edge() < rhs;
        }
        inline bool operator<(edge_id rhs, edge_ref r)
        {
            return rhs < r.edge();
        }
        inline bool operator<(edge_ref r, vertex_id rhs) //仍可依据opposite_vertex()来比较
        {
            return r.opposite_vertex() < rhs;
        }
        inline bool operator<(vertex_id rhs, edge_ref r) //仍可依据opposite_vertex()来比较
        {
            return rhs < r.opposite_vertex();
        }


        template <class VertexValueT = null_value_tag>
        class vertex;

        template <>
        class vertex<null_value_tag>
        {

        public:
            using vertex_value_t = null_value_tag;

            vertex() = default;
            vertex(null_value_tag)
            {}

//            bool is_associated(edge_id i)
//            {
//                return edges.find(i)!=edges.end();            //a possibly more efficent way :ask edge for this
//            }
            auto const &associated_edges() const
            {
                return edges;
            }
            void add_associated_edge(edge_ref i)
            {
                edges.insert(i);
            }
            void remove_associated_edge(edge_id i)
            {
                auto to_be_erased = edges.find(i);
                edges.erase(to_be_erased);
            }

            bool adjacent(vertex_id v) const
            {
                auto const &edges = associated_edges();
                if (edges.find(v) != edges.end())
                    return true;
                return false;
            }
            bool associated(edge_id e) const
            {
                auto const &edges = associated_edges();
                if (edges.find(e) != edges.end())
                    return true;
                return false;
            }

            edge_id associated_edge(vertex_id y) const
            {
                auto const &edges = associated_edges();
                for (edge_ref e : edges)
                {
                    if (e.is_connected(y))
                        return e.edge();
                }
                throw exceptions::require_edge_that_does_not_exist();
            }

        private:
            std::set<edge_ref, std::less<void>> edges;  //for std::less<void>::is_transparent  --> for comparision between differnet types
            template <typename ValueT>
            friend std::istream &operator>>(std::istream &in, vertex<ValueT> &vertex)
            {
                if constexpr (!std::is_same_v<ValueT, lzhlib::null_value_tag>)
                {
                    in >> vertex.vertex_value();
                }
                in >> vertex.edges;
                return in;
            }
            template <typename ValueT>
            friend std::ostream &operator<<(std::ostream &out, vertex <ValueT> const &vertex)
            {
                if constexpr (!std::is_same_v<ValueT, lzhlib::null_value_tag>)
                {
                    out << vertex.vertex_value() << " ";
                }
                auto const &edge_refs = vertex.associated_edges();
                out << edge_refs;
                return out;
            }
        };

        template <class VertexValueT>
        class vertex : public vertex<null_value_tag>
        {
        public:
            using vertex_value_t = VertexValueT;

            vertex() = default;
            template <class ...Args>
            vertex(std::in_place_t, Args &&... args)
                : value(std::forward<Args>(args)...)
            {

            }

            vertex_value_t &vertex_value() &
            {
                return value;
            }
            vertex_value_t &&vertex_value() &&
            {
                return std::move(value);
            }
            vertex_value_t const &vertex_value() const &
            {
                return value;
            }
            vertex_value_t const &&vertex_value() const &&
            {
                return std::move(value);
            }

        private:
            vertex_value_t value;
        };
    }
}
#endif //LZHLIB_GRAPH_VERTEX_HPP
