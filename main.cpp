#include "PulseScriptsManager.h"
#include "PulseScript.h"
#include "utilities.h"
#include <iostream>
#include <vector>


int main() 
{
    PulseScriptsManager::AddScriptToDatabase("main.PulseScript");

    std::vector<Variable> args;
    while(true)
    // PulseScriptsManager::GetScript("main.PulseScript")->Execute();
    PulseScriptsManager::GetScript("main.PulseScript")->ExecuteScriptFunction("Update", args);

    std::cin.get();
    return 0;
}
