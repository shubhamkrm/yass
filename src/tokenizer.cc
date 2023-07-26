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

bool Tokenizer2::isAtEnd() const { return (current >= source.length()); }

char Tokenizer2::advance() { return source.at(current++); }

char Tokenizer2::peek() const {
  if (isAtEnd()) return 0;
  return source.at(current);
}

void Tokenizer2::addToken(const TokenType type,
                          const std::string_view content = "") {
  tokens.push_back(
      {.type = type, .content = std::string(content), .line = line});
}

bool Tokenizer2::match(const char expected) {
  if (isAtEnd() || (peek() != expected)) return false;
  current++;
  return true;
}

void Tokenizer2::content() {
  addToken(TokenType::kContent, source.substr(start));
  while (!isAtEnd()) {
    if (peek() == '\n') line++;
    advance();
  }
  current = source.length();
}

void Tokenizer2::delimiter() {
  while (peek() == '-' && !isAtEnd()) {
    advance();
  }
  if (peek() != '\n') {
    handleError(
        line, "Lines containing delimiters cannot contain any other character");
  }
  addToken(TokenType::kFMDelimiter);
  // Consume new line.
  advance();
  line++;
  num_delimiters++;
  if (num_delimiters == 2) {
    start = current;
    content();
  }
}

void Tokenizer2::quotedWord() {
  while (peek() != '"' && !isAtEnd()) {
    if (peek() == '\n') line++;
    advance();
  }
  if (isAtEnd()) {
    handleError(line, "Encountered end of file while parsing quoted word.");
  }
  // Need to consume the closing " as well.
  advance();
  addToken(TokenType::kFMWord, source.substr(start + 1, current - start - 2));
}

void Tokenizer2::word() {
  while (!isWordSeparator(peek()) && !isAtEnd()) {
    if (peek() == '\n') line++;
    advance();
  }
  addToken(TokenType::kFMWord, source.substr(start, current - start));
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
      line++;
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
    start = current;
    scanToken();
  }

  addToken(TokenType::kEof);
  return tokens;
}

}  // namespace yass
