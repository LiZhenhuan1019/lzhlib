#ifndef LZHLIB_REPOSITORY_STOCK_ID_HPP
#define LZHLIB_REPOSITORY_STOCK_ID_HPP

#include <cstddef>    //for std::size_t

namespace lzhlib
{
    namespace detail
    {
        struct initializer_holder
        {
        };

        struct invalid_id_t
        {
            constexpr invalid_id_t(initializer_holder)
            {}
        };
    }
    constexpr detail::invalid_id_t invalid_id{detail::initializer_holder{}};

    class stock_id
    {
    public:
        constexpr stock_id()
            : stock_id(invalid_id)
        {}
        constexpr stock_id(std::size_t i)   //not private and make repositories as friend class,because there are too many types of repository.
            : id_(i)
        {}
        constexpr stock_id(detail::invalid_id_t)
            : id_(static_cast<std::size_t>(-1))
        {}
        bool operator<(stock_id rhs) const
        {
            return id_ < rhs.id_;
        }

        bool operator==(stock_id rhs) const
        {
            return id_ == rhs.id_;
        }
        bool operator!=(stock_id rhs) const
        {
            return id_ != rhs.id_;
        }

        auto id() const noexcept
        {
            return id_;
        }
        stock_id &operator++()
        {
            ++id_;
            return *this;
        }
        stock_id &operator--()
        {
            --id_;
            return *this;
        }

    private:
        std::size_t id_;
    };
}
#endif //LZHLIB_REPOSITORY_STOCK_ID_HPP
