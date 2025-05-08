#include "parser.hh"
#include "ast.hh"
#include "token.hh"
#include <iostream>
#include <memory>
#include <stdexcept>

std::unique_ptr<Program> Parser::ParseProgram() {
  auto program = std::make_unique<Program>();
  _current_token_index = -2;
  ConsumeToken();
  ConsumeToken();

  std::unique_ptr<Instruction> instr = ParseInstruction();
  program->add_instruction(std::move(instr));

  return program;
}

std::unique_ptr<Expression> Parser::ParseExpression() {

  std::unique_ptr<Expression> left = ParsePrefix();

  while (_current_token.type != TokenType::kEof or
         _current_token.type != TokenType::kIllegal) {

    if (!IsOperator(_current_token.type))
      return left;

  }

  return left;

  // switch (_current_token.type) {
  //   case TokenType::kNumber:
  //     return std::make_unique<IntExpression>(std::stoi(_current_token.value));
  //     break;
  //
  //   default:
  //     throw std::runtime_error("expected Expression got " + StringTokenType(
  //                                  _current_token.type) + " instead");
  //     return nullptr;
  //     break;
  // }
}

std::unique_ptr<Expression> Parser::ParsePrefix() {
  TokenType prefix_type;

  if (_current_token.type == TokenType::kMinus) {
    prefix_type = TokenType::kMinus;
    std::unique_ptr<PrefixExpression> prefix;
    ConsumeToken();

    std::unique_ptr<Expression> expr = ParseExpression();

    prefix = std::make_unique<PrefixExpression>(prefix_type, std::move(expr));

    return prefix;

    // return std::make_unique<PrefixExpression>(prefix_type,
    //        std::make_unique<IntExpression>(std::stoi(_current_token.value)));
  } else if (_current_token.type == TokenType::kTilde) {
    prefix_type = TokenType::kTilde;
    ConsumeToken();
  } else if (_current_token.type == TokenType::kNumber) {
    auto res = std::make_unique<IntExpression>(std::stoi(_current_token.value));
    return res;
  } else if (_current_token.type == TokenType::kLeftParenthesis) {
    ConsumeToken();
    auto res = ParseExpression();
    ConsumeToken();
    return res;
  } else {
    throw std::runtime_error("expected prefix token got " + StringTokenType(
                                 _current_token.type) + " instead");
  }

  throw _current_token;
}


std::unique_ptr<ReturnStatement> Parser::ParseReturnStatement() {
  ConsumeToken();


  std::unique_ptr<Expression> res = ParseExpression();

  auto return_statement = std::make_unique<ReturnStatement>(std::move(res));

  ExpectToken(TokenType::kSemiColon);
  ConsumeToken();

  return return_statement;
}

void Parser::ParseFunctionArguments() {
  ExpectToken(TokenType::kLeftParenthesis);
  ExpectToken(TokenType::kRightParenthesis);

  ExpectToken(TokenType::kLeftBracket);
  ConsumeToken();
}

std::unique_ptr<FunctionDeclaration> Parser::ParseFunctionDeclaration(
    Type declaration_type, std::unique_ptr<Identifier> ident) {

  auto function = std::make_unique<FunctionDeclaration>(std::move(ident),
                  declaration_type);

  ParseFunctionArguments();

  std::unique_ptr<Instruction> function_instruction = ParseInstruction();
  function->add_instruction(std::move(function_instruction));

  while (_current_token.type != TokenType::kRightBracket) {
    function_instruction = ParseInstruction();
    function->add_instruction(std::move(function_instruction));
  }

  CheckCurToken(TokenType::kRightBracket);

  return function;
}


std::unique_ptr<Declaration> Parser::ParseDeclaration(Type declaration_type) {
  ExpectToken(TokenType::kIdentifier);

  auto ident = std::make_unique<Identifier>(_current_token.value);
  std::unique_ptr<Declaration> instr;

  if (_next_token.type == TokenType::kEqual) {
    instr = std::make_unique<Declaration>(std::move(ident), declaration_type);
    ConsumeToken();

    switch (declaration_type) {
      case Type::kInt:
        ExpectToken(TokenType::kNumber);
        ExpectToken(TokenType::kSemiColon);
        ConsumeToken();
        break;

      default:
        return nullptr;
        break;
    }

  } else if (_next_token.type == TokenType::kLeftParenthesis)
    instr = ParseFunctionDeclaration(declaration_type, std::move(ident));


  else
    ExpectToken(TokenType::kSemiColon);

  return instr;
}

std::unique_ptr<Instruction> Parser::ParseInstruction() {
  switch (_current_token.type) {
    case TokenType::kReturn:
      return ParseReturnStatement();
      break;

    case TokenType::kInt:
      return ParseDeclaration(Type::kInt);
      break;

    default:
      throw std::runtime_error("Expected instruction, got " + StringTokenType(
                                   _current_token.type) + " instead");
  }

}

void Parser::ParseStatement() {
}

void Parser::ConsumeToken() {
  _current_token_index++;
  _current_token = _next_token;

  if (_current_token_index + 1 < _tokens.size())
    _next_token = _tokens[_current_token_index + 1];

  else
    _next_token.type = TokenType::kEof;
}

void Parser::ExpectToken(const TokenType & tok) {
  if (_next_token.type != tok) {
    throw std::runtime_error("expected " + StringTokenType(tok) + ", got " +
                             StringTokenType(_next_token.type) + " instead");
  } else
    ConsumeToken();
}


void Parser::CheckCurToken(const TokenType& tok) {
  if (_current_token.type != tok) {
    throw std::runtime_error("expected current token " + StringTokenType(
                                 tok) + ", got " +
                             StringTokenType(_next_token.type) + " instead");
  } else
    ConsumeToken();
}
