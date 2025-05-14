#pragma once
#include "ast.hh"
#include "token.hh"
#include <memory>
#include <vector>

using namespace ast;

class Parser {
 public:
  Parser(const std::vector<Token>& tokens) : _tokens(tokens) {}

  std::unique_ptr<ast::Program> ParseProgram();

  std::unique_ptr<ast::Program> program() { return std::move(_program); };

 private:
  std::unique_ptr<ast::Instruction> ParseInstruction();

  int Precedence(TokenType tok);
  std::unique_ptr<ast::Expression> ParseExpression(int precedenceLimit);

  std::unique_ptr<ast::ReturnStatement> ParseReturnStatement();
  void ParseStatement();
  std::unique_ptr<ast::Declaration> ParseDeclaration(Type declaration_type);
  void ParseFunctionArguments();
  std::unique_ptr<ast::FunctionDeclaration>
  ParseFunctionDeclaration(Type declaration_type,
                           std::unique_ptr<ast::Identifier> ident);
  void ConsumeToken();

  void ExpectToken(const TokenType &tok);
  void CheckCurToken(const TokenType &tok);

  std::unique_ptr<ast::Expression> ParsePrefix();
  bool IsOperator(TokenType t) {
    return t == TokenType::kPlus or t == TokenType::kMinus or
           t == TokenType::kStar or t == TokenType::kSlash;
  }

 private:
  int _current_token_index;
  Token _current_token;
  Token _next_token;
  std::vector<Token> _tokens;
  std::unique_ptr<ast::Program> _program;
};
