#pragma once
#include <memory>
#include <vector>
#include "token.hh"
#include "ast.hh"

class Parser {
 public:
  Parser(const std::vector<Token>& tokens)
    : _tokens(tokens) {}

  void ParseProgram();

  const Program& program() const {
    return _program;
  };

 private:
  std::unique_ptr<Instruction> ParseInstruction();
  std::unique_ptr<Expression> ParseExpression();
  std::unique_ptr<ReturnStatement> ParseReturnStatement();
  void ParseStatement();
  std::unique_ptr<Declaration> ParseDeclaration(Type declaration_type);
  void ParseFunctionArguments();
  std::unique_ptr<FunctionDeclaration> ParseFunctionDeclaration(
      Type declaration_type, std::unique_ptr<Identifier> ident);
  void ConsumeToken();

  void ExpectToken(const TokenType& tok);
  void CheckCurToken(const TokenType& tok);

 private:
  int _current_token_index;
  Token _current_token;
  Token _next_token;
  std::vector<Token> _tokens;
  Program _program;
};
