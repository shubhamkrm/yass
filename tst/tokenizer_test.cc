#define BOOST_TEST_MODULE Tokenizer Test
#include <boost/test/included/unit_test.hpp>
#include <string>

#include "../src/tokenizer.h"

namespace yass {
    
namespace testing {

BOOST_AUTO_TEST_CASE(happy_case_test) {
  std::string input = R"input(---
title: "A fancy title"
description: "It's a nice example"
---

Hola!
)input";
  Tokenizer2 tokenizer(input);
  auto tokens = tokenizer.Tokenize();
  for (auto &token : tokens) {
    std::cout << token.type << " : " << token.content
              << "; Line : " << token.line << std::endl;
  }

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

  BOOST_CHECK_EQUAL_COLLECTIONS(tokens.begin(), tokens.end(),
          expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(test_empty_frontmatter) {
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


}

}  // namespace testing
} /* yass  */ 
