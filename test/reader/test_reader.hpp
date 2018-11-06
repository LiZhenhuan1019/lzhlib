#ifndef LZHLIB_TEST_READER_HPP
#define LZHLIB_TEST_READER_HPP
#include <functional>
#include <cassert>
#include "lzhlib/reader/literal_reader.hpp"
namespace lzhlib::test
{
    namespace reader
    {
        namespace literal_reader
        {
            namespace
            {
                inline int f(std::string_view)
                {
                    return 1;
                }
            }
            inline void test()
            {
                using lzhlib::reader::type;
                using lzhlib::reader::type_list;
                using lzhlib::reader::predefined_reader;
                using lzhlib::tuple::arg;
                using reader = lzhlib::reader::literal_reader<type_list<type<int>,
                                                                        type<std::string>,
                                                                        type<int, std::function<int(std::string_view)>>>>;
                reader c(arg<2>(f));
                assert(c.read<0>("2") == 2);
                assert(c.read<1>("\"hello\"") == "hello");
                assert(c.read<2>("") == 1);
            }
        }
        inline void test()
        {
            literal_reader::test();
        }
    }
}
#endif //LZHLIB_TEST_READER_HPP
