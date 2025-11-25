#include "PulseScriptsManager.h"
#include "PulseScript.h"
#include "utilities.h"
#include "PulseInterpreter.h"
#include <iostream>
#include <vector>


int main() 
{
    PulseScriptsManager::AddScriptToDatabase("main.PulseScript");

    PulseInterpreter::RegisterFunction("log",
        [&](const std::vector<Value>& args) -> Value
        {
            for(auto& arg : args)
            {
                std::visit([](auto&& v)
                {
                    std::cout << v;
                }, arg);
            
                std::cout << " ";
            }
        
            std::cout << std::endl;
            return 0;
        }
    );


    std::vector<Variable> args;
    while(true)
    // PulseScriptsManager::GetScript("main.PulseScript")->Execute();
    PulseScriptsManager::GetScript("main.PulseScript")->ExecuteScriptFunction("Update", args);

    std::cin.get();
    return 0;
}
