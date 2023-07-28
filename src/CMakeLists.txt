add_executable(yass yass_main.cc)
add_library(md4c vendor/md4c/entity.c vendor/md4c/md4c-html.c vendor/md4c/md4c.c)
add_library(yasscpp atom.cc config.cc parser.cc processor.cc theme.cc
    tokenizer.cc)
target_link_libraries(yasscpp PUBLIC md4c)
target_link_libraries(yasscpp PUBLIC ctemplate)
target_link_libraries(yass PUBLIC yasscpp)
install(TARGETS yass)
