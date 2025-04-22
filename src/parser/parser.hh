#pragma once
#include <vector>
#include "token.hh"

class Parser{
    public:
        Parser()=default;

        void ParseProgram();
        void ParseStatement();
        void ConsumeToken();
        int ExpectToken(const Token& tok);

    private:
        Token _current_token;
        Token _next_token;
        std::vector<Token> _tokens;
};