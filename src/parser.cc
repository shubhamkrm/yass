#include <any>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "parser.h"
#include "structs.h"
#include "tokenizer.h"
#include "vendor/md4c/md4c-html.h"

namespace yass {
namespace {

class RDParser {
 public:
  RDParser(TokenList tokens) : index(0), tokens(tokens) {}
  std::unique_ptr<Page> Parse();
  ~RDParser(){};

 private:
  /* data */
  int index;
  TokenList tokens;

  /* internal methods */
  std::unique_ptr<Page> page();
  std::map<std::string, std::any> frontmatter();
  std::pair<std::string, std::any> keyval();
  std::string content();
  std::string expect(TokenType);
  bool accept(TokenType);
};

bool RDParser::accept(TokenType type) {
  Token &token = tokens.at(index);
  if (token.type == type) {
    index++;
    return true;
  }
  return false;
}

std::string RDParser::expect(TokenType type) {
  Token &token = tokens.at(index);
  if (token.type == type) {
    index++;
    return std::move(token.content);
  }
  std::cerr << "Parse Error: Unexpected symbol : " << tokens.at(index).content
            << std::endl;
  std::cerr << "Expected symbol : " << type << ", Found " << token.type;
  exit(1);
}

std::pair<std::string, std::any> RDParser::keyval() {
  std::string key = expect(TokenType::kFMWord);
  expect(TokenType::kFMSeparator);

  std::string string_val = expect(TokenType::kFMWord);
  std::vector<std::string> list_val{string_val};

  while (accept(TokenType::kListSeparator)) {
    list_val.push_back(expect(TokenType::kFMWord));
  }
  expect(TokenType::kNewLine);
  if (list_val.size() == 1) {
    return {key, string_val};
  }
  return {key, list_val};
}

std::map<std::string, std::any> RDParser::frontmatter() {
  expect(TokenType::kFMDelimiter);
  std::map<std::string, std::any> frontmatter;
  while (tokens.at(index).type == TokenType::kFMWord) {
    frontmatter.insert(keyval());
  }
  expect(TokenType::kFMDelimiter);
  return frontmatter;
}

std::string RDParser::content() { return expect(TokenType::kContent); }

std::unique_ptr<Page> RDParser::page() {
  auto metadata = frontmatter();
  std::unique_ptr<Page> page = std::make_unique<Page>();
  page->content = content();
  if (metadata.find("type") != metadata.end()) {
    if (metadata["type"].type() != typeid(std::string)) {
      std::cerr << "Parse Error: 'type' should always be a string.\n";
      exit(1);
    } else {
      page->type = std::any_cast<std::string>(metadata["type"]);
    }
  }
  page->metadata = std::move(metadata);
  return page;
}

std::unique_ptr<Page> RDParser::Parse() { return page(); }

void process_output(const MD_CHAR *text, MD_SIZE size, void *data) {
  ((std::string *)data)->append(text, 0, size);
}

} /* namespace */
/*
 * Grammar:
 * page = {frontmatter} content
 * frontmatter = delimiter {entries} delimiter
 * entries = word ":" value "\n"
 */
std::unique_ptr<Page> Parser::Parse(std::string &input) {
  TokenList tokens = Tokenizer2(input).Tokenize();
  RDParser parser(tokens);
  std::unique_ptr<Page> page = parser.Parse();
  auto markdown_content = std::make_unique<std::string>();
  md_html(page->content.c_str(), page->content.size(), &process_output,
          markdown_content.get(), MD_DIALECT_COMMONMARK, 0);
  page->content = *markdown_content;
  return page;
}
}  // namespace yass
