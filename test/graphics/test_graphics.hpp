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
                using m0 = basic_matrix<float, 30, 35>;
                using m1 = basic_matrix<float, 35, 15>;
                using m2 = basic_matrix<float, 15, 5>;
                constexpr auto s = detail::resolve_mult_matrix<m0, m1, m2>::resolve();
                // the solution is m0 * (m1 * m2).
                static_assert(s.solution[0][2] == 0);
                static_assert(s.solution[1][2] == 1);
            }
            {
                using m0 = basic_matrix<float, 30, 35>;
                using m1 = basic_matrix<float, 35, 15>;
                using m2 = basic_matrix<float, 15, 5>;
                using m3 = basic_matrix<float, 5, 10>;
                using m4 = basic_matrix<float, 10, 20>;
                using m5 = basic_matrix<float, 20, 25>;
                constexpr auto s = detail::resolve_mult_matrix<m0, m1, m2, m3, m4, m5>::resolve();
                // the solution is (m0 * (m1 * m2)) * ((m3 * m4) * m5).
                static_assert(s.solution[0][5] == 2);
                static_assert(s.solution[0][2] == 0);
                static_assert(s.solution[1][2] == 1);
                static_assert(s.solution[3][5] == 4);
                static_assert(s.solution[3][4] == 3);
            }
        }
        void test_multiply_matrices()
        {
            {
                basic_matrix<int, 3, 1> m0{2,
                                           3,
                                           5};
                basic_matrix<int, 1, 2> m1{3, 4};
                auto result = multiply_matrices(m0, m1);
                assert((result == basic_matrix<int, 3, 2>{6, 8,
                                                         9, 12,
                                                         15, 20}));
            }
            {
                basic_matrix<float, 3, 3> m0
                    {1, 0, 0,
                     0, 0, -1,
                     0, 1, 0
                    };
                basic_matrix<int, 3, 1> m1
                    {0,
                     1,
                     0,
                    };
                auto result = multiply_matrices(m0, m1);
                assert((result == basic_matrix<float, 3, 1>{0, 0, 1}));
            }
            {
                basic_matrix<std::size_t, 30, 35> m0
                    {61, 19, 29, 42, 95, 9, 15, 27, 21, 14, 90, 10, 64, 77, 89, 23, 1, 70, 5, 10, 92, 76, 11, 1, 48, 49, 92, 41, 54, 35, 13, 84, 7, 67, 89,
                     76, 93, 81, 57, 73, 96, 89, 19, 25, 31, 30, 56, 89, 44, 82, 99, 79, 86, 62, 66, 93, 95, 68, 70, 78, 91, 35, 97, 10, 23, 43, 16, 1, 46, 79,
                     12, 38, 59, 24, 78, 26, 89, 78, 34, 52, 49, 90, 85, 85, 55, 54, 79, 81, 29, 71, 77, 63, 20, 38, 77, 20, 38, 21, 18, 51, 18, 22, 28, 10, 20,
                     66, 23, 63, 18, 73, 82, 35, 74, 78, 54, 20, 61, 3, 14, 29, 80, 49, 19, 50, 89, 72, 100, 34, 99, 80, 15, 96, 9, 83, 83, 53, 66, 70, 66, 25,
                     46, 78, 23, 95, 51, 19, 78, 15, 82, 20, 91, 43, 31, 88, 64, 62, 26, 16, 95, 94, 59, 53, 89, 17, 78, 94, 87, 11, 49, 41, 33, 85, 66, 99, 65,
                     86, 73, 30, 84, 83, 7, 72, 26, 85, 88, 39, 48, 32, 62, 10, 45, 23, 80, 23, 42, 66, 98, 10, 99, 39, 20, 59, 3, 78, 89, 41, 51, 74, 23, 3,
                     6, 68, 40, 26, 94, 66, 78, 31, 76, 1, 68, 50, 31, 22, 16, 19, 45, 8, 35, 90, 55, 66, 32, 33, 45, 39, 75, 64, 98, 18, 51, 59, 91, 83, 95,
                     18, 2, 55, 77, 78, 60, 31, 15, 12, 89, 25, 29, 45, 91, 61, 78, 31, 20, 97, 28, 39, 1, 1, 68, 41, 82, 45, 60, 74, 99, 15, 60, 15, 21, 58,
                     47, 53, 84, 93, 13, 35, 55, 10, 95, 4, 73, 66, 41, 48, 69, 87, 71, 75, 42, 60, 4, 40, 42, 4, 25, 1, 26, 86, 32, 81, 27, 54, 17, 38, 58,
                     70, 70, 34, 25, 57, 3, 21, 78, 91, 41, 76, 23, 20, 53, 61, 84, 98, 31, 85, 74, 16, 65, 50, 6, 90, 15, 15, 22, 80, 69, 4, 98, 28, 20, 68,
                     23, 70, 8, 80, 53, 21, 68, 56, 2, 79, 38, 93, 89, 44, 68, 44, 98, 28, 91, 83, 76, 16, 73, 72, 94, 46, 53, 3, 39, 59, 74, 43, 67, 75, 57,
                     86, 74, 69, 29, 27, 42, 95, 56, 38, 87, 16, 18, 12, 35, 84, 35, 97, 68, 7, 44, 32, 9, 92, 78, 48, 43, 2, 96, 93, 37, 57, 82, 23, 28, 4,
                     12, 27, 32, 93, 19, 61, 18, 79, 32, 37, 3, 92, 21, 94, 88, 86, 85, 6, 76, 92, 90, 54, 85, 95, 61, 37, 47, 37, 38, 19, 64, 21, 19, 95, 60,
                     80, 12, 88, 12, 26, 60, 36, 74, 65, 55, 26, 96, 39, 54, 37, 29, 29, 25, 56, 57, 76, 46, 90, 64, 37, 28, 59, 77, 60, 45, 47, 2, 56, 81, 68,
                     22, 7, 29, 73, 96, 41, 73, 70, 65, 8, 64, 34, 84, 60, 12, 36, 45, 49, 53, 99, 90, 40, 50, 19, 44, 4, 99, 69, 56, 27, 65, 7, 83, 13, 74,
                     95, 41, 72, 26, 5, 2, 71, 51, 19, 56, 4, 34, 29, 24, 68, 13, 31, 13, 58, 95, 57, 25, 63, 93, 91, 18, 93, 56, 61, 98, 22, 55, 67, 48, 52,
                     8, 26, 88, 96, 77, 51, 64, 63, 37, 8, 13, 82, 51, 14, 54, 24, 20, 4, 63, 82, 9, 66, 52, 13, 69, 45, 32, 12, 66, 57, 90, 9, 47, 28, 20,
                     13, 48, 84, 44, 72, 30, 25, 75, 81, 98, 13, 71, 39, 96, 64, 64, 36, 38, 3, 64, 81, 63, 74, 2, 8, 50, 26, 26, 38, 37, 20, 24, 33, 27, 91,
                     71, 11, 83, 63, 1, 83, 15, 40, 16, 90, 30, 66, 85, 74, 47, 78, 85, 64, 55, 39, 12, 24, 70, 88, 53, 5, 54, 99, 77, 7, 7, 21, 21, 13, 66,
                     14, 18, 32, 83, 67, 59, 42, 79, 100, 74, 29, 52, 30, 52, 82, 58, 92, 32, 32, 71, 90, 56, 99, 45, 84, 71, 31, 17, 19, 50, 14, 69, 47, 20, 74,
                     10, 9, 98, 34, 12, 86, 62, 82, 66, 28, 75, 43, 42, 30, 66, 18, 49, 15, 12, 16, 57, 14, 66, 12, 48, 31, 98, 55, 2, 93, 94, 56, 87, 70, 100,
                     34, 85, 2, 64, 84, 8, 55, 50, 5, 96, 63, 86, 63, 10, 76, 62, 45, 38, 5, 55, 58, 49, 20, 83, 97, 69, 46, 13, 46, 12, 100, 46, 16, 23, 50,
                     23, 49, 94, 38, 80, 17, 65, 30, 52, 60, 63, 40, 14, 67, 32, 20, 38, 14, 77, 67, 40, 84, 91, 54, 43, 32, 23, 45, 8, 47, 66, 13, 61, 57, 85,
                     3, 52, 95, 91, 100, 61, 76, 81, 61, 5, 99, 3, 85, 89, 30, 17, 16, 83, 22, 8, 59, 47, 38, 44, 43, 8, 45, 29, 37, 18, 74, 30, 47, 28, 31,
                     36, 24, 61, 2, 39, 63, 1, 78, 41, 97, 46, 19, 88, 88, 79, 8, 50, 51, 15, 62, 2, 16, 48, 74, 9, 16, 18, 20, 66, 45, 45, 84, 50, 12, 8,
                     11, 83, 85, 40, 55, 7, 52, 88, 18, 58, 96, 45, 81, 78, 65, 18, 57, 43, 80, 68, 62, 32, 56, 14, 3, 98, 3, 63, 83, 78, 47, 75, 39, 47, 89,
                     56, 84, 18, 18, 90, 4, 15, 6, 80, 89, 17, 77, 87, 29, 58, 67, 52, 85, 65, 90, 32, 55, 66, 4, 11, 37, 18, 88, 15, 71, 37, 27, 57, 69, 69,
                     20, 93, 87, 56, 29, 51, 47, 93, 60, 88, 55, 94, 32, 1, 4, 52, 5, 34, 89, 21, 47, 46, 33, 96, 61, 5, 22, 14, 67, 75, 68, 62, 27, 13, 63,
                     37, 98, 57, 81, 71, 74, 69, 26, 44, 10, 63, 47, 27, 15, 72, 31, 73, 4, 93, 64, 19, 77, 61, 50, 14, 40, 37, 56, 50, 63, 22, 95, 39, 14, 60,
                     18, 8, 41, 97, 66, 42, 23, 85, 32, 40, 7, 62, 92, 96, 9, 33, 91, 24, 52, 73, 29, 58, 64, 60, 41, 32, 74, 46, 3, 65, 4, 22, 21, 38, 93
                    };
                basic_matrix<std::size_t, 35, 15> m1
                    {97, 90, 14, 46, 5, 91, 54, 28, 85, 47, 77, 92, 63, 100, 51,
                     61, 30, 42, 90, 25, 21, 96, 96, 80, 69, 53, 32, 93, 91, 12,
                     18, 26, 12, 28, 98, 64, 36, 35, 42, 75, 34, 40, 51, 76, 34,
                     39, 68, 6, 45, 46, 75, 11, 17, 43, 49, 93, 35, 15, 60, 81,
                     89, 41, 31, 70, 77, 19, 62, 2, 55, 6, 72, 47, 48, 44, 44,
                     37, 92, 95, 34, 96, 74, 83, 56, 67, 16, 41, 90, 32, 47, 70,
                     32, 97, 96, 15, 85, 43, 25, 85, 35, 7, 60, 73, 1, 37, 88,
                     43, 24, 18, 10, 34, 20, 27, 29, 41, 85, 36, 40, 42, 54, 16,
                     35, 59, 64, 7, 59, 14, 32, 84, 14, 93, 79, 91, 17, 53, 12,
                     19, 68, 20, 53, 88, 48, 91, 63, 28, 48, 91, 97, 69, 4, 75,
                     25, 78, 100, 24, 80, 94, 37, 41, 21, 41, 85, 10, 36, 66, 37,
                     18, 39, 90, 24, 55, 91, 100, 39, 88, 90, 49, 63, 85, 86, 64,
                     44, 14, 77, 25, 9, 27, 94, 26, 48, 88, 98, 15, 75, 43, 37,
                     13, 88, 71, 100, 50, 31, 64, 22, 53, 44, 44, 50, 100, 43, 48,
                     73, 91, 65, 19, 53, 91, 32, 1, 69, 67, 39, 16, 45, 83, 79,
                     55, 33, 68, 90, 17, 37, 54, 91, 66, 64, 7, 65, 1, 70, 78,
                     49, 97, 12, 28, 91, 85, 47, 74, 22, 46, 13, 57, 71, 5, 58,
                     29, 91, 31, 6, 48, 67, 10, 38, 30, 81, 46, 17, 42, 91, 27,
                     22, 99, 86, 87, 52, 67, 75, 3, 7, 30, 12, 19, 5, 26, 79,
                     97, 21, 41, 92, 69, 83, 71, 58, 29, 1, 18, 34, 21, 95, 43,
                     33, 49, 32, 19, 44, 2, 55, 3, 86, 17, 94, 26, 56, 54, 26,
                     40, 46, 99, 58, 49, 75, 69, 41, 44, 1, 3, 62, 54, 6, 6,
                     25, 73, 33, 18, 5, 80, 47, 96, 74, 100, 54, 21, 77, 52, 42,
                     36, 18, 79, 49, 23, 3, 23, 75, 78, 88, 51, 78, 62, 54, 28,
                     1, 58, 29, 40, 86, 29, 31, 12, 82, 86, 76, 9, 58, 30, 35,
                     36, 52, 48, 77, 10, 88, 65, 62, 88, 29, 22, 76, 14, 69, 100,
                     30, 60, 96, 89, 59, 13, 77, 18, 28, 49, 73, 18, 60, 12, 16,
                     99, 3, 14, 2, 24, 69, 41, 78, 24, 20, 42, 51, 73, 25, 58,
                     97, 79, 53, 41, 57, 31, 77, 53, 54, 62, 17, 8, 3, 20, 49,
                     78, 6, 18, 87, 72, 99, 68, 12, 69, 43, 85, 45, 32, 46, 2,
                     89, 84, 3, 29, 91, 80, 10, 100, 79, 75, 48, 64, 15, 98, 42,
                     90, 63, 41, 51, 97, 49, 82, 39, 68, 97, 6, 91, 38, 20, 42,
                     40, 49, 89, 43, 48, 100, 98, 56, 40, 9, 100, 4, 17, 5, 9,
                     89, 66, 48, 68, 18, 22, 99, 75, 72, 20, 4, 22, 82, 11, 6,
                     64, 22, 17, 9, 35, 54, 46, 60, 46, 98, 54, 31, 53, 69, 96
                    };
                basic_matrix<std::size_t, 15, 5> m2
                    {21, 17, 45, 7, 44,
                     29, 86, 56, 70, 53,
                     25, 53, 70, 20, 19,
                     7, 50, 74, 99, 96,
                     53, 62, 28, 93, 89,
                     71, 47, 66, 11, 85,
                     55, 25, 32, 38, 44,
                     68, 2, 23, 12, 86,
                     42, 28, 97, 47, 76,
                     25, 66, 50, 66, 86,
                     90, 75, 89, 56, 77,
                     25, 90, 4, 10, 46,
                     85, 14, 52, 10, 52,
                     17, 22, 79, 93, 95,
                     31, 21, 41, 65, 18,
                    };
                basic_matrix<std::size_t, 5, 10> m3
                    {89, 55, 45, 6, 3, 45, 85, 91, 41, 5,
                     86, 30, 28, 25, 21, 45, 57, 96, 41, 21,
                     97, 16, 61, 71, 94, 88, 1, 94, 77, 36,
                     70, 21, 19, 64, 80, 22, 49, 74, 59, 64,
                     41, 39, 69, 48, 14, 39, 74, 47, 14, 6,
                    };
                basic_matrix<std::size_t, 10, 20> m4
                    {29, 84, 87, 93, 73, 65, 40, 72, 47, 27, 27, 82, 2, 3, 57, 13, 8, 12, 45, 50,
                     67, 82, 52, 61, 21, 41, 47, 51, 88, 87, 27, 63, 19, 42, 67, 28, 96, 3, 65, 69,
                     90, 99, 25, 69, 79, 94, 71, 37, 80, 35, 11, 99, 5, 91, 42, 1, 54, 33, 13, 22,
                     97, 70, 9, 62, 91, 85, 4, 75, 64, 39, 90, 93, 66, 98, 92, 28, 19, 97, 71, 60,
                     77, 37, 60, 4, 54, 41, 79, 1, 75, 24, 69, 49, 81, 81, 96, 4, 7, 2, 67, 58,
                     11, 2, 24, 9, 29, 100, 47, 82, 55, 86, 49, 37, 98, 98, 65, 59, 15, 56, 87, 40,
                     52, 87, 71, 94, 89, 13, 30, 69, 51, 21, 28, 66, 91, 72, 51, 88, 36, 32, 33, 19,
                     71, 27, 31, 100, 60, 92, 39, 17, 96, 30, 89, 12, 1, 2, 55, 41, 96, 28, 54, 36,
                     86, 17, 29, 92, 79, 98, 8, 14, 47, 27, 57, 17, 3, 73, 13, 9, 87, 42, 23, 83,
                     81, 71, 37, 21, 44, 25, 20, 19, 31, 41, 53, 30, 40, 17, 3, 98, 59, 95, 84, 24
                    };
                basic_matrix<std::size_t, 20, 25> m5
                    {69, 84, 11, 20, 77, 72, 56, 76, 20, 13, 34, 68, 83, 61, 15, 13, 97, 92, 47, 89, 40, 6, 70, 95, 42,
                     41, 50, 56, 32, 100, 72, 64, 91, 15, 90, 87, 91, 27, 24, 19, 92, 91, 58, 48, 63, 93, 84, 71, 3, 6,
                     84, 51, 64, 74, 41, 24, 83, 48, 18, 81, 79, 5, 38, 5, 75, 23, 48, 14, 48, 77, 54, 11, 92, 17, 35,
                     23, 75, 16, 51, 79, 34, 13, 64, 49, 81, 33, 83, 9, 75, 24, 99, 96, 41, 22, 53, 84, 64, 6, 4, 53,
                     10, 93, 49, 42, 90, 27, 24, 58, 43, 7, 74, 100, 27, 67, 37, 46, 78, 60, 22, 18, 65, 77, 14, 65, 33,
                     81, 13, 26, 63, 88, 90, 47, 64, 76, 34, 91, 48, 78, 79, 7, 4, 9, 72, 70, 54, 25, 4, 15, 41, 60,
                     83, 57, 33, 92, 76, 67, 85, 93, 35, 4, 7, 94, 4, 19, 17, 13, 97, 11, 71, 71, 69, 75, 25, 64, 5,
                     16, 85, 96, 62, 33, 78, 60, 19, 11, 16, 66, 8, 70, 2, 67, 99, 7, 98, 61, 5, 90, 15, 22, 83, 46,
                     54, 23, 21, 78, 33, 91, 84, 59, 24, 58, 69, 25, 76, 88, 87, 40, 39, 31, 40, 96, 27, 55, 91, 7, 74,
                     28, 33, 91, 10, 65, 72, 35, 55, 21, 43, 22, 24, 5, 35, 33, 58, 24, 52, 55, 92, 90, 34, 74, 36, 48,
                     32, 64, 68, 37, 57, 16, 100, 27, 89, 28, 11, 48, 96, 65, 30, 46, 67, 70, 69, 71, 25, 37, 47, 65, 85,
                     15, 22, 36, 87, 15, 47, 97, 57, 52, 92, 81, 16, 19, 40, 98, 43, 51, 50, 32, 10, 99, 33, 72, 98, 17,
                     5, 13, 54, 67, 32, 51, 85, 39, 74, 3, 91, 23, 47, 94, 18, 11, 7, 33, 31, 24, 30, 58, 70, 80, 17,
                     50, 33, 54, 67, 95, 39, 19, 50, 13, 85, 39, 72, 30, 44, 87, 13, 55, 18, 42, 37, 5, 56, 31, 74, 35,
                     49, 51, 99, 45, 82, 72, 33, 56, 77, 22, 72, 46, 96, 5, 97, 91, 61, 70, 1, 44, 65, 49, 63, 73, 71,
                     53, 95, 38, 17, 94, 36, 42, 89, 96, 27, 88, 9, 80, 64, 78, 64, 4, 94, 2, 80, 9, 7, 97, 95, 31,
                     57, 21, 49, 90, 35, 27, 52, 17, 75, 6, 71, 53, 97, 3, 86, 76, 73, 46, 89, 77, 48, 3, 64, 30, 48,
                     33, 68, 5, 41, 94, 87, 25, 54, 61, 29, 65, 9, 48, 70, 82, 55, 50, 21, 73, 97, 60, 39, 18, 43, 22,
                     54, 8, 30, 61, 11, 20, 4, 4, 41, 14, 78, 2, 49, 84, 95, 93, 94, 41, 78, 80, 78, 49, 100, 52, 91,
                     67, 37, 44, 44, 76, 45, 31, 77, 77, 45, 50, 17, 28, 15, 18, 17, 76, 11, 82, 19, 33, 70, 41, 65, 82
                    };
                auto result = multiply_matrices(m0, m1, m2, m3, m4, m5);
                basic_matrix<std::size_t, 30, 25> expected =
                    {6550086929746957, 7007088342808033, 6566341762570177, 7825315492818720, 9263329484295744, 7760232214973308, 7376205933281258, 7951267002161605,
                     6822789794559099, 5853944125689132, 8755431613249821, 6522729836649407, 7372855826268722, 7029253891129015, 7565355038654913, 7209114644865522,
                     8418971303748330, 7263423937787196, 6953510501587530, 8237829425049236, 7876173016189122, 6040194829187224, 7600067176782424, 7616107148130339,
                     6734076490098894,
                     9452060000649811, 10112892858885518, 9476742045551224, 11293519332224243, 13368512668715202, 11199534039006203, 10645315640311296, 11475518312422178,
                     9845346813326361, 8449487430270055, 12636144032062791, 9413947794145816, 10639219020148262, 10143499611436635, 10918520718477687, 10405173064096499,
                     12149629449818703, 10482622243259992, 10034043102606051, 11887811397652629, 11367845156876519, 8717253000587661, 10968228954932238, 10990846684788833,
                     9716946657738784,
                     7285096496771561, 7794801984560341, 7303852766540839, 8704049612114686, 10303470665819210, 8631342193813314, 8204640697718156, 8844502174453006,
                     7588495507710759, 6512124758553040, 9739159867917702, 7255507965261024, 8200271378169866, 7817964603604385, 8415172045477838, 8020146365914863,
                     9364439148723706, 8079461525633365, 7733556334016948, 9162778803678822, 8761612136496194, 6718474828660089, 8453870693866738, 8470566555514582,
                     7489407592191346,
                     8314339682494443, 8896557823565908, 8336340254491438, 9934351431440194, 11759784395159244, 9851557612495821, 9364331741736040, 10094696605993335,
                     8660640579999688, 7432913389100789, 11115885020784787, 8281127397162487, 9358935036423285, 8922794682770782, 9604782877204642, 9153938089191924,
                     10687651533376971, 9221526750712061, 8826210483534075, 10457406222711925, 10000298391354815, 7668182663509276, 9648603958549926, 9667908225660330,
                     8547378064817701,
                     8939411956556716, 9564045091781213, 8962112916169729, 10680241746739170, 12643151468841910, 10591500846029581, 10067527613176203, 10852493320302909,
                     9311824778981635, 7990084177381439, 11950168837042960, 8902682647870438, 10062619890217028, 9593816695438193, 10325854470533454, 9839795675663421,
                     11490636751544588, 9913684725400821, 9489987974501490, 11243227471476534, 10750097794949212, 8244131515565687, 10373220658162318, 10395028341098497,
                     9190337564366718,
                     7820021121856590, 8367317600452952, 7840468182679480, 9343502951657391, 11060256820943286, 9265533151482028, 8807291139465865, 9494179862498809,
                     8145675690932115, 6990672322986100, 10454614640198919, 7788501862600656, 8802399390570952, 8392075084727289, 9033366039466897, 8609448275684985,
                     10051995039798441, 8673000554227736, 8301437494973489, 9835555975038461, 9405418202047177, 7211964079929199, 9074635636857410, 9092652052781624,
                     8039308404445824,
                     7568918542052095, 8098659968676538, 7589021984216066, 9043875411618226, 10705223047576778, 8968340940858121, 8524685959325600, 9189636824399606,
                     7883729179976688, 6766799919986655, 10119137462436558, 7538787458521840, 8519391373263791, 8122260394848748, 8743497547136935, 8333291774733951,
                     9729220398568315, 8394563788565204, 8034779838790725, 9519522070717391, 9103931498683207, 6980585865326323, 8783264002330072, 8800583215745927,
                     7780851880271107,
                     7061981503400799, 7556042287086895, 7079800649729335, 8436982280983515, 9987975830490482, 8366718938099794, 7953251252068145, 8573375515644716,
                     7356559672240288, 6311953546504302, 9440669600261200, 7032934671741703, 7949629321553666, 7579158706401518, 8157352677643090, 7773822069006181,
                     9077831875048195, 7831910097304534, 7496866844342263, 8882428980310573, 8492492175723099, 6512703780334861, 8195148086981750, 8211822669370009,
                     7260308143723978,
                     7221411674959481, 7727303461405916, 7240851809983520, 8628830130872741, 10214234598027764, 8556939060376413, 8133673277734740, 8768129606497465,
                     7522015603657243, 6456390635603463, 9655062152620732, 7192947499971793, 8128564289290412, 7749829583958567, 8342566725173862, 7950992394725292,
                     9282883458978841, 8009558276348427, 7665956154735191, 9082727170779253, 8686258242181561, 6660491213712070, 8380485674116300, 8397259903663024,
                     7423587047463806,
                     7510139777948176, 8036338437735412, 7530056617822630, 8973424792005343, 10622617895278144, 8898732500720964, 8458770316965101, 9118500786690044,
                     7823140437452161, 6714000002430790, 10040948250025486, 7480251165082205, 8453936581320518, 8060083567758681, 8676018452751810, 8268556422589006,
                     9654218269230377, 8329749774334265, 7972536656661327, 9446143192043879, 9033034201588145, 6926721668635764, 8715736862027120, 8733311945905709,
                     7720611995508962,
                     8709469875472934, 9318481969519264, 8731979870654990, 10405974338899658, 12318099329292742, 10319273315982138, 9808853022700609, 10573799528245997,
                     9072026905133090, 7785377486276540, 11643266410271306, 8674160881935656, 9803489462098845, 9346625527561394, 10060557402997377, 9587679778273297,
                     11195230157051189, 9659001205496816, 9245702823589507, 10954032336200127, 10474498905373305, 8032239867218257, 10106620328507260, 10127207908528014,
                     8953642046946807,
                     7632106598355326, 8167500986465058, 7653197283013940, 9120152656929581, 10795466179618560, 9043931542150266, 8596680822760982, 9267420053855106,
                     7949641354215651, 6824590478216032, 10204921195820865, 7602628931167068, 8590667173939933, 8190269066649301, 8817538036599390, 8404577196617576,
                     9811073255890974, 8465653939323814, 8101753922758152, 9599420226116516, 9181485030857762, 7039545368674066, 8857477684222570, 8874441958547846,
                     7845504852732247,
                     8094975057943230, 8661050026185360, 8116029538233848, 9671886379179335, 11449278689870170, 9591517092911430, 9116964084645748, 9827926462196987,
                     8432044126732957, 7236130561247426, 10821951690491242, 8062267701061777, 9111965563655447, 8687509035793569, 9350980290620141, 8911048060057397,
                     10405428372439964, 8977631987111537, 8593474969781752, 10181198182550172, 9735466421154872, 7465772315259743, 9393679618255776, 9413287434657031,
                     8321920527598453,
                     7719338240409292, 8259399118659963, 7739938988608069, 9223778019359275, 10917864518804178, 9146717479794807, 8694055569316818, 9372242059777563,
                     8040179519393792, 6901439220344307, 10320172980672320, 7688660809465155, 8688467556974712, 8283421955774030, 8917196370364044, 8498923278203525,
                     9922339718874753, 8561320713665692, 8194434781730837, 9708484877430121, 9285002636481073, 7119256502301021, 8957557396812518, 8975223367463248,
                     7935422179848750,
                     7424808724338691, 7943979228075216, 7443910004536072, 8871019245605686, 10500905225364954, 8796917296703973, 8361872737132737, 9013996342746558,
                     7733830118128916, 6636994742785347, 9925738244575325, 7394603703487401, 8357333792100390, 7967688897816458, 8576401106701011, 8173733000458205,
                     9543795049157262, 8234226741613255, 7881865437585224, 9338274609476746, 8929561376183734, 6847225006499457, 8615714818385474, 8632826989970428,
                     7633006698901188,
                     7372151107167958, 7887776161758028, 7391580899670536, 8808588440094718, 10426925581938332, 8735252321459974, 8302974595703916, 8950543747775264,
                     7678871418163738, 6590482140492806, 9855824102105110, 7342605242687488, 8298056368723840, 7911407176473214, 8516099405520658, 8115917489701130,
                     9476186615776546, 8176131270849600, 7826039470079842, 9271961163496838, 8866724099692642, 6799135818360126, 8554794900997612, 8572295392016834,
                     7578688852689324,
                     6695347846739814, 7163801186076396, 6712740059348416, 7999527748308386, 9469824161276616, 7933069701856784, 7540715019594147, 8128746205678406,
                     6974341554542488, 5984947044368116, 8950969380235213, 6668300314045433, 7536708843051207, 7185653370596114, 7734303992040418, 7370450718116098,
                     8606527492600593, 7425532887400825, 7107672597746229, 8421072513401457, 8052195043883949, 6175005817556978, 7769750318249842, 7785921841601449,
                     6883107359429001,
                     7183957203813853, 7687069205703818, 7203079191735650, 8583871551860938, 10160980698736974, 8512218494544391, 8091280506905324, 8722426928401330,
                     7482939111642504, 6422686420223697, 9604702535822994, 7155455932957884, 8086295650726857, 7709415515722868, 8299010239502497, 7909600697343661,
                     9234671704207329, 7967790482289580, 7626138063173495, 9035574439825125, 8640967447179317, 6625724198312745, 8336885883756400, 8353328174504037,
                     7385099793052805,
                     7430589129115389, 7950934502848604, 7450287420394926, 8878531051358207, 10509515029010350, 8804198986680061, 8368878437299533, 9021705787630938,
                     7739736362961797, 6643184178144413, 9934293408638846, 7401002874748729, 8363729497889687, 7973714711745945, 8583682549093809, 8181446103564235,
                     9551533969803728, 8241264670656887, 7887854984247496, 9345719079475622, 8937729934346124, 6852904626258605, 8622875806578152, 8639381925236904,
                     7638675754960083,
                     8147060143325147, 8717685904887256, 8168495925445364, 9734366085666665, 11522958078550732, 9652980366814689, 9175809892047581, 9891515164875827,
                     8486337651737169, 7283381282488123, 10892156696720097, 8114456960490620, 9170512653675371, 8742955675319231, 9411356926747716, 8970085755964626,
                     10472674695133580, 9035939428544910, 8648503333594226, 10247073815943108, 9799183837493832, 7513699395311760, 9454494456896278, 9472802993579151,
                     8375376154590839,
                     7539822128450675, 8068091421041549, 7560157230061865, 9009434324246601, 10664228997331362, 8933962810592786, 8492158653434902, 9154683456445820,
                     7853371333680198, 6741411231257154, 10080729618969957, 7510156069164916, 8486551967775163, 8090802021096578, 8710176797150190, 8302316712625419,
                     9692009070830634, 8362699422208773, 8003750383271010, 9483090541857904, 9069754627121478, 6953842989046895, 8749766718255860, 8766328352611686,
                     7750803172643505,
                     7622561942692220, 8155762692328460, 7642414753314452, 9107458711408367, 10781161696887182, 9031731968656692, 8584963428947802, 9254457107755639,
                     7939998257919157, 6813918938187444, 10190499195267868, 7591817452600135, 8580233286120071, 8180520236433213, 8805331896880023, 8391205800087495,
                     9798283776403528, 8453793408525015, 8091941765146256, 9587122565435800, 9167432941744324, 7030107672673033, 8845598522073484, 8863908377300143,
                     7836242696848385,
                     7296782541785648, 7807795114806639, 7316315941791919, 8718832696470007, 10320545320542424, 8646008393536647, 8218383050745165, 8859475047369746,
                     7600341694201072, 6523751779161729, 9755609496338416, 7267926742645519, 8213165775561471, 7830334614715594, 8429377299201821, 8034022802651038,
                     9379621221106298, 8092970296138032, 7745874694101088, 9177398706654548, 8776899549447094, 6729777748431354, 8467738381870318, 8484331336379124,
                     7501030526414471,
                     7063405265196546, 7557620818150575, 7081581057543995, 8439151543844412, 9989973227957282, 8368702152218045, 7954981203567029, 8575355927247465,
                     7357642584580371, 6313898842039271, 9442806865812288, 7034696961783304, 7950810513417328, 7580165984648741, 8159121528797930, 7776038820192491,
                     9079496049457078, 7833623699094977, 7498247190206852, 8883988666962900, 8494936377181846, 6514051356832725, 8196647166471490, 8212925627514795,
                     7261537381721762,
                     6443110510120171, 6894686191800802, 6460397076712800, 7698792281670622, 9113186366536260, 7634402312032175, 7256959104128290, 7823067395533511,
                     6711332014486294, 5760599803697463, 8614479331006069, 6417666649355409, 7252430934982965, 6914308335376892, 7443305472891824, 7094575975683780,
                     8282413440502287, 7146380197935913, 6839631794991935, 8103925785301409, 7750294075567971, 5942426530345416, 7477257706162350, 7491588095424242,
                     6623531602248505,
                     8264627375274888, 8842933984267960, 8286453056782290, 9874951549633562, 11689426106148198, 9792763080302632, 9308299331671689, 10034289842814643,
                     8608586231479070, 7388415693105962, 11049176148324068, 8231624309564318, 9302757531673951, 8869355002317890, 9547256158207305, 9098527961647029,
                     10623656824959303, 9166065716676474, 8773460852348741, 10394599862144989, 9940201049706323, 7622427265096893, 9590768581416320, 9610388898284582,
                     8496104842771601,
                     7594046682115998, 8125448614275949, 7614599699270881, 9074360228068048, 10740999545417874, 8998696507028037, 8553228725542740, 9220464168618297,
                     7909587260776107, 6789798726114039, 10152981725353905, 7564182120607543, 8547426967866685, 8149117532911915, 8772813812012674, 8361030389922877,
                     9761415099823167, 8422531417662754, 8061464608859309, 9550863800491669, 9134593475286803, 7004051286870959, 8812360163609232, 8830059340735290,
                     7806452861202821,
                     7472921141549304, 7996723450560840, 7492884275122064, 8929117535000511, 10570033161120398, 8854684721256500, 8416960211540914, 9073488963997875,
                     7784285741175161, 6681040220031060, 9991387622548136, 7443359651026075, 8411942207089631, 8019980008304081, 8633141329702355, 8228012294990943,
                     9606453552132688, 8288611547633431, 7932950194645048, 9399360756168120, 8988638681188588, 6892447171381033, 8672650215723628, 8689831449531459,
                     7682244573509001,
                     7664036831054241, 8200522296382432, 7684237212865774, 9157284281630564, 10839870500468704, 9080947468639559, 8631808307377718, 9305037892629763,
                     7983142957612434, 6851465966747039, 10246284903237663, 7633355434244077, 8626815556787915, 8224778467760088, 8853409161820252, 8437740476106678,
                     9851635141453727, 8500099859915315, 8135858963059471, 9639371425540093, 9217977326543467, 7068348656585666, 8893815039810718, 8911640210960174,
                     7878816586217219,
                     6983150199883398, 7471822328567721, 7001475447551267, 8343708768206643, 9876570009242486, 8273979410793581, 7864788579596305, 8478229224128847,
                     7273703977875764, 6242790164332509, 9335791884930821, 6955135700939482, 7860143073051143, 7493693561079728, 8066626517223182, 7688169059933685,
                     8976262569726196, 7744762430294257, 7412961474468700, 8782856943456204, 8399051962028800, 6440187995500501, 8103475669959004, 8119381468367289,
                     7178785866296139,
                    };
                assert(result == expected);
            }
        }
        void test_multiply_operator()
        {
            basic_matrix<int, 2, 1> m0{2,
                                       1};
            basic_matrix<int, 1, 3> m1{3, 2, 4};

            auto result = m0 * m1 >> eval;
            basic_matrix<int, 2, 3> expected{6, 4, 8,
                                             3, 2, 4};
            assert(result == expected);
        }
        void test()
        {
            test_resolve_mult_matrix();
            test_multiply_matrices();
            test_multiply_operator();
        }
    }
}
#endif //LZHLIB_GRAPH_TEST_GRAPH_HPP