TESTS = tokenizer_test parser_test
check_PROGRAMS = tokenizer_test parser_test
tokenizer_test_SOURCES = tokenizer_test.cc ../src/tokenizer.cc
tokenizer_test_LDADD = -lsystem

parser_test_SOURCES = parser_test.cc ../src/parser.cc ../src/tokenizer.cc ../src/vendor/md4c/md4c-html.c ../src/vendor/md4c/md4c.c ../src/vendor/md4c/entity.c
parser_test_LDADD = -lsystem

AM_CXXFLAGS = -std=c++20
