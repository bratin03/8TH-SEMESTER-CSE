#!/bin/bash

# Check if the source file 'Q1.cpp' exists in the current directory.
if [ ! -f Q1.cpp ]; then
    echo "Error: Q1.cpp not found!"
    exit 1
fi

# Compile 'Q1.cpp' using g++ with C++14 standard, enabling all warnings and O2 optimization.
# The compiled output will be named 'Q1.out'.
g++ -std=c++14 -Wall -O2 -o Q1.out Q1.cpp

# Check the exit status of the compilation command.
# An exit status of 0 indicates successful compilation.
if [ $? -eq 0 ]; then
    echo "Compilation successful. You can run the program using: ./Q1.out <n>"
else
    echo "Compilation failed."
fi
