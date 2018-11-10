#ifndef LZHLIB_TEST_DYNAMIC_READER_HPP
#define LZHLIB_TEST_DYNAMIC_READER_HPP
#include <cassert>
#include <lzhlib/reader/dynamic_reader.hpp>
namespace lzhlib::test::reader::dynamic_reader
{
    namespace detail
    {
        struct not_default_destructible
        {
            explicit not_default_destructible(int i)
                : i(i)
            {}
            int i;
            friend bool operator==(not_default_destructible const &lhs, not_default_destructible const &rhs)
            {
                return lhs.i == rhs.i;
            }
        };
        struct read_from_string_view
        {
            not_default_destructible operator()(std::string_view str)
            {
                using namespace std::literals;
                int result{};
                auto[ptr, error] = std::from_chars(str.data(), str.data() + str.size(), result);
                if (ptr == str.data())
                    throw std::logic_error("expect integer, got "s + str.data());
                return not_default_destructible{result};
            }
        };
    }
    inline void test()
    {
        using lzhlib::reader::type_list;
        using lzhlib::reader::type;
        using lzhlib::tuple::arg;
        using reader = lzhlib::reader::dynamic_reader<type_list<type<int>, type<detail::not_default_destructible, detail::read_from_string_view>>>;
        reader r0;
        r0.read(1, "42");
        assert(!r0.get<0>().has_value());
        assert(r0.get<1>() == detail::not_default_destructible(42));
    };
}
#endif //LZHLIB_TEST_DYNAMIC_READER_HPP
