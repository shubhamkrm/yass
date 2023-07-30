cmake_minimum_required(VERSION 3.24)
project(Yass VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_subdirectory(src)

# Testing
enable_testing()
add_subdirectory(tst)

# Packaging for end-users
include(GNUInstallDirs)
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE.md")
set(CPACK_PACKAGE_VERSION_MAJOR "${yass_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${yass_VERSION_MINOR}")
set(CPACK_SOURCE_GENERATOR "TGZ")
include(CPack)

