#ifndef LZHLIB_UTILITY_OPTIONAL_HPP
#define LZHLIB_UTILITY_OPTIONAL_HPP
#include <exception>
#include "lzhlib/lzhlib_conf.hpp"

namespace lzhlib
{
    struct nullopt_t
    {
        enum class construct
        {
            token
        };
        explicit constexpr nullopt_t(construct)
        {}
    };
    inline constexpr nullopt_t nullopt{nullopt_t::construct::token};

    struct bad_optional_access : public std::exception
    {
        char const *what() const noexcept override
        { return "bad optional access."; }
        ~bad_optional_access() override = default;
    };

    [[noreturn]] inline void throw_bad_optional_access()
    {
        lzhlib_throw_or_abort(bad_optional_access());
    }
}

#endif //LZHLIB_UTILITY_OPTIONAL_HPP
