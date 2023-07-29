#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <any>
#include <string>
#include <typeindex>

#include "../src/parser.h"

namespace yass {
namespace {

TEST(ParserTest, SimpleParserTest) {
  std::string input = R"input(---
title: "A fancy title"
description: "It's a nice example"
---

# Hola!
)input";
  Parser parser;
  auto page = parser.Parse(input);

  EXPECT_EQ(page->metadata.size(), 2);
  EXPECT_EQ(page->content, "<h1>Hola!</h1>\n");

  ASSERT_EQ(page->metadata["title"].type(), typeid(std::string));
  ASSERT_EQ(page->metadata["description"].type(), typeid(std::string));

  std::string title = std::any_cast<std::string>(page->metadata["title"]);
  std::string description =
      std::any_cast<std::string>(page->metadata["description"]);
  EXPECT_EQ(title, "A fancy title");
  EXPECT_EQ(description, "It's a nice example");
}

TEST(ParserTest, EmptyFrontmatterTest) {
  std::string input = R"input(---
---

Hola!
)input";
  Parser parser;
  auto page = parser.Parse(input);
  EXPECT_EQ(page->metadata.size(), 0);
  EXPECT_EQ(page->content, "<p>Hola!</p>\n");
}

TEST(ParserTest, ParseListAttributes) {
  std::string input = R"input(---
categories: cat1, cat2, cat3
---

# Hola!
)input";
  Parser parser;
  auto page = parser.Parse(input);

  // Metadata key `type` is always present.
  EXPECT_EQ(page->metadata.size(), 1);
  EXPECT_EQ(page->content, "<h1>Hola!</h1>\n");

  ASSERT_EQ(page->metadata["categories"].type(),
            typeid(std::vector<std::string>));

  auto categories =
      std::any_cast<std::vector<std::string>>(page->metadata["categories"]);
  EXPECT_THAT(categories, ::testing::ElementsAre("cat1", "cat2", "cat3"));
}

}  // namespace
}  // namespace yass
