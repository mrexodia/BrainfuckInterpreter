#include <iostream>
#include <vector>
#include "Brainfuck.h"

class StdIoSemantics : public Brainfuck::Semantics
{
public:
    StdIoSemantics(size_t size = 30000)
            : _ptr(0)
    {
        _array.reserve(size);
    }

    virtual void IncrementPointer()
    {
        if (_ptr < _array.size() - 1)
            _ptr++;
    }

    virtual void DecrementPointer()
    {
        if (_ptr > 0)
            _ptr--;
    }

    virtual void IncrementData()
    {
        _array[_ptr]++;
    }

    virtual void DecrementData()
    {
        _array[_ptr]--;
    }

    virtual void ReadData()
    {
        int ch = std::getchar();
        if (ch != EOF)
            _array[_ptr] = (unsigned char)ch;
    }

    virtual void WriteData()
    {
        std::putchar(_array[_ptr]);
    }

    virtual bool TestZero() const
    {
        return _array[_ptr] == 0;
    }

protected:
    std::vector<unsigned char> _array;
    size_t _ptr;
};

int main(int argc, char* argv[])
{
    auto interpreter = Brainfuck();
    auto code = "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.";
    auto error = interpreter.Parse(code);
    if (error != Brainfuck::Success)
    {
        std::string errorName;
        switch (error)
        {
            case Brainfuck::ErrorNonMatchingBracket:
                errorName = "ErrorNonMatchingBracket";
                break;

            case Brainfuck::GenericError:
                errorName = "GenericError";
                break;

            default:
                errorName = "UNKNOWN";
                break;
        }
        std::cout << "There was an error parsing (" << errorName << ")" << std::endl;
        return -1;
    }
    auto context = StdIoSemantics();
    interpreter.Execute(context);
    return 0;
}