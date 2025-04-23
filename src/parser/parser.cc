#include "parser.hh"
#include <stdexcept>


#include <iostream>

void Parser::ParseProgram(){
    Program p;
    _current_token_index = -2;
    ConsumeToken();
    ConsumeToken();

    switch (_current_token.type){
        case TokenType::kInt:
            ParseDeclaration(Type::kInt);
            break;
    }

}

void Parser::ParseDeclaration(Type declaration_type){
  ExpectToken(TokenType::kIdentifier);

  auto ident = std::make_unique<Identifier>(_current_token.value);
  auto instr = std::make_unique<Declaration>(std::move(ident), declaration_type);

  if (_next_token.type == TokenType::kEqual){
    ConsumeToken();
    switch (declaration_type){
        case Type::kInt:
            ExpectToken(TokenType::kNumber);
            ExpectToken(TokenType::kSemiColon);
            break;
        default:
            break;
    }
  } else if (_next_token.type == TokenType::kLeftParenthesis){

  } else{
    ExpectToken(TokenType::kSemiColon);
  }

  _program.add_instruction(std::move(instr));
}

void Parser::ParseStatement(){

}

void Parser::ConsumeToken(){
    _current_token_index++;
    _current_token = _next_token;
    if (_current_token_index+1 < _tokens.size())
        _next_token = _tokens[_current_token_index+1];
    else
        _next_token.type = TokenType::kEof;
}

void Parser::ExpectToken(const TokenType& tok){
    if (_next_token.type != tok){
      throw std::runtime_error("expected " + StringTokenType(tok) + ", got " +
                               StringTokenType(_next_token.type) + " instead");
    } else {
        ConsumeToken();
    }
}