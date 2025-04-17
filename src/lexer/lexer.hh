#pragma once
#include "token.hh"
#include <string>
#include <vector>

/// @brief Lexer class, take an input and generate a token vector
///
/// @param input the code you want to compile
class Lexer{
public:
    Lexer(std::string const & input)
        :_input(input){}
    
    std::string const & input() {return _input;};

    std::vector<Token> Tokens();

private:
    bool IsAlphaNum(char ch);

    /// @brief update _current_ch and _next_ch
    void NextChar(int & i);

private:
    std::string _input;
    char _current_ch;
    char _next_ch;
};