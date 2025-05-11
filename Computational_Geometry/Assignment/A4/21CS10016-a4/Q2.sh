#!/bin/bash

# Check if the source file 'Q2.cpp' exists in the current directory.
if [ ! -f Q2.cpp ]; then
    echo "Error: Q2.cpp not found!"
    exit 1
fi

# Compile 'Q2.cpp' using g++ with C++14 standard, enabling all warnings and O2 optimization.
# The compiled output will be named 'Q2.out'.
g++ -std=c++14 -Wall -O2 -o Q2.out Q2.cpp

# Check the exit status of the compilation command.
# An exit status of 0 indicates successful compilation.
if [ $? -eq 0 ]; then
    echo "Compilation successful. You can run the program using: ./Q2.out <filename>"
else
    echo "Compilation failed."
fi
