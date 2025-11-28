#include "PulseScriptsManager.h"
#include "PulseScript.h"
#include "utilities.h"
#include "PulseInterpreter.h"
#include <iostream>
#include <vector>


int main() 
{
    PulseScriptsManager psm;
    psm.AddScriptToDatabase("main.PulseScript");

    PulseInterpreter::RegisterFunction("Log",
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

    PulseInterpreter::RegisterFunction("Rand",
        [&](const std::vector<Value>& args) -> Value
        {
            return rand();
        }
    );


    std::vector<Variable> args;
    while(true) 
    {
        std::cout << "new frame" << std::endl;
        psm.GetScript("main.PulseScript")->ExecuteScriptFunction("Update", args);
        std::cout << "end frame" << std::endl;
        std::cin.get();
    }

    std::cin.get();
    return 0;
}
