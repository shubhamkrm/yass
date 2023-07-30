#ifndef TOKENIZER2_H
#define TOKENIZER2_H

#include <ostream>
#include <string>
#include <string_view>
#include <vector>

namespace yass {

// Type of tokens in the lexical grammar for a page.
enum class TokenType {
  kEof,
  kContent,
  kFMDelimiter,
  kFMWord,
  kFMSeparator,
  kListSeparator,
  kNewLine,
};

// Provides an implemention of << operator for token type for ease in printing
// for debugging.
inline std::ostream& operator<<(std::ostream& o, const TokenType token_type) {
  switch (token_type) {
    case TokenType::kEof:
      return o << "EOF";
    case TokenType::kContent:
      return o << "CONTENT";
    case TokenType::kFMDelimiter:
      return o << "DELIMITER";
    case TokenType::kFMWord:
      return o << "WORD";
    case TokenType::kFMSeparator:
      return o << "SEPARATOR";
    case TokenType::kListSeparator:
      return o << "LIST_SEPARATOR";
    case TokenType::kNewLine:
      return o << "NEW_LINE";
  }
  return o << "INVALID";
}

// Represents a parsed lexical token.
struct Token {
  // Type of the parsed token.
  TokenType type;

  // The parsed lexeme as a string.
  std::string content;

  // Line number at which the lexeme occurs in the input. It is useful for
  // debugging and for meaningful error messages.
  size_t line;

  // Provides default implementation for the equality operator.
  bool operator==(const Token&) const = default;
};

// Implementation for << operator for token for printing. Can be used for
// debugging and for printing token information in error messages.
inline std::ostream& operator<<(std::ostream& o, const Token& token) {
  o << token.type << " : " << token.content << "; at line " << token.line
    << std::endl;
  return o;
}

using TokenList = std::vector<Token>;

// Reads a raw source file, and returns a list of parsed tokens. Source code
// between two delimiters("---") is considered as the frontmatter, and
// tokenized as key value pairs. Everything after the second delimiter is parsed
// as a single kContent lexeme.
class Tokenizer2 {
 private:
  // Index of the current character being parsed.
  size_t current_ = 0;
  // Index of the first character in the token currently being parsed.
  size_t start_ = 0;
  // Current line being parsed.
  size_t line_ = 1;
  // Tracks the number of delimiters parsed. Everything after two delimiters is
  // considered as part of the content.
  int8_t num_delimiters_ = 0;

  // Stores a view of the source.
  std::string_view source_;

  // Contains a list of parsed tokens.
  TokenList tokens_;

  char advance();
  bool isAtEnd() const;
  bool match(char expected);
  char peek() const;
  void scanToken();
  void delimiter();
  void quotedWord();
  void word();
  void content();
  void addToken(const TokenType type, const std::string_view content);

 public:
  // Constructor which accepts the input source as input.
  Tokenizer2(std::string_view source) : source_(source) {}

  // Does the actual tokenization part.
  TokenList Tokenize();

  // Default destructor.
  ~Tokenizer2(){};
};
}  // namespace yass

#endif /* TOKENIZER2_H */
