/* Authors: Tyler Burkett, Linda Rynerson
 * Part of the handout for project 1 of CS441 Fall 2024
 * CLASS: CPE 441
 * PROGRAM: main.cpp
 * DATE: 10/2/2024
 * SYNOPSIS: program takes input from a .b file and outputs the tokens along with the line number and type. To run the program, change the file in the makefile to the file you are wanting to use and then type make run at the terminal. Also includes support for make clean. 
 * COMPILATION: g++ -Wall -g -std=c++20
 * BUGS: characters, strings and bad characters do not work.
 * REFERENCE: Tyler Burket, Hunter Abshire
 */

#include <iostream>
#include <fstream>
#include <regex>
#include <variant>
#include "token.hpp"
#include "scanner.hpp"

#define MIN_ARGC 2

/* ASCII color codes */
static const std::string RESET           =  "\x1b[0m";
static const std::string COLOR_BRIGHTER  =  "\x1b[1m";
static const std::string UNDERLINE_TEXT  =  "\x1b[4m";
static const std::string COLOR_RED       =  "\x1b[31m";
static const std::string COLOR_GREEN     =  "\x1b[32m";
static const std::string COLOR_YELLOW    =  "\x1b[33m";
static const std::string COLOR_BLUE      =  "\x1b[34m";

// Format string using ASCII color codes to change how text is dispalyed on the console 
std::string color_string(std::string text, std::string color_code) {
    return color_code + text + RESET; 
}

// Helper function to format the value of a character token for printing
std::string escape_char(char raw_char) {
    switch(raw_char) {
        case '\n': return "\\n";
        case '\t': return "\\t";
        case '\'': return "\\'";
        case '\\': return "\\\\";
        default: return std::string(1, raw_char);
    }
}

// Helper function to format the value of a string token for printing 
std::string escape_string(std::string raw_string) {
    std::string current_str_val = raw_string;
    std::string new_str_val;
    std::regex newline(R"(\n)");
    std::regex tab(R"(\t)");
    std::regex backslash(R"(\\)");
    std::regex doublequote(R"(\")");

    new_str_val = regex_replace(current_str_val, backslash, "\\");
    std::swap(current_str_val, new_str_val);
    new_str_val = regex_replace(current_str_val, newline, "\\n");
    std::swap(current_str_val, new_str_val);
    new_str_val = regex_replace(current_str_val, tab, "\\t");
    std::swap(current_str_val, new_str_val);
    new_str_val = regex_replace(current_str_val, doublequote, "\\\"");

    return new_str_val;
}

// Visitor (callable struct) used to generate strings to print out tokens.
// This struct returns the line and column as well as what type of token was received. It also colors the tokens and escapes characters and strings.
struct token_stringify {
template <typename Token>
std::string get_start_position(Token &t) const { 
    			return std::to_string(t.start.first) + ":" + std::to_string(t.start.second);
}
std::string operator()(const int_token &x)const { 
			return get_start_position(x) + "{"+ "int_token :" + color_string(std::to_string(x.val),COLOR_BLUE+COLOR_BRIGHTER)
			+"}"; 
			}
std::string operator()(const float_token &x)   const {
     			return get_start_position(x) + "{" + "float_token :" + color_string(std::to_string(x.val),COLOR_BLUE+COLOR_BRIGHTER)+"}"; 
			}
std::string operator()(const string_token &x)  const { 
			return get_start_position(x) + "{" + "string_token :"+escape_string(x.val) + color_string(x.val,COLOR_YELLOW)+"}";
			}	
std::string operator()(const char_token &x)    const {
       	return get_start_position(x) + "{" + "char_token :" + escape_char(x.val)+ color_string(std::to_string(x.val),COLOR_YELLOW+COLOR_BRIGHTER)+"}"; 
                        }
std::string operator()(const bool_token &x)    const { return get_start_position(x) + "{" + "bool_token :" + std::to_string(x.val)+"}"; }
std::string operator()(const id_token &x)      const { return get_start_position(x) + "{" + "id_token :" + x.val +"}"; }
std::string operator()(const keyword_token &x) const { return get_start_position(x) + "{" + "keyword_token :" + color_string(x.val,COLOR_BLUE+UNDERLINE_TEXT) + "}"; }
std::string operator()(const symbol_token &x)  const { return get_start_position(x) + "{" + "symbol_token :" + x.val+"}"; }
std::string operator()(const error_token &x)   const { return get_start_position(x) + "{" + "error_token :" + color_string(x.val,COLOR_RED) + "}"; }
std::string operator()(const comment_token &x) const { return get_start_position(x) + "{" + "comment_token :" + color_string(x.val,COLOR_GREEN) + "}"; }
std::string operator()(const end_of_input &x)  const { return ""; }
};

template<>
std::string token_stringify::get_start_position<end_of_input>(end_of_input &t) const {
    return std::string("-1:-1");
}

int main(int argc, char *argv[]) {

    if (argc < MIN_ARGC) {
        std::cerr << "Expected two argument; the name of a file to scan and where to output the HTML to" << std::endl;
        return -1;
    }

    // Open the file to scan and file to output HTML to
    std::ifstream bsharpFile;
    try {
        bsharpFile.open(argv[1]);
    } catch (...) {
        std::cerr << "Unable to open file \"" << argv[1] << "\"\n";
        return -1;
    }

    // Print out file text
    std::cout << "Input:" << std::endl 
              << bsharpFile.rdbuf() << std::endl
              << std::endl;
    bsharpFile.clear();
    bsharpFile.seekg(0);
    
    std::cout << "Tokens:" << std::endl;
    token_ty token{};
    Scanner scanner(bsharpFile);
    //token_stringify stringy;
    // Loop over tokens and print them until EOF reached 
    // vvvvvvvvvvvvv
    while(scanner.has_next()) {	    
	    token = scanner.next(); 
	    std::cout<<std::visit(token_stringify(),token)<<std::endl;
	    

    }
    // ^^^^^^^^^^^^^

    // Close file
    try {
        bsharpFile.close();
    } catch (...) {
        std::cerr << "Error while closing file \"" << argv[1] << "\"\n";
        return -1;
    }

    return 0;
}
