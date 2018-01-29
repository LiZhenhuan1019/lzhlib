#ifndef LZHLIB_GRAPH_EDGE_ID_HPP
#define LZHLIB_GRAPH_EDGE_ID_HPP

#include <cstddef>     //for std::size_t
#include <istream>
#include <ostream>
#include "../repository/stock_id.hpp"

namespace lzhlib
{
    class edge_id
    {
        template <class, class>
        friend
        class graph_base;

    private:
        explicit edge_id(stock_id i)
            : id_(i)
        {}
    public:
        edge_id() = default;
        bool operator<(edge_id rhs) const
        {
            return id_ < rhs.id_;
        }
        bool operator==(edge_id rhs) const
        {
            return id_ == rhs.id_;
        }
        bool operator!=(edge_id rhs) const
        {
            return !(*this == rhs);
        }
    private:
        auto id() const noexcept
        {
            return id_;
        }
        stock_id id_;

        friend std::istream &operator>>(std::istream &in, edge_id &id)
        {
            std::size_t i;
            in >> i;
            id.id_ = stock_id(i);
            return in;
        }
        friend std::ostream &operator<<(std::ostream &out, edge_id id)
        {
            out << id.id().id() << " ";
            return out;
        }
    };

}
#endif //LZHLIB_GRAPH_EDGE_ID_HPP
