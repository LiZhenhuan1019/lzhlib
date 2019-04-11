#ifndef LZHLIB_MATRIX_HPP
#define LZHLIB_MATRIX_HPP

#include <cstdlib>
#include <type_traits>
#include <array>
#include <tuple>

namespace lzhlib
{
    // basic_matrix is a type representing matrix whose size is known at compile time.
    // Making size of matrix known at compile time brings to us the following benefits:
    // 1. Compile-time optimization for multiplication with matrix chain is possible, which has be implemented.
    //    For example, assume that we have three matrices: m0 with size 30 * 35, m1 with size 35 * 15, and m2 with size 15 * 5.
    //    Then we want to compute the multiplication of m0, m1, and m2.
    //    If we first multiply m0 and m1, then multiply the result to m2, we have to multiply the underlying number 30 * 35 * 15 + 30 * 15 * 5 = 15825 times.
    //    However if we first multiply m1 and m2, then multiply m0 to the result, we only have to do multiplication 35 * 15 * 5 + 30 * 35 * 5 = 7875 times.
    //    basic_matrix will use dynamic-programming to choose the best associativity, at compile time.
    // 2. Other compile time optimizations such as loop unrolling and auto vectorization is also possible.
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
    namespace detail
    {
        template <std::size_t NumberOfMatrix>
        struct mult_matrix_size
        {
            constexpr static std::size_t number_of_matrix = NumberOfMatrix;
            constexpr static std::size_t size = NumberOfMatrix + 1;
            size_t size_of_matrices[size];  // matrix with index i has height of size_of_matrices[i] and width of size_of_matrices[i+1]
        };
        template <std::size_t NumberOfMatrix>
        struct mult_matrix_solution
        {
            constexpr static std::size_t number_of_matrix = NumberOfMatrix;
            constexpr static std::size_t size = NumberOfMatrix;
            size_t solution[size][size]; // k = solution[i][j], with k indicating that sub matrix chain from matrix i to matrix j should be split at matrix k.
            // that is, matrix list m0, m1, ...mn should be split into m0, m1, ...mk and mk+1, mk+2, ...mn.
        };
        template <typename Tuple, std::size_t ...Indices>
        constexpr mult_matrix_size<std::tuple_size<Tuple>::value> generate_matrix_size_helper(std::index_sequence<Indices...>)
        {
            return mult_matrix_size<std::tuple_size<Tuple>::value>{{std::tuple_element_t<0, Tuple>::height, std::tuple_element_t<Indices, Tuple>::width...}};
        }
        template <typename ...Matrices>
        constexpr mult_matrix_size<sizeof...(Matrices)> generate_matrix_size()
        {
            return generate_matrix_size_helper<std::tuple<Matrices...>>(std::index_sequence_for<Matrices...>{});
        }
        // compute the optimized execution sequence of matrix multiplication at compile time using dynamic programming.
        template <typename ...Matrices>
        struct resolve_mult_matrix
        {
            constexpr static std::size_t size = sizeof...(Matrices);
            constexpr static mult_matrix_solution<size> resolve()
            {
                mult_matrix_size<size> matrices = generate_matrix_size<Matrices...>();
                size_t cost[size][size]{};
                mult_matrix_solution<size> result{};
                for (size_t i = 0; i < size; ++i)
                {
                    cost[i][i] = 0;
                    result.solution[i][i] = 0;
                }
                for (size_t l = 2; l <= size; ++l) // l is the length of matrix chain.
                {
                    for (size_t i = 0; i <= size - l; ++i) // i is the start point of matrix chain.
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
            constexpr static mult_matrix_solution<size> solution = resolve();
        };
        template <typename ...Ts>
        struct integer_matrix
        {
        };
        template <std::size_t Index, typename Seq>
        struct get
        {
        };
        template <std::size_t Index, std::size_t ...Indices>
        struct get<Index, std::index_sequence<Indices...>>
        {
            using detail_tuple = std::tuple<std::integral_constant<std::size_t, Indices>...>;
            constexpr static std::size_t value = std::tuple_element_t<Index, detail_tuple>::value;
        };
        template <std::size_t Index, typename ...Seqs>
        struct get<Index, integer_matrix<Seqs...>>
        {
            using detail_tuple = std::tuple<Seqs...>;
            using type = std::tuple_element_t<Index, detail_tuple>;
        };
        template <std::size_t Index, typename Seq>
        using get_t = typename get<Index, Seq>::type;
        template <std::size_t Index, typename Seq>
        inline constexpr auto get_v = get<Index, Seq>::value;

        // transform resolve_mult_matrix::solution from 2d array to type of integer_matrix<std::index_sequence<...>>.
        template <typename Resolver, std::size_t I, std::size_t ...Indices>
        constexpr auto array_to_type_helper(std::index_sequence<Indices...>)
        {
            return std::index_sequence<Resolver::solution.solution[I][Indices]...>{};
        }
        template <typename Resolver, std::size_t ...Indices>
        constexpr auto array_to_type_impl(std::index_sequence<Indices...>)
        {
            return integer_matrix<decltype(array_to_type_helper<Resolver, Indices>(std::make_index_sequence<Resolver::size>{}))...>{};
        }
        template <typename Resolver>
        using array_to_type = decltype(array_to_type_impl<Resolver>(std::make_index_sequence<Resolver::size>{}));
    }


