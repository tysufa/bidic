#pragma once
#include "token.hh"
#include <string>
#include <vector>

class Lexer{
public:
    Lexer(std::string const & input)
        :_input(input){}
    
    std::string const & input() {return _input;};

    std::vector<Token> Tokens();

private:
    std::string _input;
};