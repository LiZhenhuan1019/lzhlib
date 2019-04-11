#ifndef LZHLIB_READER_DYNAMIC_READER_HPP
#define LZHLIB_READER_DYNAMIC_READER_HPP
#include <string_view>
#include <array>
#include <optional>
#include <functional>
#include <lzhlib/reader/literal_reader.hpp>
namespace lzhlib
{
    namespace reader
    {
        template <typename TypeList, typename From = std::string_view>
        class dynamic_reader
        {
            using type_list = TypeList;
            using from = From;
            using types = typename type_list::types;
            using readers = typename type_list::readers;
            static constexpr std::size_t size = std::tuple_size_v<types>;
            template <std::size_t Index>
            using elem_t = std::tuple_element_t<Index, types>;
            template <std::size_t ...Indices>
            static std::tuple<std::optional<elem_t<Indices>>...> tuple_of_optional_of_element_helper(std::index_sequence<Indices...>);
            using variable_tuple_t = decltype(tuple_of_optional_of_element_helper(std::make_index_sequence<size>{}));

            using reader_t = lzhlib::reader::literal_reader<TypeList>;
        public:
            void read(std::size_t index, from str)
            {
                if(index >= assignments.size())
                    throw std::out_of_range("argument 'index' in function 'read' of class 'lzhlib::reader::dynamic_reader' is out of range.");
                assignments[index](str);
            }
            template <std::size_t Index>
            std::optional<elem_t<Index>> &get()
            {
                return std::get<Index>(variables);
            }
            template <std::size_t Index>
            std::optional<elem_t<Index>> const &get() const
            {
                return std::get<Index>(variables);
            }
            template <std::size_t Index>
            std::optional<elem_t<Index>> const &cget() const
            {
                return std::get<Index>(variables);
            }
        private:
            template <std::size_t Index>
            struct assignment
            {
                explicit assignment(dynamic_reader &reader)
                    : reader(reader)
                {}
                void operator()(from str) const
                {
                    std::get<Index>(reader.variables).emplace(reader.reader.template read<Index>(str));
                }
                dynamic_reader &reader;
            };
            template <std::size_t Index>
            friend
            class dynamic_reader::assignment;
            using assignments_t = std::array<std::function<void(from)>, size>;
            template <std::size_t ...Indices>
            assignments_t create_assignments(std::index_sequence<Indices...>)
            {
                return {assignment<Indices>(*this)...};
            }
            variable_tuple_t variables;
            reader_t reader;
            assignments_t assignments = create_assignments(std::make_index_sequence<size>{});
        };
    }
}
#endif //LZHLIB_READER_DYNAMIC_READER_HPP
