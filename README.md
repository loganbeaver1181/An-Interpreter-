# An-Interpreter

This project is a simple interpreter written in C. It uses a recursive descent parser to interpret a custom language defined in the Grammar.txt file.

Getting Started
These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

Prerequisites
GCC Compiler
A Unix-like operating system or Windows with a Unix-like environment (like WSL or Cygwin)
The interpreter takes two command-line arguments: the input file and the output file.

The inputFile should contain the code written in the custom language defined in the Grammar.txt file. The outputFile will contain the output of the interpreted code.

Files
Interpreter.c: The main controller for the interpreter.
Parser.c: Contains the recursive descent parser.
Tokenizer.c: Contains the tokenizer that breaks the input into tokens.
Interpreter.h, Parser.h, Tokenizer.h: Header files for the corresponding C files.
Grammar.txt: Defines the grammar of the custom language that the interpreter can interpret.
