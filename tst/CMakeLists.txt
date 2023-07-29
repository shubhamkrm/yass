cmake_minimum_required(VERSION 3.25)

# Fetch googletest
include(FetchContent)
FetchContent_Declare(
    googletest
    URL
    https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Build unit tests
include_directories(${yass_SOURCE_DIR}/src)

add_executable(tokenizer_test tokenizer_test.cc)
target_link_libraries(tokenizer_test yasscpp GTest::gtest_main)

add_executable(parser_test parser_test.cc)
target_link_libraries(parser_test yasscpp GTest::gtest_main GTest::gmock_main)

include(GoogleTest)
gtest_discover_tests(tokenizer_test)
gtest_discover_tests(parser_test)
