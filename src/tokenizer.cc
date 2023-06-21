#include "tokenizer.h"

#include <ctype.h>
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <vector>

namespace yass
{
namespace {

class Tokenizer
{
public:
    virtual TokenList Tokenize(const std::string&) const = 0;
    virtual ~Tokenizer() {}
};

class StateMachineTokenizer : public Tokenizer
{
public:
    struct State {
        std::function<State(char, TokenList&)> transition;
    }; 

    StateMachineTokenizer ();
    TokenList Tokenize(const std::string &input) const;
    virtual ~StateMachineTokenizer () {}

private:
    State states[10];
};

/*
 * Some limitations:
 * 1. Any value cannot have a `"` char in it. TODO: Need to implement escaping.
 * 2. Tokenizer rejects the input if it does not start with the delimiter.
 *    This logic should be in the parser, tokenizer should be more permissive.
 * 3: No internationalization support.
 */
StateMachineTokenizer::StateMachineTokenizer() {
    states[0].transition = [=](char ch, TokenList &tokens) {
        Token &token = tokens.back();
        tokens.back().type = TokenType::kFMDelimiter;
        if (ch == '-') return states[0];
        if (ch == '\n') {
            tokens.emplace_back();
            return states[7];
        }
        std::cerr << "Parse Error: First line should be a delimiter.\n";
        exit(1);
    };

    states[1].transition = [=](char ch, TokenList &tokens) {
        Token &token = tokens.back();
        token.type = TokenType::kFMKey;
        if (isspace(ch)) return states[1];
        if (isalnum(ch)) {
            token.content += ch;
            return states[2];
        }
        std::cerr << "Error parsing frontmatter." 
            "Key can only contain alphanumeric values.\n";
        std::cerr << "Last parsed token : " << tokens.back().content << '\n';
        exit(1);
    };

    states[2].transition = [=](char ch, TokenList &tokens) {
        Token &token = tokens.back();
        token.type = TokenType::kFMKey;
        if (isalnum(ch)) {
            token.content += ch;
            return states[2];
        }
        if (isspace(ch)) return states[3];
        if (ch == ':') {
            tokens.emplace_back();
            tokens.back().type = TokenType::kFMSeparator;
            tokens.emplace_back();
            return states[4];
        }
        std::cerr << "Error parsing frontmatter." 
            "Key can only contain alphanumeric values.\n";
        std::cerr << "Last parsed token : " << tokens.back().content << '\n';
        exit(1);
    };

    states[3].transition = [=](char ch, TokenList &tokens) {
        Token &token = tokens.back();
        token.type = TokenType::kFMKey;
        if (isspace(ch)) return states[3];
        if (ch == ':') {
            tokens.emplace_back();
            tokens.back().type = TokenType::kFMSeparator;
            tokens.emplace_back();
            return states[4];
        }
        std::cerr << "Error parsing frontmatter. Key cannot contain spaces.\n";
        std::cerr << "Last parsed token : " << tokens.back().content << '\n';
        exit(1);
    };

    states[4].transition = [=](char ch, TokenList &tokens) {
        Token &token = tokens.back();
        token.type = TokenType::kFMValue;
        if (isspace(ch)) return states[4];
        if (ch == '"') {
            return states[5];
        }
        std::cerr << "Error parsing frontmatter: Did you forget a quote?\n";
        std::cerr << "Last parsed token : " << tokens.back().content << '\n';
        exit(1);
    };
    states[5].transition = [=](char ch, TokenList &tokens) {
        Token &token = tokens.back();
        token.type = TokenType::kFMValue;
        if (ch == '"') {
            tokens.emplace_back();
            return states[6];
        }
        token.content += ch;
        return states[5];
    };
    states[6].transition = [=](char ch, TokenList &tokens) {
        if (ch == ' ') return states[6];
        if (ch == '\n') return states[7];
        std::cerr << "Error parsing frontmatter:"
            "All keys should start at a new line.\n";
        std::cerr << "Last parsed token : " << tokens.back().content << '\n';
        exit(1);
    };
    states[7].transition = [=](char ch, TokenList &tokens) {
        Token &token = tokens.back();
        if (ch == '-') {
            token.type = TokenType::kFMDelimiter;
            return states[8];
        }
        if (isalnum(ch)) {
            token.type = TokenType::kFMKey;
            token.content += ch;
            return states[1];
        }
        if (isspace(ch)) {
            return states[1];
        }
        std::cerr << "Error parsing frontmatter." 
            "Key can only contain alphanumeric values.\n";
        std::cerr << "Last parsed token : " << tokens.back().content << '\n';
        exit(1);
    };
    states[8].transition = [=](char ch, TokenList &tokens) {
        if (ch == '-') return states[8];
        if (ch == '\n') {
            tokens.emplace_back();
            return states[9];
        }
        std::cerr << "Error parsing frontmatter." 
            "Key can only contain alphanumeric values.\n";
        std::cerr << "Last parsed token : " << tokens.back().content << '\n';
        exit(1);
    };
    states[9].transition = [=](char ch, TokenList &tokens) {
        Token &token = tokens.back();
        token.type = TokenType::kContent;
        token.content += ch;
        return states[9];
    };
}

TokenList StateMachineTokenizer::Tokenize(const std::string &input)
    const {
    TokenList tokens;
    tokens.emplace_back();
    State state = states[0];
    for (char c: input) {
        state = state.transition(c, tokens);
    }
    return tokens;
}

} /* namespace */

TokenList Tokenize(std::string &input) {
    auto tokenizer = std::make_unique<StateMachineTokenizer>();
    return tokenizer->Tokenize(input);
}
} /* yass */ 
