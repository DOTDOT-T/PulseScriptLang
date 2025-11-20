#pragma once

#include <unordered_map>

enum class TokenType {
    Identifier,
    Number,         // "3"
    FloatingNumber, // "3.14"
    StringLiteral,  // a string like -> "this is a string"
    Function,       // custom function made by user

    Let,            // var declaraction
    Turn,           // var modification
    Arrow,          // "->"
    LParen,         // "("
    RParen,         // ")"
    Comma,          // ","
    LBrace,         // "{"
    RBrace,         // "}"

    Plus,           // "+"
    Minus,          // "-"
    Star,           // "*"
    Slash,          // "/"
    
    EndOfFile
};

using Value = std::variant<int, float, std::string>;

//base struct for parser
struct ASTNode
{
    virtual ~ASTNode() = default;
};

struct Scope
{
    std::unordered_map<std::string, Value> variables;
};

struct ASTExpression : ASTNode
{
    virtual ~ASTExpression() {};
    virtual Value Evaluate(const Scope& scope) const = 0;
};

