# lzhlib

[![CI](https://github.com/LiZhenhuan1019/lzhlib/actions/workflows/ci.yml/badge.svg)](https://github.com/LiZhenhuan1019/lzhlib/actions/workflows/ci.yml)

## What is it?

lzhlib is an open source C++ library.
It's still in development and may change dramatically over time.

**No reliability is guaranteed**; use it at your own risk.

## Getting started

These instructions will guide you to get a copy of the project and install on your machine.

### Prerequisites

* cmake
* a modern C++ compiler that satisfies C++17 standard.

### Installing

#### Arch Linux

Go to the directory you wish to build the source and run the following commands:

    git clone git@gitlab.com:lzh1019/arch_pkg/lzhlib.git
    cd lzhlib/
    makepkg -si

Then the library is built and installed.

#### Other Linux Distributions

Go to the directory you wish to build the source and run the following commands:

    git clone git@gitlab.com:LiZhenhuan1019/lzhlib.git
    cd lzhlib/

Build the Debug version:

    cmake -DCMAKE_BUILD_TYPE=Debug -S . -B "build-debug"
    cmake --build "build-debug/"

Install the Debug version:
    
    cd "build-debug"
    sudo make DESTDIR="/usr" install

Modify `DESTDIR` to for example "/usr/local" if you want.

Then return to parent directory:

    cd ..

Build the Release version:

    cmake -DCMAKE_BUILD_TYPE=Release -S . -B "build-release"
    cmake --build "build-release/"

Install the Release version:

    cd "build-release"
    sudo make DESTDIR="/usr" install

Modify `DESTDIR` to for example "/usr/local" if you want.

### Running the unit tests

The unit tests are written with [GoogleTest](https://github.com/google/googletest). CMake looks
for a system installation first and fetches GoogleTest v1.14.0 through `FetchContent` when it is
missing (the first configure then needs network access):

    cmake -DCMAKE_BUILD_TYPE=Debug -DLZHLIB_BUILD_GTEST=ON -S . -B build-debug
    cmake --build build-debug --target lzhlib_unit_tests
    ctest --test-dir build-debug --output-on-failure

The older hand written checks under `test/<package>/` are still built by default and kept for
reference; they do not depend on GoogleTest.

### Running the micro benchmarks

    cmake -DCMAKE_BUILD_TYPE=Release -DLZHLIB_BUILD_BENCHMARKS=ON -S . -B build-release
    cmake --build build-release --target matrix_benchmark
    ./build-release/benchmark/matrix_benchmark

### Continuous integration

`.github/workflows/ci.yml` runs on every push and pull request:

 * GCC and Clang, C++17 and C++20, Debug, plus one optimized Release configuration (`NDEBUG`);
 * ASan + UBSan and TSan builds, so memory errors, UB and data races fail the build.

Both sanitizer jobs start the tests through `setarch $(uname -m) -R`: the sanitizer runtimes
do not cope with the 32 bit ASLR entropy some kernels use (ASan dies with `DEADLYSIGNAL`,
TSan with `unexpected memory mapping`). Use the same prefix when running these builds locally
on WSL or on a distribution that ships such a kernel.

## Design notes

### `indexed_list`: links stored as indices, nodes stored contiguously

`indexed_list<T>` keeps the nodes in a `std::vector` and links them through `std::size_t`
indices (`prev` / `next`) into a doubly linked circular list. `head` is owned by the caller and
`null_index` denotes an empty list, so one node can be moved between lists without allocating.

* Compared with `std::list`: nodes are contiguous (cache friendly) and there is no per node
  pointer overhead, only two indices.
* Compared with `std::vector`: insert and erase in the middle are O(1), because only indices are
  rewritten and no element is shifted.
* The price: indices are not type safe, a stale index cannot be detected, and a node can only
  belong to one list at a time.

### `value_repo`: an object pool with value semantics

`value_repo<T>` builds two `indexed_list`s (used / unused) on top of that and stores the value in
`std::optional<T>`:

* `assign_object(args...)` pops a free slot in O(1) and constructs the value in place;
* `remove_object(id)` destroys the value, `return_object(id)` keeps it alive but returns the slot;
* the storage grows by 1.5× when the free list is empty, the same policy as `std::vector`;
* iteration walks the used list only, so it is O(live) rather than O(capacity).

The invariant is `used == true` implies `value.has_value() == true`; addresses are not stable
across growth.

### `graphics::matrix`: expression templates and compile time associativity

`basic_matrix<T, Height, Width>` encodes the dimensions in the type, so `operator*` returns a
lazy expression node instead of a materialized matrix. `>> eval` materializes it, and assigning
an expression to a `basic_matrix` materializes it as well.

The associativity of a chain is chosen at compile time with the standard dynamic programming
algorithm over the dimension sequence. For `(30x35)(35x15)(15x5)` the naive left to right order
needs 30·35·15 + 30·15·5 = 15825 scalar multiplications, while
`(30x35)((35x15)(15x5))` needs 35·15·5 + 30·35·5 = 7875, i.e. 50 % fewer.

`benchmark/matrix_benchmark.cpp` (Google Benchmark) reproduces the comparison against
PX4/Matrix: about 2.1× faster on a chain of five matrices. The remaining gap to a hand written
kernel is SIMD, register blocking and cache blocking, none of which are implemented on purpose -
the goal of the library is the compile time part.

## Documentation

 *  [graphics.matrix](doc/graphics/matrix.md)

    A matrix library.

## Authors

* [Li Zhenhuan](https://gitlab.com/LiZhenhuan1019)

## License

This project is under the MIT license - see the [LICENSE](LICENSE) file for details.
