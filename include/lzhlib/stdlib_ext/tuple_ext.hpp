#ifndef LZHLIB_STDLIB_EXT_TUPLE_EXT_HPP
#define LZHLIB_STDLIB_EXT_TUPLE_EXT_HPP
#include <tuple>
#include <utility>
#include <initializer_list>
#include <lzhlib/stdlib_ext/type_traits_ext.hpp>
//#include "lzhlib/stdlib_ext/stdlib_ext_conf.hpp"
namespace lzhlib
{
    namespace tuple
    {
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
        namespace detail
        {
            template <std::size_t Level>
            struct overload_level : overload_level<Level - 1>
            {
            };
            template <>
            struct overload_level<0>
            {
            };
            template <std::size_t Index, typename Tuple>
            constexpr std::tuple_element_t<Index, Tuple> get_argument(overload_level<1>)
            {
                return std::tuple_element_t<Index, Tuple>();
            }
            template <std::size_t Index, typename Tuple, typename Argument, typename ...ArgumentSeq, typename = std::enable_if_t<std::is_same_v<arg_helper<Index, typename remove_cvrf_t<Argument>::value_type>, remove_cvrf_t<Argument>>, void>>
            constexpr std::tuple_element_t<Index, Tuple> get_argument(overload_level<1>, Argument &&argument, ArgumentSeq &&...argument_seq)
            {
                return std::tuple_element_t<Index, Tuple>(std::forward<typename remove_cvrf_t<Argument>::value_type &&>(argument.value));
            }
            template <std::size_t Index, typename Tuple, typename Argument, typename ...ArgumentSeq>
            constexpr std::tuple_element_t<Index, Tuple> get_argument(overload_level<0>, Argument &&argument, ArgumentSeq &&...argument_seq)
            {
                return get_argument<Index, Tuple>(overload_level<1>{}, argument_seq...);
            }

            template <typename Tuple, std::size_t ...Indices, typename ...Arguments>
            constexpr Tuple create_tuple_impl(std::index_sequence<Indices...>, Arguments &&...arguments)
            {
                return Tuple{get_argument<Indices, Tuple>(overload_level<1>{}, arguments...)...};
            };
        }
        template <typename Tuple, typename ...Arguments>
        constexpr Tuple create_tuple(Arguments &&...arguments)
        {
            return detail::create_tuple_impl<Tuple>(std::make_index_sequence<std::tuple_size_v<Tuple>>{}, std::forward<Arguments>(arguments)...);
        }
    }
}
#endif //LZHLIB_STDLIB_EXT_TUPLE_EXT_HPP
