#pragma once
#include <string>

class Lexer{
public:
    Lexer(std::string const & input)
        :_input(input){}
    
    void tempo();

    std::string const & input() {return _input;};

private:
    std::string _input;

};