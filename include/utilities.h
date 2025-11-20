#pragma once

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

//base struct for parser

struct ASTNode
{
    virtual ~ASTNode() = default;
};

struct ASTExpression : ASTNode
{
};