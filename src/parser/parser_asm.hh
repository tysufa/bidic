#pragma once
#include "../scav/scav.hh"
#include "token.hh"
#include <memory>
#include <vector>

class Parser_asm {
 public:
  Parser_asm(const std::vector<Token>& tokens) : _tokens(tokens) {}

  std::unique_ptr<scav::Program> ParseProgram();

 private:
  std::shared_ptr<scav::Instruction> ParseInstruction();
  std::shared_ptr<scav::Return> ParseReturn();
  std::shared_ptr<scav::FunctionDeclaration> ParseFunctionDeclaration();

  std::shared_ptr<scav::Move> ParseMove();
  std::shared_ptr<scav::Mult> ParseMult();
  std::shared_ptr<scav::Add> ParseAdd();
  std::shared_ptr<scav::Neg> ParseNeg();
  std::shared_ptr<scav::Not> ParseNot();

  std::string ParseRegister();

  std::string ConsumeToken();
  std::string CheckCurToken(const TokenType &tok);

 private:
  int _current_token_index;
  Token _current_token;
  Token _next_token;
  std::vector<Token> _tokens;
};