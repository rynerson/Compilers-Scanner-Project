#include "scanner.hpp"
#include <iostream>
#include <sstream>
/* AUTHOR: Linda Rynerson
 * CLASS: CPE 441
 * PROGRAM: scanner.cpp
 * DATE: 10/2/2024
 * COMPILATION g++ -Wall -g -std=c++20
 * SYNOPSIS: program takes input from a .b file and outputs the tokens along with the line number and type. To run the program, change the file in t *he makefile to the file you are wanting to use and then type make run at the terminal. Also includes support for make clean.
 * BUGS: strings characters and bad characters result in an infinite loop
 * REFERENCES: Tyler Burkett Hunter Abshire*/
/* Public */
Scanner::Scanner(std::istream & stream) {
        read_input(stream);
	
    };

bool Scanner::has_next() {
    return not input_text.empty();
}

// Complete this method
token_ty Scanner::next() {
	std::regex Integer(R"(^\d+)");
       	std::regex Keyword(R"(^(as|array|boolean|char|else|float|for|function|if|integer|print|return|string|void|
	and|not|or|loop|while|repeat|write|read))");
	std::regex Operator(R"(^\:|\,|\;|\=|\[|\]|\(|\)|\++|\--|\-|\!|\^|\*|\/|\%|\+|\-|\<|\<=|\>|\>=|\==|\!=|\&&|\||)");
	std::regex Identifier(R"(^([a-zA-Z][a-zA-Z0-9_]*))");
	std::regex Comsing(R"(^(//.*))");
	std::regex multi (R"(^(/\*[^*]*\*+([^\/*][^*]*\*+)*\/))");
	std::regex floater(R"(^(\d+\.\d*))");
	std::regex stringer(R"(^(\".*\"))");
	std::regex boolean(R"(^(true|false))");
	std::regex character(R"(^(\'.\'))");
	std::regex bad(R"(^('(..|")'))");
	std::regex whitespace(R"(^(\s+))");
	std::regex eof(R"(EOF)");
	std::smatch match_result;
	token_ty token{};	
	//check if eof is matching
     if(std::regex_search(input_text,match_result,eof)){
                return empty_token{};
		     
     }
     //remove whitespace
     else if(std::regex_search(input_text,match_result,whitespace)){
	     std::regex_replace(input_text,whitespace,"");
	     
     }
     //check if float numbers are matching
     else if(std::regex_search(input_text,match_result,floater)){
	     process_float_lit(match_result,token);
     }
     //check if integers are matching
     else if(std::regex_search(input_text,match_result,Integer)){
		process_int_lit(match_result,token);	
     }
     //check if keywords are matching
     else if(std::regex_search(input_text,match_result,Keyword)){
		default_rule<keyword_token>(match_result,token);
		
     }
     //check if booleans are matching
     else if(std::regex_search(input_text,match_result,boolean)){
     		process_bool_lit(match_result,token);
     }
     //check if identifiers a matching
     else if(std::regex_search(input_text,match_result,Identifier)){
	     default_rule<id_token>(match_result,token);
     }
     //check if single line comments are matching
     else if(std::regex_search(input_text,match_result,Comsing)){
		default_rule<comment_token>(match_result,token);
	}
     //check if multiline comments are matching
     else if(std::regex_search(input_text,match_result,multi)){
	     default_rule<comment_token>(match_result,token);
   	  }
     //check if operators are matching
     else if(std::regex_search(input_text,match_result,Operator)){
		default_rule<symbol_token>(match_result,token);
		}
     //check if strings are matching
     else if(std::regex_search(input_text,match_result,stringer)){
		process_string_lit(match_result,token);
		}
     //check if characters are matching
     else if(std::regex_search(input_text,match_result,character)){	
		process_char_lit(match_result,token);
		}
     //check if there is a bad character
     else if(std::regex_search(input_text,match_result,bad)){
		auto start_position = current_position;
		auto end_position = end_position_of(match_result[0]);
                token.emplace<error_token>(error_token{start_position,end_position,match_result[0]}); 
		}
    //anything not matched to any of the above	
    else{
	     auto start_position = current_position;
	     char err_char = 'e';
     	     auto end_position = end_position_of(std::to_string(err_char));
	     token.emplace<error_token>(error_token{start_position,end_position,std::to_string(err_char)});	     
	}
    //return token
    current_position = position_after(match_result[0]);
    input_text = match_result.suffix();
    return token;
    
}


