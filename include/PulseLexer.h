#pragma once
#include <string>
#include <vector>
#include <cctype>
#include <variant>

#include "utilities.h"

using Value = std::variant<int, std::string>;

struct Token
{
    TokenType type;
    std::string text;
};

class PulseLexer
{
public:
    PulseLexer(const std::string &src);

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
