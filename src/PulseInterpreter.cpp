#include "PulseInterpreter.h"
#include <iostream>
#include <stdexcept>

PulseInterpreter::PulseInterpreter()
{
    // Exemple : fonction log
    RegisterFunction("log", [this](const std::vector<Value> &args) -> Value
                     {
        for (const auto& v : args) 
        {
            std::visit([](auto&& val)
            { std::cout << val << " "; }, v);
        }
        std::cout << std::endl;
        return 0; });
}

void PulseInterpreter::RegisterFunction(const std::string &name, std::function<Value(const std::vector<Value> &)> func)
{
    nativeFunctions[name] = func;
}

void PulseInterpreter::Execute(const std::vector<std::unique_ptr<ASTStatement>> &stmts)
{
    for (auto &stmt : stmts)
    {
        if (auto letStmt = dynamic_cast<ASTLetStatement *>(stmt->content.get()))
        {
            // exécution d'un let
            Value val = EvalExpression(letStmt->value.get());
            scope.variables[letStmt->varName] = val;
        }
        else if (auto fdef = dynamic_cast<ASTFunctionDef *>(stmt->content.get()))
        {
            userFunctions[fdef->name] = std::make_unique<ASTFunctionDef>();
            userFunctions[fdef->name]->name = fdef->name;
            userFunctions[fdef->name]->parameters = fdef->parameters;

            // déplacer le body
            for (auto &s : fdef->body)
            {
                userFunctions[fdef->name]->body.push_back(std::move(s));
            }
        }
        else if (auto call = dynamic_cast<ASTFunctionCall *>(stmt->content.get()))
        {
            // appel de fonction
            auto it = userFunctions.find(call->name);
            if (it != userFunctions.end())
            {
                std::vector<Value> args;
                for (auto &a : call->args)
                    args.push_back(EvalExpression(a.get()));
                ExecuteFunction(it->second.get(), args);
            }
            else
            {
                // fonction native
                auto itNative = nativeFunctions.find(call->name);
                if (itNative != nativeFunctions.end())
                {
                    std::vector<Value> args;
                    for (auto &a : call->args)
                        args.push_back(EvalExpression(a.get()));
                    itNative->second(args);
                }
                else
                {
                    throw std::runtime_error("Unknown function: " + call->name);
                }
            }
        }
    }
}

void PulseInterpreter::ExecuteFunction(ASTFunctionDef *func, const std::vector<Value> &args)
{
    if (args.size() != func->parameters.size())
        throw std::runtime_error("Wrong number of arguments in function call");

    // Create a new local scope
    Scope previous = scope; // copy previous scope
    scope = Scope();

    // Bind parameters
    for (size_t i = 0; i < args.size(); i++)
    {
        scope.variables[func->parameters[i]] = args[i];
    }

    // Execute body
    Execute(func->body);

    // Restore previous scope
    scope = previous;
}

Value PulseInterpreter::EvalExpression(const ASTExpression *expr)
{
    if (!expr)
        throw std::runtime_error("Null expression");
    if (auto call = dynamic_cast<const ASTFunctionCall *>(expr))
    {
        std::vector<Value> args;
        for (auto &arg : call->args)
        {
            args.push_back(EvalExpression(arg.get()));
        }
        auto it = nativeFunctions.find(call->name);
        if (it != nativeFunctions.end())
            return it->second(args);
        throw std::runtime_error("Unknown function: " + call->name);
    }
    if (auto bin = dynamic_cast<const ASTBinaryOp *>(expr))
    {
        auto leftVal = EvalExpression(bin->left.get());
        auto rightVal = EvalExpression(bin->right.get());

        float leftF, rightF;

        // Convert left operand
        if (auto pInt = std::get_if<int>(&leftVal))
            leftF = static_cast<float>(*pInt);
        else if (auto pFloat = std::get_if<float>(&leftVal))
            leftF = *pFloat;
        else
            throw std::runtime_error("Left operand is not numeric");

        // Convert right operand
        if (auto pInt = std::get_if<int>(&rightVal))
            rightF = static_cast<float>(*pInt);
        else if (auto pFloat = std::get_if<float>(&rightVal))
            rightF = *pFloat;
        else
            throw std::runtime_error("Right operand is not numeric");

        // Compute
        float result;
        switch (bin->op) {
            case '+': result = leftF + rightF; break;
            case '-': result = leftF - rightF; break;
            case '*': result = leftF * rightF; break;
            case '/': 
                if (rightF == 0) throw std::runtime_error("Division by zero");
                result = leftF / rightF; 
                break;
            default:
                throw std::runtime_error("Unknown binary operator");
        }

        // If both were int, maybe return int
        if (std::holds_alternative<int>(leftVal) && std::holds_alternative<int>(rightVal))
            return static_cast<int>(result);
        return result;

    }

    return expr->Evaluate(scope);

    throw std::runtime_error("Unknown expression type");
}
