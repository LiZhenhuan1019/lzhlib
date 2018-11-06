#ifndef LZHLIB_STDLIB_EXT_TUPLE_EXT_HPP
#define LZHLIB_STDLIB_EXT_TUPLE_EXT_HPP
#include <tuple>
#include <utility>
#include <initializer_list>
//#include "lzhlib/stdlib_ext/stdlib_ext_conf.hpp"
namespace lzhlib
{
    namespace tuple
    {
        namespace detail
        {
            template <typename T>
            using remove_cvrf_t = std::remove_cv_t<std::remove_reference_t<T>>;
            template <typename Tuple, std::size_t Index>
            constexpr std::tuple_element_t<Index, Tuple> get_argument()
            {
                return std::tuple_element_t<Index, Tuple>();
            }
            template <typename Tuple, std::size_t Index, typename Argument, typename ...ArgumentSeq, std::enable_if_t<Index == remove_cvrf_t<Argument>::index, int> = 0>
            constexpr std::tuple_element_t<Index, Tuple> get_argument(Argument &&argument, ArgumentSeq &&...argument_seq)
            {
                return std::forward<typename remove_cvrf_t<Argument>::value_type &&>(argument.value);
            }
            template <typename Tuple, std::size_t Index, typename Argument, typename ...ArgumentSeq, std::enable_if_t<Index != remove_cvrf_t<Argument>::index, short> = 0>
            constexpr std::tuple_element_t<Index, Tuple> get_argument(Argument &&argument, ArgumentSeq &&...argument_seq)
            {
                return get_argument<Tuple, Index>(argument_seq...);
            }

            template <typename Tuple, std::size_t ...Indices, typename ...Arguments>
            constexpr Tuple create_tuple_impl(std::index_sequence<Indices...>, Arguments &&...arguments)
            {
                return Tuple{get_argument<Tuple, Indices>(arguments...)...};
            };
        }
        template <std::size_t Index, typename ValueType>
        struct arg_helper
        {
            static constexpr std::size_t index = Index;
            using value_type = ValueType;
            explicit arg_helper(value_type &&element) noexcept(noexcept(value_type(std::forward<value_type &&>(element))))
                : value(std::forward<value_type>(element))
            {}
            value_type &&value;
        };
        template <std::size_t Index, typename ValueType>
        arg_helper<Index, ValueType> arg(ValueType &&value)
        {
            return arg_helper<Index, ValueType>{std::forward<ValueType>(value)};
        }
        template <std::size_t Index, typename ValueType>
        arg_helper<Index, std::initializer_list<ValueType>> arg(std::initializer_list<ValueType> &&list)
        {
            return arg_helper<Index, std::initializer_list<ValueType>>{std::move(list)};
        }
        template <typename Tuple, typename ...Arguments>
        constexpr Tuple create_tuple(Arguments &&...arguments)
        {
            return detail::create_tuple_impl<Tuple>(std::make_index_sequence<std::tuple_size_v<Tuple>>{}, std::forward<Arguments>(arguments)...);
        }
    }
}
#endif //LZHLIB_STDLIB_EXT_TUPLE_EXT_HPP
