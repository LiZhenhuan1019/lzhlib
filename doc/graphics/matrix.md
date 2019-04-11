# lzhlib.graphics.matrix

## Source

[include/graphics/matrix.hpp](../../include/graphics/matrix.hpp)

## basic_matrix

`basic_matrix` is a type representing matrix whose size is known at compile time.
Making size of matrix known at compile time brings to us the following benefits:

1. Compile-time optimization for multiplication with matrix chain is possible, which has be implemented.

   For example, assume that we have three matrices: m0 with size 30 * 35, m1 with size 35 * 15, and m2 with size 15 * 5.

   Then we want to compute the multiplication of m0, m1, and m2.

   If we first multiply m0 and m1, then multiply the result to m2, we have to multiply the underlying number 30 * 35 * 15 + 30 * 15 * 5 = 15825 times.

   However if we first multiply m1 and m2, then multiply m0 to the result, we only have to do multiplication 35 * 15 * 5 + 30 * 35 * 5 = 7875 times.

   `basic_matrix` will use dynamic-programming to choose the best associativity, at compile time.

2. Other compile time optimizations such as loop unrolling and auto vectorization is also possible.


### Usage
You can create a matrix and initialize it using list initialization like this:

```
basic_matrix<int, 2, 3> m0{
        0, 1, 2,
        3, 4, 5};
```

The first template argument indicates the underlying number.

The second and third template arguments indicate the number of row(height) and column(width) of the matrix, respecitvely.

You can use `multiply_matrices` with arbitrary number of matrices to do matrix multiplication, with the benefit of compile time optimization of associativity.

```
multiply_matrices(m0, m1, m2, m3);
```

Of course you can use `*` to multiply them, with the same semantics.

```
basic_matrix<int 2, 3> m0;
basic_matrix<int 3, 1> m1;
basic_matrix<int 1, 4> m2;
basic_matrix<int, 2, 4> result = m0 * m1 * m2;
```

However, you must use `>> eval` at the end of multiplication chains to perform the multiplication if you want to use type deduction.

```
auto result = m0 * m1 * m2 >> eval; // OK, the multiplication is performed.
```

Otherwise, there is only a compile time matrix chain generated.
```
auto chain = m0 * m1 * m2; // Wrong! No multiplication is performed.
auto result = chain >> eval; // OK. You can use >> eval
    // to perform the computation.
```

Also notice the danger of dangling reference when any matrix is a temporary.
```
auto result = m0 * basic_matrix<int, 3, 2>{} >> eval; // OK.
    // The computation is preformed and no reference to the temporary any more.
auto chain = m0 * basic_matrix<int, 3, 2>{}; // Wrong!
    // chain has a dangling reference to the temporary.
```

### Comparision

##### [PX4/Matrix](https://github.com/PX4/Matrix)

[PX4/Matrix](https://github.com/PX4/Matrix) has more feature than `lzhlib.graphics.matrix` for now.

However, because `lzhlib.graphics.matrix` has optimization for multiplication of matrix chain, it runs faster than [PX4/Matrix](https://github.com/PX4/Matrix) and in some circumstance it's 4 times faster.

See below for the benchmark result between `lzhlib.graphics.matrix` and [PX4/Matrix](https://github.com/PX4/Matrix), by multiplying 5 matrices with sizes varying from 5 to 35.


```
Run on (8 X 4200 MHz CPU s)
CPU Caches:
  L1 Data 32K (x4)
  L1 Instruction 32K (x4)
  L2 Unified 256K (x4)
  L3 Unified 8192K (x1)
----------------------------------------------------------------------
Benchmark                               Time           CPU Iterations
----------------------------------------------------------------------
benchmark_lzhlib_matrix              8522 ns       8517 ns      82426
benchmark_lzhlib_matrix              8438 ns       8435 ns      82426
benchmark_lzhlib_matrix              8429 ns       8427 ns      82426
benchmark_lzhlib_matrix              8432 ns       8430 ns      82426
benchmark_lzhlib_matrix              8438 ns       8433 ns      82426
benchmark_lzhlib_matrix_mean         8452 ns       8448 ns      82426
benchmark_lzhlib_matrix_median       8438 ns       8433 ns      82426
benchmark_lzhlib_matrix_stddev         39 ns         38 ns      82426
benchmark_px4_matrix                17927 ns      17920 ns      39387
benchmark_px4_matrix                17897 ns      17891 ns      39387
benchmark_px4_matrix                17887 ns      17881 ns      39387
benchmark_px4_matrix                17886 ns      17881 ns      39387
benchmark_px4_matrix                17911 ns      17899 ns      39387
benchmark_px4_matrix_mean           17902 ns      17895 ns      39387
benchmark_px4_matrix_median         17897 ns      17891 ns      39387
benchmark_px4_matrix_stddev            17 ns         16 ns      39387
```


