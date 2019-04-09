#ifndef LZHLIB_GRAPH_TEST_GRAPH_HPP
#define LZHLIB_GRAPH_TEST_GRAPH_HPP

#include <lzhlib/graphics/matrix.hpp>
#include <cassert>
namespace lzhlib::test
{
    namespace graphics
    {
        void test_resolve_mult_matrix()
        {
            {
                constexpr mult_matrix_size<6> matrix_size{30, 35, 15};
                constexpr auto s = resolve_mult_matrix(matrix_size);
                // the solution is m0 * (m1 * m2).
                static_assert(s.solution[0][2] == 0);
                static_assert(s.solution[1][2] == 1);
            }
            {
                constexpr mult_matrix_size<6> matrix_size{30, 35, 15, 5, 10, 20, 25};
                constexpr auto s = resolve_mult_matrix(matrix_size);
                // the solution is (m0 * (m1 * m2)) * ((m3 * m4) * m5).
                static_assert(s.solution[0][5] == 2);
                static_assert(s.solution[0][2] == 0);
                static_assert(s.solution[1][2] == 1);
                static_assert(s.solution[3][5] == 4);
                static_assert(s.solution[3][4] == 3);
            }
        }
        void test()
        {
            test_resolve_mult_matrix();
        }
    }
}
#endif //LZHLIB_GRAPH_TEST_GRAPH_HPP
