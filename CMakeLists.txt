cmake_minimum_required(VERSION 3.25)
project(Yass VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_subdirectory(src)
add_subdirectory(tst)

# Testing
enable_testing()
add_test(NAME TokenizerTest COMMAND tokenizer_test)
add_test(NAME ParserTest COMMAND parser_test)

# Packaging for end-users
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${yass_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${yass_VERSION_MINOR}")
set(CPACK_SOURCE_GENERATOR "TGZ")
include(CPack)

