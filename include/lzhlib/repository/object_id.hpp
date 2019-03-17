#ifndef LZHLIB_REPOSITORY_STOCK_ID_HPP
#define LZHLIB_REPOSITORY_STOCK_ID_HPP

#include <cstddef>    //for std::size_t

namespace lzhlib
{
    namespace detail
    {

        struct invalid_id_t
        {
            explicit constexpr invalid_id_t() = default;
        };
    }
    constexpr std::size_t max_size_t_v = std::numeric_limits<std::size_t>::max();
    constexpr detail::invalid_id_t invalid_id;

    class object_id
    {
    public:
        constexpr object_id()
            : object_id(invalid_id)
        {}
        constexpr object_id(std::size_t i)
            : id_(i)
        {}
        constexpr object_id(detail::invalid_id_t)
            : id_(max_size_t_v)
        {}
        auto id() const noexcept
        {
            return id_;
        }
        object_id &operator++()
        {
            ++id_;
            return *this;
        }
        object_id &operator--()
        {
            --id_;
            return *this;
        }
        bool operator<(object_id rhs) const
        {
            return id_ < rhs.id_;
        }

        bool operator==(object_id rhs) const
        {
            return id_ == rhs.id_;
        }
        bool operator!=(object_id rhs) const
        {
            return id_ != rhs.id_;
        }


    private:
        std::size_t id_;
    };
}
#endif //LZHLIB_REPOSITORY_STOCK_ID_HPP
