add_executable(yass yass_main.cc)

include(FetchContent)
# TOML++
FetchContent_Declare(
    tomlplusplus
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
    GIT_TAG        v3.3.0
)
# FmtLib
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG        10.0.0
)
FetchContent_MakeAvailable(tomlplusplus fmt)

set(BUILD_SHARED_LIBS OFF)

add_library(md4c-html vendor/md4c/entity.c vendor/md4c/md4c-html.c vendor/md4c/md4c.c)
add_library(yasscpp atom.cc config.cc fileutils.cc  parser.cc processor.cc
    theme.cc tokenizer.cc)
target_link_libraries(yasscpp PUBLIC md4c-html tomlplusplus::tomlplusplus
    fmt::fmt)
target_link_libraries(yass PUBLIC yasscpp)
install(TARGETS yass)
