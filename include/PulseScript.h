#ifndef PULSESCRIPT_H
#define PULSESCRIPT_H

#include <memory>
#include <vector>
#include <string>

class PulseLexer;
class PulseParser;
class PulseInterpreter;
struct ASTStatement;

class PulseScript
{
public:
    PulseScript(const char* scriptPath);
    ~PulseScript();
    void Execute();

private:
    std::vector<std::unique_ptr<ASTStatement>> ast;
    std::unique_ptr<PulseInterpreter> interpreter;

    std::string ReadFileToString(const std::string& filename);
};

#endif