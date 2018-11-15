#ifndef LZHLIB_STDLIB_EXT_TYPE_TRAITS_EXT_HPP
#define LZHLIB_STDLIB_EXT_TYPE_TRAITS_EXT_HPP
#include <type_traits>
namespace lzhlib
{
    template <typename T>
    using remove_cvrf_t = std::remove_cv_t<std::remove_reference_t<T>>;

    template <std::size_t Level>
    struct overload_level : overload_level<Level - 1>
    {
    };
    template <>
    struct overload_level<0>
    {
    };
}
#endif //LZHLIB_STDLIB_EXT_TYPE_TRAITS_EXT_HPP
