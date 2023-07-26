#ifndef TOKENIZER2_H
#define TOKENIZER2_H

#include <ostream>
#include <string>
#include <string_view>
#include <vector>

namespace yass {

enum class TokenType {
  kEof,  
  kContent,
  kFMDelimiter,
  kFMWord,
  kFMSeparator,
  kListSeparator,
  kNewLine,
};

inline std::ostream& operator<<(std::ostream& o, const TokenType token_type) {
    switch(token_type) {
    case TokenType::kEof: return o << "EOF";
    case TokenType::kContent: return o << "CONTENT";
    case TokenType::kFMDelimiter: return o << "DELIMITER";
    case TokenType::kFMWord: return o << "WORD";
    case TokenType::kFMSeparator: return o << "SEPARATOR";
    case TokenType::kListSeparator: return o << "LIST_SEPARATOR";
    case TokenType::kNewLine: return o << "NEW_LINE";
    }
    return o << "INVALID";
}
struct Token {
  TokenType type;
  std::string content;
  size_t line;
  auto operator<=>(const Token&) const = default;
};

inline std::ostream& operator<<(std::ostream& o, const Token& token) {
    o << token.type << " : " << token.content
        << "; at line " << token.line << std::endl;
    return o;
} 

using TokenList = std::vector<Token>;

class Tokenizer2 {
 private:
  size_t current = 0;
  size_t start = 0;
  size_t line = 1;
  int8_t num_delimiters = 0;
  std::string_view source;
  TokenList tokens;

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
  Tokenizer2(std::string_view source) : source(source) {}
  TokenList Tokenize();
  ~Tokenizer2() {};
};
}  // namespace yass

#endif /* TOKENIZER2_H */
