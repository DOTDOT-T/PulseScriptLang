#include "PulseLexer.h"
#include "PulseParser.h"
#include "PulseInterpreter.h"
#include <iostream>
#include <fstream>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

std::string ReadFileToString(const std::string& filename) {
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (!file) {
        throw std::runtime_error("Impossible d'ouvrir le fichier: " + filename);
    }

    std::ostringstream ss;
    ss << file.rdbuf();  // Lit tout le contenu
    return ss.str();
}

int main() {
    std::string code = ReadFileToString("main.PulseScript");
    

    // Lexer
    PulseLexer lexer(code);
    std::vector<Token> tokens;
    while (!lexer.End()) tokens.push_back(lexer.Next());
    tokens.push_back({TokenType::EndOfFile, ""});

    // Parser
    PulseParser parser(tokens);
    auto ast = parser.ParseScript();

    // Interpreter
    PulseInterpreter interpreter;
    interpreter.Execute(ast);

    std::cin.get();
    return 0;
}
