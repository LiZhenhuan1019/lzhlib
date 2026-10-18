#include <gtest/gtest.h>

#include <type_traits>
#include <utility>

#include "lzhlib/graphics/matrix.hpp"

namespace
{
    using lzhlib::basic_matrix;
    using lzhlib::eval;
    using lzhlib::is_matrix;
    using lzhlib::multiply_matrices;

    basic_matrix<int, 2, 3> sequential_2x3()
    {
        basic_matrix<int, 2, 3> m{};
        int value = 1;
        for (auto &row : m.value)
        {
            for (auto &element : row)
            {
                element = value++;
            }
        }
        return m;
    }

    basic_matrix<int, 3, 2> sequential_3x2()
    {
        basic_matrix<int, 3, 2> m{};
        int value = 7;
        for (auto &row : m.value)
        {
            for (auto &element : row)
            {
                element = value++;
            }
        }
        return m;
    }

    template <typename MatrixT>
    void fill_incrementally(MatrixT &matrix, typename MatrixT::value_type first = 1)
    {
        auto value = first;
        for (auto &row : matrix.value)
        {
            for (auto &element : row)
            {
                element = value;
                value = static_cast<typename MatrixT::value_type>(value + 1);
            }
        }
    }

    template <typename MatrixT>
    MatrixT identity()
    {
        static_assert(MatrixT::height == MatrixT::width, "identity requires a square matrix");
        MatrixT result{};
        for (std::size_t i = 0; i < MatrixT::height; ++i)
        {
            result.value[i][i] = typename MatrixT::value_type{1};
        }
        return result;
    }
}

// basic_matrix is an aggregate whose single member is the storage, so it can be
// value initialized. Default initialization of an int matrix would leave it
// uninitialized, which is why the tests always use `{}`.
TEST(matrix, value_initialized_matrix_is_all_zeros)
{
    basic_matrix<int, 2, 3> m{};
    for (auto const &row : m.value)
    {
        for (auto const &element : row)
        {
            EXPECT_EQ(element, 0);
        }
    }
}

TEST(matrix, traits_and_compile_time_dimensions)
{
    static_assert(is_matrix<basic_matrix<int, 2, 3>>::value);
    static_assert(!is_matrix<int>::value);
    static_assert(basic_matrix<int, 2, 3>::height == 2);
    static_assert(basic_matrix<int, 2, 3>::rows == 2);
    static_assert(basic_matrix<int, 2, 3>::width == 3);
    static_assert(basic_matrix<int, 2, 3>::columns == 3);
    SUCCEED();
}

TEST(matrix, multiply_two_matrices)
{
    auto product = multiply_matrices(sequential_2x3(), sequential_3x2());

    static_assert(std::is_same_v<decltype(product), basic_matrix<int, 2, 2>>);

    EXPECT_EQ(product.value[0][0], 58);
    EXPECT_EQ(product.value[0][1], 64);
    EXPECT_EQ(product.value[1][0], 139);
    EXPECT_EQ(product.value[1][1], 154);
}

TEST(matrix, operator_equals_accepts_expression)
{
    auto m0 = sequential_2x3();
    auto m1 = sequential_3x2();

    basic_matrix<int, 2, 2> assigned{};
    assigned = m0 * m1;

    EXPECT_EQ(assigned.value[0][0], 58);
    EXPECT_EQ(assigned.value[1][1], 154);
}

TEST(matrix, evaluation_is_deferred_until_eval)
{
    auto m0 = sequential_2x3();
    auto m1 = sequential_3x2();

    auto chain = m0 * m1;  // expression template, nothing is multiplied yet
    auto evaluated = chain >> eval;

    EXPECT_EQ(evaluated.value[0][0], 58);
    EXPECT_EQ(evaluated.value[0][1], 64);
    EXPECT_EQ(evaluated.value[1][0], 139);
    EXPECT_EQ(evaluated.value[1][1], 154);
}

TEST(matrix, identity_is_neutral_element)
{
    auto matrix = sequential_2x3();

    auto left = multiply_matrices(identity<basic_matrix<int, 2, 2>>(), matrix);
    auto right = multiply_matrices(matrix, identity<basic_matrix<int, 3, 3>>());

    EXPECT_TRUE(left == matrix);
    EXPECT_TRUE(right == matrix);
}

TEST(matrix, associativity_choice_keeps_result_identical)
{
    basic_matrix<int, 3, 5> m0{};
    basic_matrix<int, 5, 2> m1{};
    basic_matrix<int, 2, 4> m2{};
    basic_matrix<int, 4, 3> m3{};

    fill_incrementally(m0);
    fill_incrementally(m1, 2);
    fill_incrementally(m2, 3);
    fill_incrementally(m3, 5);

    // optimized (compile time dynamic programming) ...
    auto optimized = multiply_matrices(m0, m1, m2, m3);

    // ... must equal an explicit left fold.
    auto naive = multiply_matrices(multiply_matrices(multiply_matrices(m0, m1), m2), m3);

    EXPECT_TRUE(optimized == naive);
}

TEST(matrix, chain_of_four_all_ones)
{
    basic_matrix<int, 2, 2> m{};
    m.value[0][0] = 1;
    m.value[0][1] = 1;
    m.value[1][0] = 1;
    m.value[1][1] = 1;

    auto result = multiply_matrices(m, m, m, m);

    // [[1,1],[1,1]] ^ 4 == [[8,8],[8,8]]
    EXPECT_EQ(result.value[0][0], 8);
    EXPECT_EQ(result.value[0][1], 8);
    EXPECT_EQ(result.value[1][0], 8);
    EXPECT_EQ(result.value[1][1], 8);
}

TEST(matrix, works_with_floating_point)
{
    basic_matrix<double, 2, 2> lhs{};
    basic_matrix<double, 2, 3> rhs{};

    lhs.value = {{{1.5, 2.0}, {3.0, 4.0}}};
    rhs.value[0][0] = 1.0;
    rhs.value[1][1] = 1.0;
    rhs.value[1][2] = 2.0;

    auto result = multiply_matrices(lhs, rhs);

    EXPECT_DOUBLE_EQ(result.value[0][0], 1.5);
    EXPECT_DOUBLE_EQ(result.value[0][1], 2.0);
    EXPECT_DOUBLE_EQ(result.value[0][2], 4.0);
    EXPECT_DOUBLE_EQ(result.value[1][2], 8.0);
}
