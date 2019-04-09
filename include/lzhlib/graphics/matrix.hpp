#ifndef LZHLIB_MATRIX_HPP
#define LZHLIB_MATRIX_HPP

#include <cstdlib>
#include <type_traits>
#include <array>

namespace lzhlib
{
    template <std::size_t NumberOfMatrix>
    struct mult_matrix_size
    {
        constexpr static std::size_t number_of_matrix = NumberOfMatrix;
        constexpr static std::size_t size = NumberOfMatrix + 1;
        size_t size_of_matrices[size];  // matrix of index i has height of size_of_matrices[i] and width of size_of_matrices[i+1]
    };
    template <std::size_t NumberOfMatrix>
    struct mult_matrix_solution
    {
        constexpr static std::size_t number_of_matrix = NumberOfMatrix;
        constexpr static std::size_t size = NumberOfMatrix;
        size_t solution[size][size]; // k = solution[i][j], with k indicating that sub matrix chain from matrix i to matrix j should be split at matrix k.
    };
    template <std::size_t S>
    constexpr mult_matrix_solution<S> resolve_mult_matrix(mult_matrix_size<S> matrices)
    {
        size_t cost[S][S]{};
        mult_matrix_solution<S> result{};
        for (size_t i = 0; i < S; ++i)
        {
            cost[i][i] = 0;
            result.solution[i][i] = 0;
        }
        for (size_t l = 2; l <= S; ++l) // l is the length of matrix chain.
        {
            for (size_t i = 0; i <= S - l; ++i) // i is the start point of matrix chain.
            {
                size_t j = i + l - 1; // j is the end point of matrix chain.
                cost[i][j] = std::numeric_limits<size_t>::max();
                result.solution[i][j] = j - 1;  // default solution is to multiply matrix i..j-1 and multiply the result to matrix j.
                for (size_t k = i; k < j; ++k)
                {
                    size_t current_cost = cost[i][k] + cost[k + 1][j] +
                                          matrices.size_of_matrices[i] * matrices.size_of_matrices[k + 1] * matrices.size_of_matrices[j + 1];
                    if (current_cost < cost[i][j])
                    {
                        cost[i][j] = current_cost;
                        result.solution[i][j] = k; // now the solution is to multiply matrix i..k and multiply matrix k+1..j and multiply the results.
                    }
                }
            }
        }
        return result;
    }


    template <typename T, size_t Height, size_t Width>
    struct basic_matrix
    {
        constexpr static size_t height = Height;
        constexpr static size_t rows = height;
        constexpr static size_t width = Width;
        constexpr static size_t columns = width;
        using value_type = T;
        std::array<std::array<value_type, width>, height> value;
    };
    template <typename T>
    struct is_matrix : std::false_type
    {
    };
    template <typename T, size_t Height, size_t Width>
    struct is_matrix<basic_matrix<T, Height, Width>> : std::true_type
    {
    };
    template <typename T, size_t Height, size_t Width>
    bool operator==(basic_matrix<T, Height, Width> const &lhs, basic_matrix<T, Height, Width> const &rhs)
    {
        return lhs.value == rhs.value;
    }

    template <typename ...Matrices>
    using last_type = std::tuple_element_t<sizeof...(Matrices) - 1, std::tuple<Matrices...>>;
    template <typename ...Matrices>
    using first_type = std::tuple_element_t<0, std::tuple<Matrices...>>;
    template <typename ...Matrices>
    using result_of_t = basic_matrix<std::common_type_t<typename Matrices::value_type...>, first_type<Matrices...>::height, last_type<Matrices...>::width>;

    template <typename Tuple, size_t ...Indices>
    constexpr bool check_consistencies_helper(std::index_sequence<Indices...>)
    {
        return ((std::tuple_element_t<Indices, Tuple>::width ==
                 std::tuple_element_t<Indices + 1, Tuple>::height)&&...);
    }

    template <typename ...Matrices>
    constexpr bool check_consistencies()
    {
        using tuple_type = std::tuple<Matrices...>;
        return check_consistencies_helper<tuple_type>(std::make_index_sequence<sizeof...(Matrices) - 1>{});
    }

    template <typename ...Matrices>
    result_of_t<Matrices...> multiply_matrices(Matrices const &...matrices)
    {
        static_assert(std::conjunction_v<std::bool_constant<(sizeof...(Matrices) > 1)>, is_matrix<Matrices>...>);
        static_assert(check_consistencies<Matrices...>());

//        mult_matrix_solution<sizeof...(Matrices)> solution = resolve_mult_matrix()
    }
}


#endif //LZHLIB_MATRIX_HPP
