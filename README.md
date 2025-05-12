# Compilers-Scanner-Project
General Project Description:
This is a project from my compilers class in the Fall of 2024. The goal of the project was to make a simple scanner for a simple programming language, called B Minor. 
A scanner takes a file as input and outputs all of the tokens within that file. A token is the smallest element in a programming language. 
For our programming language, some of the tokens we needed to match were integers, strings, identifiers, chars, floats, single line comments, and double line comments. 
A scanner is the first stage in a compiler. A compiler translates code from a high-level programming language to a low-level programming language. 

Libraries, technologies, and data structures used:
The project uses C++ Regex libraries to match the input to the different token types. All of the tokens are stored in variant structs. There is a callable struct in main.cpp for returning the line number, token type,
and token value when std::visit is called in main. There is a class for the scanner.

Bugs currently within the code:
The code currently does not handle strings, chars, or "bad" chars. Bad chars are described in the project description as being chars with invalid escapes or chars with multiple letters in single quotes.
These cases produce an infinite loop when hit.

File running instructions:
To run the project, type the input file into the IN="" in the makefile, save the file, and then type Make run at the terminal.
