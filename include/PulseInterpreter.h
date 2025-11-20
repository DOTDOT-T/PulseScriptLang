#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
#include "PulseParser.h"

struct Scope
{
    std::unordered_map<std::string, Value> variables;
};

class PulseInterpreter
{
public:
    PulseInterpreter();

    // exécution d'une liste de statements
    void Execute(const std::vector<std::unique_ptr<ASTStatement>> &stmts);
    void ExecuteFunction(ASTFunctionDef *func, const std::vector<Value> &args);
    // fonctions natives
    void RegisterFunction(const std::string &name, std::function<Value(const std::vector<Value> &)> func);

    const Scope &GetScope() const { return scope; }

private:
    Value EvalExpression(const ASTExpression *expr);

    Scope scope;
    std::unordered_map<std::string, std::function<Value(const std::vector<Value> &)>> nativeFunctions;
    std::unordered_map<std::string, std::unique_ptr<ASTFunctionDef>> userFunctions;
};
