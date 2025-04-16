#include <iostream>
#include "lexer/lexer.hh"

int main(){

    Lexer tempo("int a = 3;");

    tempo.tempo();

    std::cout << "hello world" << std::endl;

    return 0;
}