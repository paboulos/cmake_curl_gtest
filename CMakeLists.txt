cmake_minimum_required(VERSION 3.22)

project(CURL_LiB_APP LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(EXECUTABLE_NAME Executable)
set(EXE_LIB_NAME "EXE_LIB")
set(LIBRARY_NAME LibAbc)
if (MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()
option(WIN_64 "Build Windows" ON) # Enabled by default
option(ENABLE_TESTING "Enable Unit Testing" OFF)

if(WIN_64)
    add_compile_definitions(-D_WIN64)
endif(WIN_64)

find_package(CURL CONFIG REQUIRED)
include(FetchContent)

add_subdirectory(app)
add_subdirectory(src)
if(ENABLE_TESTING)
  set(TEST_MAIN "unit_tests")
  include(CTest)
  enable_testing()
  add_subdirectory(tests)
endif()