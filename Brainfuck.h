#ifndef BRAINFUCK_H
#define BRAINFUCK_H

#include <cstdio>
#include <vector>
#include <unordered_map>

class Brainfuck
{
public:
    enum ParseError
    {
        Success,
        ErrorBracketMismatch,
        GenericError
    };

    class Interpreter
    {
    public:
        virtual void IncrementPointer() = 0;
        virtual void DecrementPointer() = 0;
        virtual void IncrementData() = 0;
        virtual void DecrementData() = 0;
        virtual void ReadData() = 0;
        virtual void WriteData() = 0;
        virtual bool TestZero() const = 0;
    };

    class Compiler
    {
    public:
        virtual void IncrementPointer() = 0;
        virtual void DecrementPointer() = 0;
        virtual void IncrementData() = 0;
        virtual void DecrementData() = 0;
        virtual void ReadData() = 0;
        virtual void WriteData() = 0;
        virtual void JumpForward(int src, int dest) = 0;
        virtual void JumpBackward(int src, int dest) = 0;
    };

    Brainfuck()
    {
        _commandMap['>'] = IncrementPointer;
        _commandMap['<'] = DecrementPointer;
        _commandMap['+'] = IncrementData;
        _commandMap['-'] = DecrementData;
        _commandMap['.'] = WriteData;
        _commandMap[','] = ReadData;
        _commandMap['['] = JumpForward;
        _commandMap[']'] = JumpBackward;
    }

    ParseError Parse(const std::string & code)
    {
        std::vector<size_t> bracketStack;
        size_t matchingBracket;

        for (auto ch : code)
        {
            auto found = _commandMap.find(ch);
            if (found != _commandMap.end()) //the character is a valid brainfuck command
            {
                auto node = CommandNode(found->second);
                switch (node.command)
                {
                    case JumpForward:
                        _commands.push_back(node); //add the command to the list
                        bracketStack.push_back(_commands.size() - 1); //push a pointer to this command on the bracket stack
                        break;

                    case JumpBackward:
                        if (!bracketStack.size()) //empty bracket stack means bracket mismatch
                            return ErrorBracketMismatch;

                        matchingBracket = bracketStack.back(); //pop the last element from the bracket stack
                        bracketStack.pop_back();

                        node.matchingBracket = matchingBracket; //update the closing bracket
                        _commands.push_back(node); //add the command to the list
                        _commands[matchingBracket].matchingBracket = _commands.size() - 1; //update the opening bracket
                        break;

                    default:
                        _commands.push_back(node); //add the command to the list
                        break;
                }
            }
            else
            {
                if (ch == '\n')
                {
                    _line++;
                    _column = 1;
                }
                else if (ch != '\r')
                    _column++;
                _commands.push_back(CommandNode(Unknown)); //add an unknown (empty) command to the list
            }
        }
        return bracketStack.size() ? ErrorBracketMismatch : Success; //non-empty bracket stack means bracket mismatch
    }

    void Execute(Interpreter & semantics)
    {
        auto commandCount = _commands.size();
        if (!_commands.size())
            return;

        for (size_t currentCommand = 0; currentCommand < commandCount; currentCommand++)
        {
            unsigned char byte;
            switch (_commands[currentCommand].command)
            {
                case Unknown: //unknown commands do nothing (this allows comments)
                    break;

                case IncrementPointer:
                    semantics.IncrementPointer();
                    break;

                case DecrementPointer:
                    semantics.DecrementPointer();
                    break;

                case IncrementData:
                    semantics.IncrementData();
                    break;

                case DecrementData:
                    semantics.DecrementData();
                    break;

                case WriteData:
                    semantics.WriteData();
                    break;

                case ReadData:
                    semantics.ReadData();
                    break;

                case JumpForward:
                    if (semantics.TestZero())
                        currentCommand = _commands[currentCommand].matchingBracket;
                    break;

                case JumpBackward:
                    if (!semantics.TestZero())
                        currentCommand = _commands[currentCommand].matchingBracket;
                    break;
            }
        }
    }

    void Compile(Compiler & compiler)
    {
        auto commandCount = _commands.size();
        for (size_t currentCommand = 0; currentCommand < commandCount; currentCommand++)
        {
            const auto & command = _commands[currentCommand];
            switch (command.command)
            {
                case Unknown: //unknown commands do nothing (this allows comments)
                    break;

                case IncrementPointer:
                    compiler.IncrementPointer();
                    break;

                case DecrementPointer:
                    compiler.DecrementPointer();
                    break;

                case IncrementData:
                    compiler.IncrementData();
                    break;

                case DecrementData:
                    compiler.DecrementData();
                    break;

                case WriteData:
                    compiler.WriteData();
                    break;

                case ReadData:
                    compiler.ReadData();
                    break;

                case JumpForward:
                    compiler.JumpForward(currentCommand, command.matchingBracket);
                    break;

                case JumpBackward:
                    compiler.JumpBackward(currentCommand, command.matchingBracket);
                    break;
            }
        }
    }

    void GetLocation(int & line, int & column)
    {
        line = _line;
        column = _column;
    }

private:
    enum Command
    {
        Unknown,
        IncrementPointer,
        DecrementPointer,
        IncrementData,
        DecrementData,
        WriteData,
        ReadData,
        JumpForward,
        JumpBackward
    };

    struct CommandNode
    {
        CommandNode(Command command)
                : command(command),
                  matchingBracket(-1)
        {
        }

        size_t matchingBracket;
        Command command;
    };

    std::vector<CommandNode> _commands;
    std::unordered_map<char, Command> _commandMap;
    int _line = 1;
    int _column = 1;
};

#endif //BRAINFUCK_H
