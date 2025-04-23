#pragma once
#include <vector>
#include "token.hh"
#include "ast.hh"

class Parser{
    public:
        Parser(const std::vector<Token>& tokens)
            :_tokens(tokens){}

        void ParseProgram();

        const Program& program() const {return _program;};

    private:
        void ParseStatement();
        void ParseDeclaration();
        void ConsumeToken();
        void ExpectToken(const TokenType& tok);

    private:
        int _current_token_index;
        Token _current_token;
        Token _next_token;
        std::vector<Token> _tokens;
        Program _program;
};