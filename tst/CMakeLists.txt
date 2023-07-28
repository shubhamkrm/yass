cmake_minimum_required(VERSION 3.25)
find_package(Boost COMPONENTS filesystem system unit_test_framework REQUIRED)

include_directories(${yass_SOURCE_DIR}/src
    ${Boost_INCLUDE_DIRS}
)
add_definitions(-DBOOST_TEST_DYN_LINK)

# Build unit tests
add_executable(tokenizer_test tokenizer_test.cc)
target_link_libraries(tokenizer_test
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    yasscpp)

add_executable(parser_test parser_test.cc)
target_link_libraries(parser_test
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    yasscpp)

