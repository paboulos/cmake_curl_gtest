FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
    GIT_SHALLOW TRUE
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)


set(TEST_SOURCES 
  "api_test.cc" 
  "run_test.cc")
set(TEST_INCLUDES "./")

add_executable(${TEST_MAIN} ${TEST_SOURCES})
target_include_directories(
    ${TEST_MAIN} 
    PUBLIC 
    ${TEST_INCLUDES}
    ${LIBRARY_NAME}
    )
target_link_libraries(
    ${TEST_MAIN}
    PUBLIC
    ${EXE_LIB_NAME}
    ${LIBRARY_NAME}
    GTest::gtest_main
    GTest::gmock
    )
# add ctest discovery
gtest_discover_tests(${TEST_MAIN})