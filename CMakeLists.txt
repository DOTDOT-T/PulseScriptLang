cmake_minimum_required(VERSION 3.21)

project(PulseScriptParser)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(PulseParserExample)

target_sources(PulseParserExample
    PRIVATE
        main.cpp   
        src/PulseParser.cpp 
        src/PulseLexer.cpp 
        src/PulseInterpreter.cpp 
        src/PulseScript.cpp
        src/PulseScriptsManager.cpp
)
target_include_directories(PulseParserExample PRIVATE ${PROJECT_SOURCE_DIR}/include)