#include "parser.h"

#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "structs.h"
#include "tokenizer.h"
#include "vendor/md4c/md4c-html.h"

namespace yass {
namespace {

class RDParser
{
public:
    RDParser (TokenList tokens) : index(0), tokens(tokens) {}
    std::unique_ptr<Page> Parse();
    ~RDParser () {};

private:
    /* data */
    int index;
    TokenList tokens;

    /* internal methods */
    std::unique_ptr<Page> page();
    std::map<std::string, std::string> frontmatter();
    std::pair<std::string, std::string> keyval();
    std::string content();
    std::string expect(TokenType);
};   


std::string RDParser::expect(TokenType type) {
    Token &token = tokens.at(index);
    if (token.type == type) {
        index++;
        return std::move(token.content);
    }
    std::cerr << "Parse Error: Unexpected symbol : " <<
        tokens.at(index).content << std::endl;
    std::cerr << "Expected symbol : " << type << ", Found " << token.type;
    exit(1);
}

std::pair<std::string, std::string> RDParser::keyval() {
    std::string key = expect(TokenType::kFMWord);
    expect(TokenType::kFMSeparator);
    std::string val = expect(TokenType::kFMWord);
    expect(TokenType::kNewLine);
    return {key, val};
}

std::map<std::string, std::string> RDParser::frontmatter() {
    expect(TokenType::kFMDelimiter);
    std::map<std::string, std::string> frontmatter;
    while (tokens.at(index).type == TokenType::kFMWord) {
        frontmatter.insert(keyval());
    }
    expect(TokenType::kFMDelimiter);
    return frontmatter;
}

std::string RDParser::content() {
    return expect(TokenType::kContent);
}

std::unique_ptr<Page> RDParser::page() {
    auto metadata = frontmatter();
    std::unique_ptr<Page> page = std::make_unique<Page>();
    page->content = content();
    page->type = metadata["type"];
    page->metadata = std::move(metadata);
    return page;
}

std::unique_ptr<Page> RDParser::Parse() {
    return page();
}

void process_output(const MD_CHAR* text, MD_SIZE size, void *data) {
    ((std::string*)data)->append(text, 0, size);
}

} /* namespace */
/*
 * Grammar:
 * page = {frontmatter} content
 * frontmatter = delimiter {entries} delimiter
 * entries = word ":" word "\n"
 */
std::unique_ptr<Page> Parser::Parse(std::string &input) {
    TokenList tokens = Tokenizer2(input).Tokenize();
    RDParser parser(tokens);
    std::unique_ptr<Page> page = parser.Parse();
    auto markdown_content = std::make_unique<std::string>();
    md_html(
            page->content.c_str(), page->content.size(), &process_output,
            markdown_content.get(),
            MD_DIALECT_COMMONMARK, 0);
    page->content = *markdown_content;
    return page;
} 
} /* yass */ 
