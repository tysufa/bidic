#pragma once
#include "../scug/scug.hh"
#include "token.hh"
#include <memory>
#include <vector>

class Parser_asm {
 public:
  Parser_asm(const std::vector<Token>& tokens) : _tokens(tokens) {}

  std::unique_ptr<scug::Program> ParseProgram();

  std::unique_ptr<scug::Program> program() { return std::move(_program); };

 private:
  std::unique_ptr<scug::Instruction> ParseInstruction();

//   int Precedence(TokenType tok);
//   std::unique_ptr<scug::Expression> ParseExpression(int precedenceLimit);

//   std::unique_ptr<scug::ReturnStatement> ParseReturnStatement();
//   void ParseStatement();
//   std::unique_ptr<scug::Declaration> ParseDeclaration(Type declaration_type);
//   void ParseFunctionArguments();
  std::unique_ptr<scug::FunctionDeclaration>
  ParseFunctionDeclaration(std::unique_ptr<scug::Identifier> ident);
  std::unique_ptr<scug::Move> ParseMove();
  std::string ParseRegister();
  std::string ConsumeToken();

  std::string CheckCurToken(const TokenType &tok);

//   std::unique_ptr<scug::Expression> ParsePrefix();
//   bool IsOperator(TokenType t) {
//     return t == TokenType::kPlus or t == TokenType::kMinus or
//            t == TokenType::kStar or t == TokenType::kSlash;
//   }

 private:
  int _current_token_index;
  Token _current_token;
  Token _next_token;
  std::vector<Token> _tokens;
  std::unique_ptr<scug::Program> _program;
};