/* Private */
void Scanner::read_input(std::istream & stream) {
   constexpr auto read_size = std::size_t{4096};
   stream.exceptions(std::ios_base::badbit);
   //auto out = std::string{};
   auto buf = std::string(read_size, '\0');
   while (stream.read(& buf[0], read_size)) {
       input_text.append(buf, 0, stream.gcount());
   }
   input_text.append(buf, 0, stream.gcount());
}

Scanner::position Scanner::end_position_of(std::string text) {
    if (text.length() == 0) {
        return current_position;
    }
    auto end_position = current_position;
    for (auto& i : text.substr(0, text.length()-1)) {
        if (i == '\n') {
            end_position.first++;
            end_position.second = 1;
        }
        else {
            end_position.second++;
        }
    }
    return end_position;
} // end_position_of


Scanner::position Scanner::position_after(std::string text) {
    auto new_position = current_position;
    for (auto& i : text) {
        if (i == '\n') {
            new_position.first++;
            new_position.second = 1;
        }
        else {
            new_position.second++;
        }
    }

    return new_position;
} // position_after



// Methods to process literal tokens
bool Scanner::process_int_lit   (std::smatch match, token_ty& token) {
	int val = stoi(match[0]);
	auto start_position = current_position;
	auto end_position = end_position_of(match[0]);
	token.emplace<int_token>(int_token{start_position,end_position,val});	
    		return true;
	
}

bool Scanner::process_float_lit (std::smatch match, token_ty& token) {
	float val = stof(match[0]);
        auto start_position = current_position;
	auto end_position = end_position_of(match[0]);
	token.emplace<float_token>(float_token{start_position,end_position,val});
		return true;
	
}

bool Scanner::process_char_lit  (std::smatch match, token_ty& token) {
	std::string current_str = match[0];
       char new_str;
       auto start_position = current_position;
       auto end_position = end_position_of(match[0]);
       if(current_str == "\\n"){
	       new_str = '\n';
       }
       else if(current_str == "\\t"){
	       new_str = '\t';
       }
       else if(current_str == "\\'"){
	       new_str = '\'';
       }
       else if(current_str == "\\\\"){
	       new_str = '\\';
       }
       else{
	       new_str = current_str[0];
       }
       token.emplace<char_token>(char_token{start_position,end_position,new_str});
    		return true;
	
}

bool Scanner::process_string_lit(std::smatch match, token_ty& token) {
	std::string current_str = match[0];
	std::string new_str;
	std::regex new_line(R"(\\n)");
	std::regex tab(R"(\\t)");
	std::regex backslash(R"(\\)");
	std::regex doublequote(R"(\\\")");
	new_str = regex_replace(current_str,backslash,"\\");
	std::swap(current_str,new_str);
	new_str = regex_replace(current_str, new_line,"\n");
	std::swap(current_str,new_str);
	new_str = regex_replace(current_str, tab,"\t");
	std::swap(current_str,new_str);
	new_str = regex_replace(current_str,doublequote,"");
	auto start_position = current_position;
	auto end_position = end_position_of(match[0]);	
	token.emplace<string_token>(string_token{start_position,end_position,new_str});	
    		return true;
}

bool Scanner::process_bool_lit  (std::smatch match, token_ty& token) {
	 std::istringstream iss(match[0]);
	 bool res;
	 iss>>std::boolalpha>>res;
	 auto start_position = current_position;
	 auto end_position = end_position_of(match[0]);
	token.emplace<bool_token>(bool_token{start_position,end_position,res});	
	
		return true;
	
	
}

template<typename TokenType>
bool Scanner::default_rule      (std::smatch match, token_ty& token) {
    auto start_position = current_position;
    auto end_position = end_position_of(match[0]);
    token.emplace<TokenType>(TokenType{start_position,end_position,match[0]});

    return true;
}

bool Scanner::default_ignore    (std::smatch match, token_ty& token) { 
    return true;
}
