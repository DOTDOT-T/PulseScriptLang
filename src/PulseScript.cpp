#include "PulseScript.h"

#include <iostream>
#include <fstream>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include "PulseLexer.h"
#include "PulseParser.h"
#include "PulseInterpreter.h"

PulseScript::PulseScript(const char *scriptPath)
{
    std::string code = ReadFileToString("main.PulseScript");    

    // Lexer
    PulseLexer lexer(code);
    std::vector<Token> tokens;
    while (!lexer.End()) tokens.push_back(lexer.Next());
    tokens.push_back({TokenType::EndOfFile, ""});

    // Parser
    PulseParser parser(tokens);
    auto src = parser.ParseScript();
    ast = std::move(src);
    interpreter = std::make_unique<PulseInterpreter>();

}

PulseScript::~PulseScript()
{
}

void PulseScript::Execute()
{
    interpreter->Execute(ast);
}

std::string PulseScript::ReadFileToString(const std::string& filename) 
{
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (!file) {
        throw std::runtime_error("Impossible d'ouvrir le fichier: " + filename);
    }

    std::ostringstream ss;
    ss << file.rdbuf();  // Lit tout le contenu
    return ss.str();
}
