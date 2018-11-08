#ifndef LZHLIB_TYPE_TRAITS_EXT_HPP
#define LZHLIB_TYPE_TRAITS_EXT_HPP
#include <type_traits>
namespace lzhlib
{
    template <typename T>
    using remove_cvrf_t = std::remove_cv_t<std::remove_reference_t<T>>;
}
#endif //LZHLIB_TYPE_TRAITS_EXT_HPP
