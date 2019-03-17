# lzhlib

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

### Running the tests

I actually wrote some test, built under build directory with path `test/PACKAGE_NAME/test_PACKAGE_NAME`.

I did not use any test tool. Contributions of improving testing are welcome.

## Authors

* [Li Zhenhuan](https://gitlab.com/LiZhenhuan1019)

## License

This project is under the MIT license - see the [LICENSE](LICENSE) file for details.
