#include "parser.hh"
#include "ast.hh"
#include "token.hh"
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

std::unique_ptr<ast::Program> Parser::ParseProgram() {
  auto program = std::make_unique<ast::Program>();
  _current_token_index = -2;
  ConsumeToken();
  ConsumeToken();

  std::unique_ptr<ast::Instruction> instr = ParseInstruction();
  program->add_instruction(std::move(instr));

  return program;
}

int Parser::Precedence(TokenType tok) {
  if (tok == TokenType::kStar or tok == TokenType::kSlash)
    return 50;
  else if (tok == TokenType::kPlus or tok == TokenType::kMinus or
           tok == TokenType::kTilde)
    return 45;
  else if (tok == TokenType::kLeftParenthesis)
    return 10;
  else
    throw std::invalid_argument("expected operator, got " +
                                StringTokenType(tok) + " instead");
}

std::unique_ptr<ast::Expression> Parser::ParseExpression(int precedenceLimit) {

  std::unique_ptr<ast::Expression> left = ParsePrefix();

  while (_current_token.type != TokenType::kEof or
         _current_token.type != TokenType::kIllegal) {

    if (!IsOperator(_current_token.type))
      return left;

    int prec = Precedence(_current_token.type);

    if (prec > precedenceLimit) {
      TokenType op = _current_token.type;
      ConsumeToken();
      std::unique_ptr<ast::Expression> right = ParseExpression(prec);
      left = std::make_unique<ast::BinaryExpression>(op, std::move(left),
             std::move(right));
    } else
      return left;
  }

  return left;
}

std::unique_ptr<ast::Expression> Parser::ParsePrefix() {
  TokenType prefix_type;
  std::unique_ptr<ast::PrefixExpression> prefix;
  std::unique_ptr<ast::Expression> expr;

  if (_current_token.type == TokenType::kMinus) {
    prefix_type = TokenType::kMinus;
    ConsumeToken();

    // get rest of the expression
    expr = ParseExpression(Precedence(prefix_type));

    prefix = std::make_unique<ast::PrefixExpression>(prefix_type, std::move(expr));

    return prefix;

  } else if (_current_token.type == TokenType::kTilde) {
    prefix_type = TokenType::kTilde;
    ConsumeToken();

    expr = ParseExpression(Precedence(prefix_type));

    prefix = std::make_unique<ast::PrefixExpression>(prefix_type, std::move(expr));

    return prefix;

  } else if (_current_token.type == TokenType::kNumber) {
    auto res = std::make_unique<ast::Constant>(std::stoi(_current_token.value));
    ConsumeToken();
    return res;
  } else if (_current_token.type == TokenType::kLeftParenthesis) {
    ConsumeToken();
    expr = ParseExpression(Precedence(TokenType::kLeftParenthesis));
    ConsumeToken();
    return expr;
  } else {
    throw std::runtime_error("expected prefix token got " +
                             StringTokenType(_current_token.type) + " instead");
  }
}

std::unique_ptr<ast::ReturnStatement> Parser::ParseReturnStatement() {
  ConsumeToken();

  std::unique_ptr<ast::Expression> res = ParseExpression(0);

  auto return_statement = std::make_unique<ast::ReturnStatement>(std::move(res));

  CheckCurToken(TokenType::kSemiColon);

  return return_statement;
}

void Parser::ParseFunctionArguments() {
  ExpectToken(TokenType::kLeftParenthesis);
  ExpectToken(TokenType::kRightParenthesis);

  ExpectToken(TokenType::kLeftBracket);
  ConsumeToken();
}

std::unique_ptr<ast::FunctionDeclaration>
Parser::ParseFunctionDeclaration(Type declaration_type,
                                 std::unique_ptr<ast::Identifier> ident) {

  auto function =
      std::make_unique<ast::FunctionDeclaration>(std::move(ident), declaration_type);

  ParseFunctionArguments();

  std::unique_ptr<ast::Instruction> function_instruction = ParseInstruction();
  function->add_instruction(std::move(function_instruction));

  while (_current_token.type != TokenType::kRightBracket) {
    function_instruction = ParseInstruction();
    function->add_instruction(std::move(function_instruction));
  }

  CheckCurToken(TokenType::kRightBracket);

  return function;
}

std::unique_ptr<ast::Declaration> Parser::ParseDeclaration(
    Type declaration_type) {
  ExpectToken(TokenType::kIdentifier);

  auto ident = std::make_unique<ast::Identifier>(_current_token.value);
  std::unique_ptr<ast::Declaration> instr;

  if (_next_token.type == TokenType::kEqual) {
    instr = std::make_unique<ast::Declaration>(std::move(ident), declaration_type);
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

std::unique_ptr<ast::Instruction> Parser::ParseInstruction() {
  switch (_current_token.type) {
    case TokenType::kReturn:
      return ParseReturnStatement();
      break;

    case TokenType::kInt:
      return ParseDeclaration(Type::kInt);
      break;

    default:
      throw std::runtime_error("Expected instruction, got " +
                               StringTokenType(_current_token.type) + " instead");
  }
}

void Parser::ParseStatement() {}

void Parser::ConsumeToken() {
  _current_token_index++;
  _current_token = _next_token;

  if (_current_token_index + 1 < _tokens.size())
    _next_token = _tokens[_current_token_index + 1];

  else
    _next_token.type = TokenType::kEof;
}

void Parser::ExpectToken(const TokenType &tok) {
  if (_next_token.type != tok) {
    throw std::runtime_error("expected " + StringTokenType(tok) + ", got " +
                             StringTokenType(_next_token.type) + " instead");
  } else
    ConsumeToken();
}

void Parser::CheckCurToken(const TokenType &tok) {
  if (_current_token.type != tok) {
    throw std::runtime_error("expected current token " + StringTokenType(tok) +
                             ", got " + StringTokenType(_current_token.type) +
                             " instead");
  } else
    ConsumeToken();
}
