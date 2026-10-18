#include <benchmark/benchmark.h>

#include "lzhlib/graphics/matrix.hpp"

namespace
{
    using lzhlib::basic_matrix;
    using lzhlib::multiply_matrices;

    template <typename MatrixT>
    MatrixT make_matrix(typename MatrixT::value_type first)
    {
        MatrixT result{};
        auto value = first;
        for (auto &row : result.value)
        {
            for (auto &element : row)
            {
                element = value;
                value = static_cast<typename MatrixT::value_type>(value + 1);
            }
        }
        return result;
    }

    // Five matrices of growing dimensions, the shape used in the README
    // comparison against PX4/Matrix: (5x10)(10x15)(15x25)(25x30)(30x35).
    void matrix_chain_of_five(benchmark::State &state)
    {
        auto m0 = make_matrix<basic_matrix<double, 5, 10>>(1.0);
        auto m1 = make_matrix<basic_matrix<double, 10, 15>>(2.0);
        auto m2 = make_matrix<basic_matrix<double, 15, 25>>(3.0);
        auto m3 = make_matrix<basic_matrix<double, 25, 30>>(4.0);
        auto m4 = make_matrix<basic_matrix<double, 30, 35>>(5.0);

        for (auto _ : state)
        {
            auto product = multiply_matrices(m0, m1, m2, m3, m4);
            benchmark::DoNotOptimize(product);
        }
    }

    // A single 35x35 * 35x35 product: the dense inner loop case.
    void matrix_square_35(benchmark::State &state)
    {
        auto lhs = make_matrix<basic_matrix<double, 35, 35>>(1.0);
        auto rhs = make_matrix<basic_matrix<double, 35, 35>>(2.0);

        for (auto _ : state)
        {
            auto product = multiply_matrices(lhs, rhs);
            benchmark::DoNotOptimize(product);
        }
    }
}

BENCHMARK(matrix_chain_of_five)->Unit(benchmark::kMicrosecond);
BENCHMARK(matrix_square_35)->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();
