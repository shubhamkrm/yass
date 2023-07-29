add_executable(yass yass_main.cc)

include(FetchContent)
# TOML++
FetchContent_Declare(
    tomlplusplus
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
    GIT_TAG        v3.3.0
)
FetchContent_MakeAvailable(tomlplusplus)

set(BUILD_SHARED_LIBS OFF)

add_library(md4c-html vendor/md4c/entity.c vendor/md4c/md4c-html.c vendor/md4c/md4c.c)
add_library(yasscpp atom.cc config.cc parser.cc processor.cc theme.cc
    tokenizer.cc)
target_link_libraries(yasscpp PUBLIC md4c-html tomlplusplus::tomlplusplus
    ctemplate)
target_link_libraries(yass PUBLIC yasscpp)
install(TARGETS yass)

# Packaging for end-users
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${yass_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${yass_VERSION_MINOR}")
set(CPACK_SOURCE_GENERATOR "TGZ")
include(CPack)

