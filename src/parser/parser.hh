#pragma once
#include <memory>
#include <vector>
#include "token.hh"
#include "ast.hh"

class Parser {
 public:
  Parser(const std::vector<Token>& tokens)
    : _tokens(tokens) {}

  std::unique_ptr<Program> ParseProgram();

  std::unique_ptr<Program> program() {
    return std::move(_program);
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

  std::unique_ptr<Expression> ParsePrefix();
  int Precedence(TokenType t) {
    if (t == TokenType::kPlus or t == TokenType::kMinus)
      return 2;
    else if (t == TokenType::kStar or t == TokenType::kSlash)
      return 3;
    else if (t == TokenType::kLeftParenthesis or t == TokenType::kRightParenthesis)
      return 1;
    else
      return 0;
  }
  bool IsOperator(TokenType t) {
    return t == TokenType::kPlus or t == TokenType::kMinus or
           t == TokenType::kStar or t == TokenType::kSlash;
  }


 private:
  int _current_token_index;
  Token _current_token;
  Token _next_token;
  std::vector<Token> _tokens;
  std::unique_ptr<Program> _program;
};
