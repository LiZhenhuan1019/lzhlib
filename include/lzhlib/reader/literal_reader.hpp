#ifndef LZHLIB_READER_LITERAL_READER_HPP
#define LZHLIB_READER_LITERAL_READER_HPP
#include <string_view>
#include <tuple>
#include <utility>
#include <charconv>
#include <stdexcept>
#include "lzhlib/utility/tuple_ext.hpp"
namespace lzhlib
{
    namespace reader
    {
        enum class reader_enum
        {
            arithmetic,
            string,
            user_defined
        };
        namespace detail
        {
            template <typename StringT>
            struct is_string_base : std::false_type
            {
            };
#define IS_STRING(TYPE) \
        template <> \
        struct is_string_base<TYPE> : std::true_type \
        {};
            IS_STRING(std::string)
            IS_STRING(std::wstring)
            IS_STRING(std::u16string)
            IS_STRING(std::u32string)
#undef IS_STRING
            template <typename T>
            struct is_string : detail::is_string_base<std::remove_cv_t<std::remove_reference_t<T>>>
            {
            };
            template <typename T>
            constexpr auto is_string_v = is_string<T>::value;

            template <typename Type, reader_enum e, typename From = std::string_view>
            struct predefined_reader
            {

            };
            template <typename ArithmeticT, typename From>
            struct predefined_reader<ArithmeticT, reader_enum::arithmetic, From>
            {
                ArithmeticT operator()(From str) const
                {
                    using namespace std::literals;
                    ArithmeticT result{};
                    auto[ptr, error] = std::from_chars(str.data(), str.data() + str.size(), result);
                    if (ptr == str.data())
                        throw std::logic_error("expect integer, got "s + str.data());
                    return result;
                }
            };
            template <typename StringT, typename From>
            struct predefined_reader<StringT, reader_enum::string, From>
            {
                StringT operator()(From str) const
                {
                    using traits = typename StringT::traits_type;
                    using from_traits = typename From::traits_type;
                    typename StringT::value_type delimiter{};
                    if (from_traits::eq(str[0], '\''))
                        traits::assign(delimiter, '\'');
                    else if (from_traits::eq(str[0], '"'))
                        traits::assign(delimiter, '"');
                    else
                        throw std::logic_error("expect ' or \"");
                    auto end = str.find_last_of(delimiter);
                    if (end == From::npos || end == 0)
                        throw std::logic_error("expect close of quote");
                    return StringT(str.substr(1, end - 1));
                }
            };
        }
        struct use_default_tag
        {
        };
        namespace detail
        {
            template <typename ObjectType, typename Reader, typename From, typename = std::void_t<>>
            struct type_base;
            template <typename ObjectType, typename Reader, typename From>
            struct type_base<ObjectType, Reader, From, std::enable_if_t<std::is_invocable_r_v<ObjectType, Reader, From>>>
            {
                using object_type = ObjectType;
                using reader_type = Reader;
            };
            template <typename ObjectType, typename From>
            struct type_base<ObjectType, use_default_tag, From, std::enable_if_t<!std::is_arithmetic_v<ObjectType> && !is_string_v<ObjectType>>>;
            template <typename ObjectType, typename From>
            struct type_base<ObjectType, use_default_tag, From, std::enable_if_t<std::is_arithmetic_v<ObjectType>>>
                : type_base<remove_cvrf_t<ObjectType>, predefined_reader<remove_cvrf_t<ObjectType>, reader_enum::arithmetic, From>, From>
            {
            };
            template <typename ObjectType, typename From>
            struct type_base<ObjectType, use_default_tag, From, std::enable_if_t<is_string_v<ObjectType>>>
                : type_base<ObjectType, predefined_reader<ObjectType, reader_enum::string, From>, From>
            {
            };
        }
        template <typename ObjectType, typename Reader = use_default_tag, typename From = std::string_view>
        struct type : detail::type_base<ObjectType, Reader, From>
        {
        };
        template <typename ...Types>
        struct type_list
        {
            using types = std::tuple<typename Types::object_type...>;
            using readers = std::tuple<typename Types::reader_type...>;
            static constexpr std::size_t size = sizeof...(Types);
        };
        template <typename TypeList, typename From = std::string_view>
        class literal_reader
        {
            using type_list = TypeList;
            using from = From;
            using types = typename type_list::types;
            using readers = typename type_list::readers;
            template <std::size_t Index>
            using elem_t = std::tuple_element_t<Index, types>;
        public:
            template <std::size_t N>
            using type_at = std::tuple_element_t<N, types>;
            template <typename ...PassReader>
            constexpr explicit literal_reader(PassReader ...pass_reader)
                :readers_(lzhlib::tuple::create_tuple<readers>(std::move(pass_reader)...))
            {
            }
            template <std::size_t Index>
            constexpr type_at<Index> read(From from)
            {
                return std::get<Index>(readers_)(from);
            }
        private:
            readers readers_;
        };
    }
}
#endif //LZHLIB_READER_LITERAL_READER_HPP
