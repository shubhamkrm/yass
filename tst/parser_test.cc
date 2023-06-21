#define BOOST_TEST_MODULE Tokenizer Test
#include <boost/test/included/unit_test.hpp>
#include "../src/parser.h"

#include <string>

namespace testing {
using ::yass::Page;
using ::yass::Parser;

BOOST_AUTO_TEST_CASE(happy_case_test)
{
    std::string input = R"input(---
title: "A fancy title"
description: "It's a nice example"
---

# Hola!
)input";
    Parser parser;
    auto page = parser.Parse(input);
    BOOST_TEST(page->metadata.size() == 2);
    BOOST_TEST(page->metadata["title"] == "A fancy title");
    BOOST_TEST(page->metadata["description"] == "It's a nice example");
    BOOST_TEST(page->content == "<h1>Hola!\n</h1>\n");
}

BOOST_AUTO_TEST_CASE(test_empty_frontmatter)
{
    std::string input = R"input(---
---

Hola!
)input";
    Parser parser;
    auto page = parser.Parse(input);
    BOOST_TEST(page->metadata.size() == 0);
    BOOST_TEST(page->content == "<p>Hola!\n</p>\n");
}


} /* testing */
