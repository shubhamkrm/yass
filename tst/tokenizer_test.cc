#include <gtest/gtest.h>
#include <gtest/gtest-matchers.h>
#include <string>

#include "../src/tokenizer.h"

namespace yass {
namespace testing {

TEST(TokenizerTest, SimpleTestCase) {
  std::string input = R"input(---
title: "A fancy title"
description: "It's a nice example"
---

Hola!
)input";
  Tokenizer2 tokenizer(input);
  auto tokens = tokenizer.Tokenize();

  TokenList expected = {
      Token{TokenType::kFMDelimiter, "", 1},
      Token{TokenType::kFMWord, "title", 2},
      Token{TokenType::kFMSeparator, "", 2},
      Token{TokenType::kFMWord, "A fancy title", 2},
      Token{TokenType::kNewLine, "", 2},
      Token{TokenType::kFMWord, "description", 3},
      Token{TokenType::kFMSeparator, "", 3},
      Token{TokenType::kFMWord, "It's a nice example", 3},
      Token{TokenType::kNewLine, "", 3},
      Token{TokenType::kFMDelimiter, "", 4},
      Token{TokenType::kContent, "\nHola!\n", 5},
      Token{TokenType::kEof, "", 7},
  };

  EXPECT_EQ(tokens, expected);
}

TEST(TokenizerTest, EmptyFrontmatter) {
  std::string input = R"input(---
---

Hola!
)input";
  yass::TokenList tokens = yass::Tokenizer2(input).Tokenize();
  TokenList expected = {
      Token{TokenType::kFMDelimiter, "", 1},
      Token{TokenType::kFMDelimiter, "", 2},
      Token{TokenType::kContent, "\nHola!\n", 3},
      Token{TokenType::kEof, "", 5},
  };
  EXPECT_EQ(tokens, expected);
}

TEST(TokenizerTest, FrontmatterWithList) {
  std::string input = R"input(---
categories: cat1, cat2, cat3
---

Hola!
)input";
  yass::TokenList tokens = yass::Tokenizer2(input).Tokenize();
  TokenList expected = {
      Token{TokenType::kFMDelimiter, "", 1},
      Token{TokenType::kFMWord, "categories", 2},
      Token{TokenType::kFMSeparator, "", 2},
      Token{TokenType::kFMWord, "cat1", 2},
      Token{TokenType::kListSeparator, "", 2},
      Token{TokenType::kFMWord, "cat2", 2},
      Token{TokenType::kListSeparator, "", 2},
      Token{TokenType::kFMWord, "cat3", 2},
      Token{TokenType::kNewLine, "", 2},
      Token{TokenType::kFMDelimiter, "", 3},
      Token{TokenType::kContent, "\nHola!\n", 4},
      Token{TokenType::kEof, "", 6},
  };
  EXPECT_EQ(tokens, expected);
}

} /*  testing */     
} /* yass  */ 

