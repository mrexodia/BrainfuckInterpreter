#include <iostream>
#include <fstream>
#include <vector>
#include "Brainfuck.h"
#include "StdIoInterpreter.h"
#include "CCompiler.h"

static bool readFile(const std::string & filename, std::string & contents)
{
    std::ifstream file(filename);
    if(!file.is_open())
        return false;
    std::string str;
    while(std::getline(file, str))
        contents += str + "\n";
    file.close();
    return true;
}

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        std::cout << "usage: BrainfuckInterpreter file.bf [i/c]" << std::endl;
        return -1;
    }

    std::string code;
    if(!readFile(argv[1], code))
    {
        std::cout << "failed to read input file!" << std::endl;
        return -1;
    }

    auto brainfuck = Brainfuck();
    auto error = brainfuck.Parse(code);
    if (error != Brainfuck::Success)
    {
        std::string errorName;
        switch (error)
        {
            case Brainfuck::ErrorBracketMismatch:
                errorName = "ErrorBracketMismatch";
                break;

            case Brainfuck::GenericError:
                errorName = "GenericError";
                break;

            default:
                errorName = "UNKNOWN";
                break;
        }
        int line, column;
        brainfuck.GetLocation(line, column);
        std::cout << "There was an error parsing (" << errorName << ") on line " << line << " column " << column << std::endl;
        return -1;
    }

    auto mode = std::string(argc > 2 ? argv[2] : "i");
    if(mode == "i") //interpret the file
    {
        auto semantics = StdIoSemantics();
        brainfuck.Execute(semantics);
    }
    else if(mode == "c") //compile to c
    {
        auto compiler = CCompiler();
        brainfuck.Compile(compiler);
        std::cout << compiler.GetCode();
    }
    else
    {
        std::cout << "Unknown mode!" << std::endl;
        return -1;
    }
    return 0;
}