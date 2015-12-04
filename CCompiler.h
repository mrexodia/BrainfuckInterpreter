#ifndef CCOMPILER_H
#define CCOMPILER_H

#include "Brainfuck.h"

class CCompiler : public Brainfuck::Compiler
{
public:
    CCompiler()
    {
        _indent = 0;
        addLine("#include <stdio.h>");
        addLine("#include <string.h>");
        addLine("");
        addLine("int main() {");
        addLine("    unsigned char array[30000];");
        addLine("    unsigned char* ptr = array;");
        addLine("    memset(array, 0, sizeof(array));");
        addLine("");
        _indent = 4;
    }

    virtual void IncrementPointer()
    {
        addLine("++ptr;");
    }

    virtual void DecrementPointer()
    {
        addLine("--ptr;");
    }

    virtual void IncrementData()
    {
        addLine("++*ptr;");
    }

    virtual void DecrementData()
    {
        addLine("--*ptr;");
    }

    virtual void WriteData()
    {
        addLine("putchar(*ptr);");
    }

    virtual void ReadData()
    {
        addLine("*ptr = getchar();");
    }

    virtual void JumpForward(int src, int dest)
    {
        addLine("while (*ptr) {");
        _indent += 4;
    }

    virtual void JumpBackward(int src, int dest)
    {
        _indent -= 4;
        addLine("}");
    }

    std::string GetCode()
    {
        return _code + "}\n";
    }

protected:
    std::string _code;
    int _indent;

    std::string makeIndent()
    {
        std::string result;
        for (int i = 0; i < _indent; i++)
            result += " ";
        return result;
    }

    void addLine(std::string line)
    {
        _code += makeIndent() + line + "\n";
    }
};

#endif //CCOMPILER_H
