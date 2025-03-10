# CMake C++ Project for Gtest and Curl

The aim of this project is to perform REST requests against a HTTP server. It will utilize the gtest and gmock features of GoogleTest framework for testing.

## Prerequisites

To use this project, you’ll need:

- A compatible operating system (e.g. Linux, macOS, Windows).
- A compatible C++ compiler that supports at least C++14.
- CMake and a compatible build tool for building the project.
  - Compatible build tools include Make, Ninja, MSVC and others   
  - see CMake Generators for more information.
- vcpkg installed to fetch the curl C library

## Notes

The msvc build target names can be identified by the vcxproj files.
- Ex. unit_tests.vcxproj and LibAbc.vcxproj

## Usage Example for msvc Build

1. ```VCPKG_DEFAULT_TRIPLET = 'x64-windows'```
2. ```mkdir build```
3. ```cmake --preset msvc -S . -B ./build -DENABLE_TESTING=ON```
4. ```cmake --build ./build```
 - ```cd build```
 - ```ctest```
5. ```cmake --build .\build --target install --config Debug -v```
6. ```cmake --build .\build --target LibAbc --config Debug -v```


### References

1. [CMake VCPKG Guide](https://github.com/paboulos/sdl2_vcpkg_cmake_pt1) 
2. [GoogleTest](https://google.github.io/googletest/quickstart-cmake.html)
3. [Curl](https://everything.curl.dev/libcurl/cplusplus.html)
