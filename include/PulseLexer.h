#pragma once
#include <string>
#include <vector>
#include <cctype>
#include <variant>

using Value = std::variant<int, std::string>;

enum class TokenType {
    Identifier,
    Number,
    StringLiteral,
    Function,   // custom function made by user

    Let,        // var declaraction
    Arrow,      // "->"
    LParen,     // "("
    RParen,     // ")"
    Comma,      // ","
    LBrace,
    RBrace,

    Plus,
    Minus,
    Star,
    Slash,
    
    EndOfFile
};

struct Token {
    TokenType type;
    std::string text;
};

class PulseLexer {
public:
    PulseLexer(const std::string& src);

    Token Next();
    Token Peek();
    bool End() const;

private:
    void SkipSpaces();
    Token MakeIdentifierOrKeyword();
    Token MakeNumber();
    Token MakeString();

private:
    std::string source;
    size_t pos = 0;
};
