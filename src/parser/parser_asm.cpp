#include "parser_asm.hh"
#include "token.hh"
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

std::unique_ptr<scug::Program> Parser_asm::ParseProgram() {
  auto program = std::make_unique<scug::Program>();
  _current_token_index = -2;
  ConsumeToken();
  ConsumeToken();

  std::unique_ptr<scug::Instruction> instr = ParseInstruction();
  while(instr){
    program->add_instruction(std::move(instr));
    instr = ParseInstruction();
  }


  return program;
}

std::unique_ptr<scug::Instruction> Parser_asm::ParseInstruction() {
  switch (_current_token.type) {
    // case TokenType::kIdentifier:
    //   return ParseFunctionDeclaration(std::make_unique<scug::Identifier>(_current_token.value));
    //   break;
    case TokenType::kMov:
      return ParseMove();
      break;
    case TokenType::kReturn:
      CheckCurToken(TokenType::kReturn);
      CheckCurToken(TokenType::kNL);
      return std::make_unique<scug::Ret>();
      break;
    case TokenType::kEof:
      return nullptr;
      break;

    default:
      throw std::runtime_error("Expected instruction, got " +
                               StringTokenType(_current_token.type) + " instead");
  }
}

std::unique_ptr<scug::Move> Parser_asm::ParseMove(){
  ConsumeToken();
  std::string reg=ParseRegister();
  CheckCurToken(TokenType::kComma);
  std::string nb=CheckCurToken(TokenType::kNumber);
  CheckCurToken(TokenType::kNL);
  return std::make_unique<scug::Move>(reg,nb);
}

std::string Parser_asm::ParseRegister(){
    if(_current_token.type==TokenType::kRegister)
      return ConsumeToken();
    else{
      CheckCurToken(TokenType::kLeftSquareBracket);
      std::string reg=CheckCurToken(TokenType::kRegister);
      std::string s;
      if(reg=="ebp"){
        s="ebp";
      }
      else throw std::runtime_error("Expected ebp register, got " +
                               StringTokenType(_current_token.type) + "register instead");
      CheckCurToken(TokenType::kMinus);
      s+=CheckCurToken(TokenType::kNumber);
      CheckCurToken(TokenType::kRightSquareBracket);
      return s;
    }
}

std::string Parser_asm::ConsumeToken(){
  std::string s=_current_token.value;
  _current_token_index++;
  _current_token = _next_token;

  if (_current_token_index + 1 < _tokens.size()){
    _next_token = _tokens[_current_token_index + 1];

  }
  else{
    _next_token.type = TokenType::kEof;
  }
  return s;
}

std::string Parser_asm::CheckCurToken(const TokenType &tok) {
  if (_current_token.type == tok) {
    return ConsumeToken();
  } else{
    throw std::runtime_error("expected current token " + StringTokenType(tok) +
                          ", got " + StringTokenType(_current_token.type) +
                          " instead");
  }
    
}
