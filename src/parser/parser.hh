#pragma once
#include <vector>
#include "token.hh"
#include "ast.hh"

class Parser{
    public:
        Parser(const std::vector<Token>& tokens)
            :_tokens(tokens){}

        Program ParseProgram();
        void ParseStatement();
        void ConsumeToken();
        int ExpectToken(const Token& tok);

    private:
        Token _current_token;
        Token _next_token;
        std::vector<Token> _tokens;
};