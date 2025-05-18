#pragma once
#include "../scav/scav.hh"
#include "token.hh"
#include <memory>
#include <vector>

class Parser_asm {
 public:
  Parser_asm(const std::vector<Token>& tokens) : _tokens(tokens) {}

  std::unique_ptr<scav::Program> ParseProgram();

  std::unique_ptr<scav::Program> program() { return std::move(_program); };

 private:
  std::unique_ptr<scav::Instruction> ParseInstruction();

//   int Precedence(TokenType tok);
//   std::unique_ptr<scav::Expression> ParseExpression(int precedenceLimit);

  std::unique_ptr<scav::Return> ParseReturn();
//   void ParseStatement();
//   std::unique_ptr<scav::Declaration> ParseDeclaration(Type declaration_type);
//   void ParseFunctionArguments();
  std::unique_ptr<scav::FunctionDeclaration> ParseFunctionDeclaration();
  std::unique_ptr<scav::Move> ParseMove();
  std::string ParseRegister();
  std::string ConsumeToken();

  std::string CheckCurToken(const TokenType &tok);

//   std::unique_ptr<scav::Expression> ParsePrefix();
//   bool IsOperator(TokenType t) {
//     return t == TokenType::kPlus or t == TokenType::kMinus or
//            t == TokenType::kStar or t == TokenType::kSlash;
//   }

 private:
  int _current_token_index;
  Token _current_token;
  Token _next_token;
  std::vector<Token> _tokens;
  std::unique_ptr<scav::Program> _program;
};