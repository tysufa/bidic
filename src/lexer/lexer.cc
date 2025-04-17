#include "lexer.hh"

#include "iostream"

bool Lexer::IsAlphaNum(char ch){
    return (('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z') || ('0' <= ch && ch <= '9'));
}

void Lexer::NextChar(int & i){
    i++;
    _current_ch = _next_ch;
    if (i+1 < _input.size()){
        _next_ch = _input[i+1];
    } else {
        _next_ch = 0;
    }
}

std::vector<Token> Lexer::Tokens(){
    std::vector<Token> tokens;

    Token current_token = {.type = TokenType::kVariable, .value = ""};
    // if input is empty, initialize current_ch at 0
    _current_ch = (0 < _input.size()) ? _input[0] : 0;
    // if there is less than 2 characters, initialize next_ch at 0
    _next_ch = (1 < _input.size()) ? _input[1] : 0;

    std::string s;
    s = '=';
    int i = 0;
    while (i < _input.size()) {
        // when keywords are single characters we need a string
        s = _current_ch;

        if (Keywords.contains(s)){
            current_token.value = s;
            current_token.type = Keywords[current_token.value];
            tokens.push_back(current_token);
            current_token.value = "";
            NextChar(i);
        } else if (('a' <= _current_ch && _current_ch <= 'z') || ('A' <= _current_ch && _current_ch <= 'Z')){
            current_token.value += _current_ch;
            NextChar(i);
            while (IsAlphaNum(_current_ch)){
                current_token.value += _current_ch;
                NextChar(i);
            }

            if (Keywords.contains(current_token.value)){
                current_token.type = Keywords[current_token.value];
                tokens.push_back(current_token);
                current_token.value = "";
            } else {
                current_token.type = TokenType::kVariable;
                tokens.push_back(current_token);
                current_token.value = "";
            }
        } else if (' ' == _current_ch || '\n' == _current_ch || '\r' == _current_ch){
            NextChar(i);
        } else if ('0' <= _current_ch && _current_ch <= '9'){
            current_token.value += _current_ch;
            NextChar(i);
            while ('0' <= _current_ch && _current_ch <= '9'){
                current_token.value += _current_ch;
                NextChar(i);
            }

            current_token.type = TokenType::kNumber;
            tokens.push_back(current_token);
            current_token.value = "";
        } else if (_current_ch = 0){
            break;
        }else {
            std::cout << "_current_ch = " << _current_ch << std::endl;
            NextChar(i);
        }


        // switch (current_ch)
        // {
        // case ' ':
        //     if (Keywords.contains(current_token.value)){
        //         current_token.type = Keywords[current_token.value];
        //         tokens.push_back(current_token);
        //         current_token.value = "";
        //     } else {
        //         current_token.type = TokenType::kVariable;
        //         tokens.push_back(current_token);
        //         current_token.value = "";
        //     }
        //     break;

        // default:
        //     current_token.value += _input[i];
        //     break;
        // }
    }


    return tokens;
}