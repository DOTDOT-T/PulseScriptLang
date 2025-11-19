#include "PulseInterpreter.h"
#include <iostream>
#include <stdexcept>

PulseInterpreter::PulseInterpreter() 
{
    // Exemple : fonction log
    RegisterFunction("log", [this](const std::vector<Value>& args) -> Value 
    {
        for (const auto& v : args) 
        {
            std::visit([](auto&& val)
            { std::cout << val << " "; }, v);
        }
        std::cout << std::endl;
        return 0;
    });
    RegisterFunction("AddInt", [this](const std::vector<Value>& args) -> Value
    {
        int result = 0;
        for (const auto& v : args)
        {
            result += std::get<int>(v);
        }
        return result;
    });
}

void PulseInterpreter::RegisterFunction(const std::string& name, std::function<Value(const std::vector<Value>&)> func) 
{
    nativeFunctions[name] = func;
}

void PulseInterpreter::Execute(const std::vector<std::unique_ptr<ASTStatement>>& stmts) 
{
    for (auto& stmt : stmts) 
    {
        if (auto letStmt = dynamic_cast<ASTLetStatement*>(stmt->content.get())) 
        {
            Value val = EvalExpression(letStmt->value.get());
            scope.variables[letStmt->varName] = val;
        } 
        else if (auto call = dynamic_cast<ASTFunctionCall*>(stmt->content.get())) 
        {
            std::vector<Value> args;
            for (auto& argExpr : call->args) 
            {
                args.push_back(EvalExpression(argExpr.get()));
            }
            auto it = nativeFunctions.find(call->name);
            if (it != nativeFunctions.end()) 
            {
                it->second(args);
            } else 
            {
                throw std::runtime_error("Unknown function: " + call->name);
            }
        } else 
        {
            throw std::runtime_error("Unknown statement type");
        }
    }
}

Value PulseInterpreter::EvalExpression(const ASTExpression* expr) 
{
    if (!expr) throw std::runtime_error("Null expression");

    if (auto n = dynamic_cast<const ASTNumber*>(expr)) 
    {
        return n->value;
    }
    if (auto s = dynamic_cast<const ASTString*>(expr)) 
    {
        return s->value;
    }
    if (auto id = dynamic_cast<const ASTIdentifier*>(expr)) 
    {
        auto it = scope.variables.find(id->name);
        if (it != scope.variables.end()) return it->second;
        throw std::runtime_error("Undefined variable: " + id->name);
    }
    if (auto call = dynamic_cast<const ASTFunctionCall*>(expr)) 
    {
        std::vector<Value> args;
        for (auto& arg : call->args) 
        {
            args.push_back(EvalExpression(arg.get()));
        }
        auto it = nativeFunctions.find(call->name);
        if (it != nativeFunctions.end()) return it->second(args);
        throw std::runtime_error("Unknown function: " + call->name);
    }
    if (auto bin = dynamic_cast<const ASTBinaryOp*>(expr)) 
    {
        int left = std::get<int>(EvalExpression(bin->left.get()));
        int right = std::get<int>(EvalExpression(bin->right.get()));

        switch (bin->op) {
            case '+': return left + right;
            case '-': return left - right;
            case '*': return left * right;
            case '/': return left / right;
        }
    }


    throw std::runtime_error("Unknown expression type");
}
