cmake_minimum_required(VERSION 3.22)

project(CURL_LiB_APP LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(EXECUTABLE_NAME "Executable") 
set(EXE_LIB_NAME "EXE_LIB")
set(MHD_EXECUTABLE "MICROHTTPD_EXE")
set(WIN_MHD_EXE "WIN_HTTPD_EXE")
set(LINUX_MHD_EXE "LINUX_HTTPD_EXE")
set(LIBRARY_NAME "LibAbc")
if (MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()
option(WIN_64 "Build Windows" ON) # Enabled by default
option(WIN_32 "Build Windows" OFF) # Enabled by default
option(ENABLE_TESTING "Enable Unit Testing" OFF)

if(WIN_64)
    add_compile_definitions(-D_WIN64)
endif(WIN_64)
if(WIN_32)
    add_compile_definitions(-D_WIN32)
endif(WIN_32)

set(MICROHTTPD_LIBRARIES "${CMAKE_SOURCE_DIR}/thirdparty/libmicrohttpd-1.0.1/w32_x86_64/Debug-static/libmicrohttpd_d.lib")
set(MICROHTTPD_INCLUDES "${CMAKE_SOURCE_DIR}/thirdparty/libmicrohttpd-1.0.1/w32_x86_64/Debug-static/")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/module/")
find_package(CURL CONFIG REQUIRED)
find_package(MICROHTTPD REQUIRED)

message("MICROHTTPD_FOUND: " ${MICROHTTPD_FOUND})
include(FetchContent)

add_subdirectory(app)
add_subdirectory(httpd)
add_subdirectory(src)
if(ENABLE_TESTING)
  set(TEST_MAIN "unit_tests")
  set(MOCK_LIB "MOCK_LIB")
  include(CTest)
  enable_testing()
  add_subdirectory(tests)
endif()