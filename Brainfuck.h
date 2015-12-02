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
        ErrorNonMatchingBracket,
        GenericError
    };

    class Semantics
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
                            return ErrorNonMatchingBracket;

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
                _commands.push_back(CommandNode(Unknown)); //add an unknown (empty) command to the list
        }
        return bracketStack.size() ? ErrorNonMatchingBracket : Success; //non-empty bracket stack means bracket mismatch
    }

    void Execute(Semantics & semantics)
    {
        auto commandCount = _commands.size();
        if (!_commands.size())
            return;

        for(size_t currentCommand = 0; currentCommand < commandCount; currentCommand++)
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
};

#endif //BRAINFUCK_H
