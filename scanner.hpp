/* Authors: Tyler Burkett
 * Part of the handout for project 1 of CS441 Fall 2024
 * Description: Scanner class for B-Sharp.
 * Fill out the missing methods in "scanner.cpp" to complete
 * the project. Some methods have already been provided.
 */
#pragma once
#include "token.hpp"
#include <regex>
#include <utility>

class Scanner {
    public:

    using position = std::pair<int, int>;

    Scanner(std::istream & stream);

    // Produce next token found by scanner
    token_ty next();

    // Check if scanner still has tokens to return
    bool has_next();

    private:
    std::string input_text{};
    position current_position = {1,1};
    std::smatch best_match;


    void read_input(std::istream & stream);

    // Calcualte position where a token's text ends
    position end_position_of(std::string text);

    // Calcualte position that is after a token's text
    position position_after(std::string text);

    // Methods to process literal tokens
    bool process_int_lit   (std::smatch match, token_ty& token);
    bool process_float_lit (std::smatch match, token_ty& token);
    bool process_char_lit  (std::smatch match, token_ty& token);
    bool process_string_lit(std::smatch match, token_ty& token);
    bool process_bool_lit  (std::smatch match, token_ty& token);

    // Default action for most tokens
    template<typename TokenType>
    bool default_rule      (std::smatch match, token_ty& token);

    // Action to perform when a token is matched but not mean to
    // be produced
    bool default_ignore    (std::smatch match, token_ty& token);
    
};
