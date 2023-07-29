#include <iostream>
#include <string_view>

#include "tokenizer.h"

namespace yass {
namespace {
void handleError(int line, std::string_view message) {
  std::cerr << "Error on line " << line << ": " << message << std::endl;
}

bool isWordSeparator(char ch) { return isspace(ch) || ch == ',' || ch == ':'; }

}  // namespace

bool Tokenizer2::isAtEnd() const { return (current_ >= source_.length()); }

char Tokenizer2::advance() { return source_.at(current_++); }

char Tokenizer2::peek() const {
  if (isAtEnd()) return 0;
  return source_.at(current_);
}

void Tokenizer2::addToken(const TokenType type,
                          const std::string_view content = "") {
  tokens_.push_back(
      {.type = type, .content = std::string(content), .line = line_});
}

bool Tokenizer2::match(const char expected) {
  if (isAtEnd() || (peek() != expected)) return false;
  current_++;
  return true;
}

void Tokenizer2::content() {
  addToken(TokenType::kContent, source_.substr(start_));
  while (!isAtEnd()) {
    if (peek() == '\n') line_++;
    advance();
  }
  current_ = source_.length();
}

void Tokenizer2::delimiter() {
  while (peek() == '-' && !isAtEnd()) {
    advance();
  }
  if (peek() != '\n') {
    handleError(
        line_,
        "Lines containing delimiters cannot contain any other character");
  }
  addToken(TokenType::kFMDelimiter);
  // Consume new line.
  advance();
  line_++;
  num_delimiters_++;
  if (num_delimiters_ == 2) {
    start_ = current_;
    content();
  }
}

void Tokenizer2::quotedWord() {
  while (peek() != '"' && !isAtEnd()) {
    if (peek() == '\n') line_++;
    advance();
  }
  if (isAtEnd()) {
    handleError(line_, "Encountered end of file while parsing quoted word.");
  }
  // Need to consume the closing " as well.
  advance();
  addToken(TokenType::kFMWord,
           source_.substr(start_ + 1, current_ - start_ - 2));
}

void Tokenizer2::word() {
  while (!isWordSeparator(peek()) && !isAtEnd()) {
    if (peek() == '\n') line_++;
    advance();
  }
  addToken(TokenType::kFMWord, source_.substr(start_, current_ - start_));
}

void Tokenizer2::scanToken() {
  char c = advance();
  switch (c) {
    case '-':
      if (match('-') && match('-'))
        delimiter();
      else
        word();
      break;
    case ',':
      addToken(TokenType::kListSeparator);
      break;
    case ':':
      addToken(TokenType::kFMSeparator);
      break;
    case '\n':
      addToken(TokenType::kNewLine);
      line_++;
      break;
    case '"':
      quotedWord();
      break;
    case ' ':
    case '\f':
    case '\r':
    case '\t':
    case '\v':
      break;
    default:
      word();
      break;
  }
}

TokenList Tokenizer2::Tokenize() {
  while (!isAtEnd()) {
    start_ = current_;
    scanToken();
  }

  addToken(TokenType::kEof);
  return tokens_;
}

}  // namespace yass
