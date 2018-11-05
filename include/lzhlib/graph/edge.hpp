#ifndef LZHLIB_GRAPH_EDGE_HPP
#define LZHLIB_GRAPH_EDGE_HPP

#include <utility>    //for std::pair
#include "vertex_id.hpp"

namespace lzhlib
{
    class null_value_tag;
    namespace detail
    {
        template <class EdgeValueT = null_value_tag>
        class edge;

        template <>
        class edge<null_value_tag>
        {
        public:
            using edge_value_t = null_value_tag;
            using pair_t = std::pair<vertex_id, vertex_id>;

            edge() = default;
            edge(std::in_place_t)
            {}
            edge(null_value_tag)
            {}
            bool is_associated(vertex_id i) const
            {
                return i == vertices.first || i == vertices.second;
            }
            pair_t associated_vertices() const
            {
                return vertices;
            }
            void set_associated_vertices(vertex_id v1, vertex_id v2)
            {
                vertices.first = v1;
                vertices.second = v2;
            }
            void detach()
            {
                vertices.first = vertices.second = invalid_vertex_id;
            }

        private:
            pair_t vertices;   //对于 undirected graph,vertices.first 与vertices.second 的地位相同.对于directed graph,vertices.first为边的起点,vertices.second为边的终点.

            template <typename ValueT>
            friend std::istream &operator>>(std::istream &in, edge<ValueT> &edge)
            {
                if constexpr (!std::is_same_v<ValueT, lzhlib::null_value_tag>)
                {
                    in >> edge.edge_value();
                }
                in >> edge.vertices.first >> edge.vertices.second;
                return in;
            }
            template <typename ValueT>
            friend std::ostream &operator<<(std::ostream &out, edge <ValueT> const &edge)
            {
                if constexpr (!std::is_same_v<ValueT, lzhlib::null_value_tag>)
                {
                    out << edge.edge_value() << " ";
                }
                out << edge.vertices.first << edge.vertices.second;
                return out;
            }
        };

        template <class EdgeValueT>
        class edge : public edge<null_value_tag>
        {
        public:
            using edge_value_t = EdgeValueT;

            edge() = default;
            template <class ...Args>
            edge(std::in_place_t, Args &&...args)
                : value(std::forward<Args>(args)...)
            {
            }

            edge_value_t &edge_value() &
            {
                return value;
            }
            edge_value_t &&edge_value() &&
            {
                return std::move(value);
            }
            edge_value_t const &edge_value() const &
            {
                return value;
            }
            edge_value_t const &&edge_value() const &&
            {
                return std::move(value);
            }

        private:
            edge_value_t value;
        };
    }
}


#endif //LZHLIB_GRAPH_EDGE_HPP
