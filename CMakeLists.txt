cmake_minimum_required(VERSION 3.3)
project(BrainfuckInterpreter)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(BrainfuckInterpreter ${SOURCE_FILES} Brainfuck.h CCompiler.h StdIoInterpreter.h)