#ifndef STDIOINTERPRETER_H
#define STDIOINTERPRETER_H

#include "Brainfuck.h"

class StdIoInterpreter : public Brainfuck::Interpreter
{
public:
    StdIoInterpreter(size_t size = 30000)
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

#endif //STDIOINTERPRETER_H
