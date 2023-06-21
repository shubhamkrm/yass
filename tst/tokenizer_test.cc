#define BOOST_TEST_MODULE Tokenizer Test
#include <boost/test/included/unit_test.hpp>
#include "../src/tokenizer.h"

#include <string>

namespace testing {
using ::yass::Token;
using ::yass::TokenType;
using ::yass::Tokenize;

BOOST_AUTO_TEST_CASE(happy_case_test)
{
    std::string input = R"input(---
title: "A fancy title"
description: "It's a nice example"
---

Hola!
)input";
    std::vector<Token> tokens = Tokenize(input);
    BOOST_TEST(tokens.size() == 9);
    BOOST_CHECK(tokens[0].type == TokenType::kFMDelimiter);
    BOOST_CHECK(tokens[1].type == TokenType::kFMKey);
    BOOST_CHECK(tokens[2].type == TokenType::kFMSeparator);
    BOOST_CHECK(tokens[3].type == TokenType::kFMValue);
    BOOST_CHECK(tokens[4].type == TokenType::kFMKey);
    BOOST_CHECK(tokens[5].type == TokenType::kFMSeparator);
    BOOST_CHECK(tokens[6].type == TokenType::kFMValue);
    BOOST_CHECK(tokens[7].type == TokenType::kFMDelimiter);
    BOOST_CHECK(tokens[8].type == TokenType::kContent);
    BOOST_TEST(tokens[8].content == "\nHola!\n");
}

BOOST_AUTO_TEST_CASE(test_empty_frontmatter)
{
    std::string input = R"input(---
---

Hola!
)input";
    yass::TokenList tokens = Tokenize(input);
    BOOST_TEST(tokens.size() == 3);
    BOOST_CHECK(tokens[0].type == TokenType::kFMDelimiter);
    BOOST_CHECK(tokens[1].type == TokenType::kFMDelimiter);
    BOOST_CHECK(tokens[2].type == TokenType::kContent);
    BOOST_TEST(tokens[2].content == "\nHola!\n");
}


} /* testing */
