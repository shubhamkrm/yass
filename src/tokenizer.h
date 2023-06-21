#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <map>
#include <ostream>
#include <string>
#include <vector>

namespace yass
{
enum class TokenType {
    kContent, kFMDelimiter, kFMKey, kFMValue, kFMSeparator
};

inline std::ostream& operator<<(std::ostream& out, const TokenType token_type) {
    std::map<TokenType, std::string> string_map({
            {TokenType::kContent, "CONTENT"},
            {TokenType::kFMDelimiter, "DELIMITER"},
            {TokenType::kFMKey, "KEY"},
            {TokenType::kFMValue, "VALUE"},
            {TokenType::kFMSeparator, "SEPARATOR"}
            });
    return out << string_map[token_type];
}


struct Token {
    TokenType type;
    std::string content;
};

using TokenList = std::vector<Token>;

TokenList Tokenize(std::string &input);
  
} /*  yass */ 

#endif /* TOKENIZER_H */
