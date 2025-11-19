#pragma once
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include "PulseLexer.h"

// AST NODES

struct ASTNode {
    virtual ~ASTNode() = default;
};

struct ASTExpression : ASTNode 
{
};

struct ASTNumber : ASTExpression 
{
    int value;
    ASTNumber(int v) : value(v) {}
};

struct ASTString : ASTExpression 
{
    std::string value;
    ASTString(const std::string& v) : value(v) {}
};

struct ASTIdentifier : ASTExpression 
{
    std::string name;
    ASTIdentifier(const std::string& n) : name(n) {}
};

struct ASTFunctionCall : ASTExpression 
{
    std::string name;
    std::vector<std::unique_ptr<ASTExpression>> args;
};

struct ASTBinaryOp : ASTExpression {
    char op; // '+', '-', '*', '/'
    std::unique_ptr<ASTExpression> left;
    std::unique_ptr<ASTExpression> right;

    ASTBinaryOp(char op,
                std::unique_ptr<ASTExpression> left,
                std::unique_ptr<ASTExpression> right)
        : op(op), left(std::move(left)), right(std::move(right)) {}
};


struct ASTLetStatement : ASTNode 
{
    std::string varName;
    std::unique_ptr<ASTExpression> value;
};


struct ASTStatement : ASTNode 
{
    // Either Let or FunctionCall or FunctionDef
    std::unique_ptr<ASTNode> content;
};
struct ASTFunctionDef : ASTNode {
    std::string name;
    std::vector<std::string> parameters; 
    std::vector<std::unique_ptr<ASTStatement>> body;
};


// PARSER

class PulseParser 
{
public:
    PulseParser(const std::vector<Token>& tokens);

    // Entry point
    std::vector<std::unique_ptr<ASTStatement>> ParseScript();

private:
    const Token& Peek(int offset = 0) const;
    const Token& Next();
    bool Match(TokenType type);
    bool Consume(TokenType type, const char* error);

    std::unique_ptr<ASTStatement> ParseStatement();
    std::unique_ptr<ASTLetStatement> ParseLet();
    std::unique_ptr<ASTExpression> ParseExpression();
    std::unique_ptr<ASTFunctionCall> ParseFunctionCall(const std::string& name);
    std::unique_ptr<ASTFunctionDef> ParseFunctionDef();
    std::unique_ptr<ASTExpression> ParsePrimary();

    std::unique_ptr<ASTExpression> ParseTerm();
    std::unique_ptr<ASTExpression> ParseFactor();

private:
    std::vector<Token> tokens;
    size_t pos = 0;
};

