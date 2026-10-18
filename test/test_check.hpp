#ifndef LZHLIB_TEST_CHECK_HPP
#define LZHLIB_TEST_CHECK_HPP

// Shared helpers of the hand written tests in test/<package>/.
//
// Those tests used to be written with assert(), which is compiled out as soon
// as NDEBUG is defined: a Release build then reported success without
// evaluating a single check, and every variable that was only read inside an
// assert turned into a -Wunused-but-set-variable warning. LZH_TEST_CHECK is
// always evaluated, so the tests keep their meaning in every configuration,
// and a failure is reported with file and line instead of abort().

#include <cstdio>
#include <stdexcept>
#include <string>
#include <utility>

namespace lzhlib::test
{
    class check_failure : public std::runtime_error
    {
    public:
        check_failure(char const *expression, char const *file, int line)
            : std::runtime_error(std::string(file) + ":" + std::to_string(line) +
                                 ": check failed: " + expression)
        {}
    };

    inline void check(bool condition, char const *expression, char const *file, int line)
    {
        if (!condition)
            throw check_failure(expression, file, line);
    }

    // Runs one test executable and converts a failed check (or any other
    // exception thrown by the test) into the non zero exit code ctest expects.
    template <typename TestFunction>
    int run(char const *name, TestFunction &&test_function)
    {
        try
        {
            std::forward<TestFunction>(test_function)();
        }
        catch (std::exception const &e)
        {
            std::fprintf(stderr, "%s: FAILED\n%s\n", name, e.what());
            return 1;
        }
        catch (...)
        {
            std::fprintf(stderr, "%s: FAILED (unknown exception)\n", name);
            return 1;
        }
        std::printf("%s: passed\n", name);
        return 0;
    }
}

// Variadic so that the expression may contain unprotected commas, e.g.
// LZH_TEST_CHECK(result == basic_matrix<int, 3, 2>{6, 8, 9, 12, 15, 20});
#define LZH_TEST_CHECK(...)                                                             \
    ::lzhlib::test::check(static_cast<bool>(__VA_ARGS__), #__VA_ARGS__, __FILE__, __LINE__)

#endif // LZHLIB_TEST_CHECK_HPP
