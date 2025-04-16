#include "lexer.hh"

std::vector<Token> Lexer::Tokens(){
    std::vector<Token> tokens;

    Token current_token = {.type = TokenType::kVariable, .value = ""};
    for (int i = 0; i < _input.size(); i++){
        switch (_input[i])
        {
        case ' ':
            if (Keywords.contains(current_token.value)){
                current_token.type = Keywords[current_token.value];
                tokens.push_back(current_token);
                current_token.value = "";
            } else {
                current_token.type = TokenType::kVariable;
                tokens.push_back(current_token);
                current_token.value = "";
            }
            break;

        default:
            current_token.value += _input[i];
            break;
        }
    }


    return tokens;
}