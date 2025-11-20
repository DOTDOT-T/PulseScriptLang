#include "PulseScriptsManager.h"
#include "PulseScript.h"
#include <iostream>


int main() 
{
    PulseScriptsManager::AddScriptToDatabase("main.PulseScript");

    PulseScriptsManager::GetScript("main.PulseScript")->Execute();

    std::cin.get();
    return 0;
}