    namespace detail
    {
        template <typename Tuple, std::size_t ...Indices>
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
    }
    template <typename ...Matrices>
    using last_type = std::tuple_element_t<sizeof...(Matrices) - 1, std::tuple<Matrices...>>;
    template <typename ...Matrices>
    using first_type = std::tuple_element_t<0, std::tuple<Matrices...>>;
    template <typename ...Matrices>
    using result_of_t = basic_matrix<std::common_type_t<typename Matrices::value_type...>, first_type<Matrices...>::height, last_type<Matrices...>::width>;
    template <std::size_t First, std::size_t Last, typename ...Matrices>
    struct result_between
    {
        using detail_tuple = std::tuple<Matrices...>;
        using type = basic_matrix<std::common_type_t<typename Matrices::value_type...>,
            std::tuple_element_t<First, detail_tuple>::height,
            std::tuple_element_t<Last, detail_tuple>::width>;
    };
    template <std::size_t First, std::size_t Last, typename ...Matrices>
    using result_between_t = typename result_between<First, Last, Matrices...>::type;
    namespace detail
    {

        template <typename Solution, std::size_t First, std::size_t Last, typename ...Matrices, std::enable_if_t<First == Last, int> = 0>
        auto const &multiply_matrices_helper(std::tuple<Matrices const &...> const &matrices)
        {
            return std::get<First>(matrices);
        }
        template <typename Solution, std::size_t First, std::size_t Last, typename ...Matrices, std::enable_if_t<(First < Last), int> = 0>
        result_between_t<First, Last, Matrices...> multiply_matrices_helper(std::tuple<Matrices const &...> const &matrices)
        {
            static_assert(First < Last);
            constexpr std::size_t split_pos = get_v<Last, get_t<First, Solution>>;
            auto left_result = multiply_matrices_helper<Solution, First, split_pos>(matrices);
            auto right_result = multiply_matrices_helper<Solution, split_pos + 1, Last>(matrices);
            result_between_t<First, Last, Matrices...> result;
            for (size_t i = 0; i < left_result.height; ++i)
                for (size_t j = 0; j < right_result.width; ++j)
                {
                    result.value[i][j] = left_result.value[i][0] * right_result.value[0][j];
                    for (size_t k = 1; k < left_result.width; ++k)
                        result.value[i][j] += left_result.value[i][k] * right_result.value[k][j];
                }
            return result;
        }
        template <typename ...Matrices>
        result_of_t<Matrices...> multiply_matrices_impl(std::tuple<Matrices const &...> const &matrices)
        {
            using solution_type = array_to_type<resolve_mult_matrix<Matrices...>>;
            return multiply_matrices_helper<solution_type, 0, sizeof...(Matrices) - 1>(matrices);
        }
    }

    template <typename ...Matrices>
    result_of_t<Matrices...> multiply_matrices(Matrices const &...matrices)
    {
        static_assert(std::conjunction_v<std::bool_constant<(sizeof...(Matrices) > 1)>, is_matrix<Matrices>...>);
        static_assert(detail::check_consistencies<Matrices...>());
        return detail::multiply_matrices_impl(std::tuple<Matrices const&...>{matrices...});
    }
    template <typename ...Matrices>
    result_of_t<Matrices...> multiply_matrices_tuple(std::tuple<Matrices const &...> const &matrices)
    {
        static_assert(std::conjunction_v<std::bool_constant<(sizeof...(Matrices) > 1)>, is_matrix<Matrices>...>);
        static_assert(detail::check_consistencies<Matrices...>());
        return detail::multiply_matrices_impl(matrices);
    }

    namespace detail
    {
        template <typename ...Matrices>
        struct expr_multiply
        {
            static_assert(check_consistencies<Matrices...>());
            std::tuple<Matrices const &...> matrices;

            auto evaluate() const
            {
                return multiply_matrices_tuple(matrices);
            }
        private:
        };

        struct make_expr_multiply
        {
            template <typename ...Matrices>
            auto operator()(Matrices const &...matrices) const
            {
                return expr_multiply<Matrices...>{{matrices...}};
            }
            template <typename ...Matrices, typename NewMatrix>
            auto operator()(expr_multiply<Matrices...> const &expr, NewMatrix const &new_matrix) const
            {
                return expr_multiply<Matrices...>{std::tuple_cat(expr.matrices, std::tuple<NewMatrix const &>{new_matrix})};
            }
            template <typename ...Matrices, typename NewMatrix>
            auto operator()(NewMatrix const &new_matrix, expr_multiply<Matrices...> const &expr) const
            {
                return expr_multiply<Matrices...>{std::tuple_cat(std::tuple<NewMatrix const &>{new_matrix}, expr.matrices)};
            }
            template <typename ...Matrices1, typename ...Matrices2>
            auto operator()(expr_multiply<Matrices1...> const &expr1, expr_multiply<Matrices2...> const &expr2) const
            {
                return expr_multiply<Matrices1..., Matrices2...>{std::tuple_cat(expr1, expr2)};
            }
        };

    }
    template <typename Lhs, typename Rhs, std::enable_if_t<std::is_invocable_v<detail::make_expr_multiply, Lhs, Rhs>, int> = 0>
    auto operator*(Lhs const &lhs, Rhs const &rhs)
    {
        return detail::make_expr_multiply{}(lhs, rhs);
    }

    namespace detail
    {
        enum class construct_tag
        {
            construct
        };
        struct evaluate_tag
        {
            explicit evaluate_tag(construct_tag)
            {}
        };
    }
    detail::evaluate_tag eval{detail::construct_tag::construct};
    template <typename ...Matrices>
    auto operator>>(detail::expr_multiply<Matrices...> const &expr, detail::evaluate_tag)
    {
        return expr.evaluate();
    }

}


#endif //LZHLIB_MATRIX_HPP
