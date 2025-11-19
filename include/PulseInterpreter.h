#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
#include <variant>
#include <functional>
#include "PulseParser.h"

using Value = std::variant<int, std::string>;

struct Scope 
{
    std::unordered_map<std::string, Value> variables;
};

class PulseInterpreter 
{
public:
    PulseInterpreter();

    // exécution d'une liste de statements
    void Execute(const std::vector<std::unique_ptr<ASTStatement>>& stmts);

    // fonctions natives
    void RegisterFunction(const std::string& name, std::function<Value(const std::vector<Value>&)> func);

    const Scope& GetScope() const { return scope; }

private:
    Value EvalExpression(const ASTExpression* expr);

    Scope scope;
    std::unordered_map<std::string, std::function<Value(const std::vector<Value>&)>> nativeFunctions;
};
