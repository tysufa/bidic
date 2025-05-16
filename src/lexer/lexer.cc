#include "lexer.hh"
#include "matchit.h"

#include "iostream"
#include "token.hh"
#include <cstdio>
#include <string>


bool Lexer::IsAlphaNum(char ch) const {
  return (('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z') ||
          ('0' <= ch && ch <= '9'));
}

void Lexer::NextChar() {
  _position++;
  _current_ch = _next_ch;

  if (_position + 1 < _input.size())
    _next_ch = _input[_position + 1];
  else
    _next_ch = '\0';
}

void Lexer::NewToken(TokenType type, const std::string &value,
                     std::vector<Token>& tokens) {
  Token current_token = {.type = type, .value = value};
  tokens.push_back(current_token);
  NextChar();
}

void Lexer::SkipWhitespaceNL() {
  while (_current_ch == ' ' || _current_ch == '\n' || _current_ch == '\t')
    NextChar();
}

void Lexer::SkipWhitespace() {
  while (_current_ch == ' ' || _current_ch == '\t')
    NextChar();
}

std::vector<Token> Lexer::TokenizeAsm() {
  std::vector<Token> tokens;

  Token current_token = {.type = TokenType::kIdentifier, .value = ""};

  std::string s;
  // initialisation of _current_ch and _next_ch and put _position at 0;
  _position = -2;
  NextChar();
  NextChar();

  bool nl;

  while (_position < _input.size()) {

    SkipWhitespace();

    // when keywords are single characters we need a string
    s = _current_ch;
    nl=true;
    if(_current_ch=='\n'){
      nl=false;
    }


    current_token.value = "";

    switch (_current_ch) {
      case '\n':
        NewToken(TokenType::kNL, s, tokens);
        break;
      case ',' :
        NewToken(TokenType::kComma, s, tokens);
        break;
      case '[' :
        NewToken(TokenType::kLeftSquareBracket, s, tokens);
        break;
      case ']' :
        NewToken(TokenType::kRightSquareBracket, s, tokens);
        break;
      case '-' :
        NewToken(TokenType::kMinus, s, tokens);
        break;
      case ':' :
        NewToken(TokenType::kColon, s, tokens);
        break;

      default:
        if (IsLetter(_current_ch)) {
          while (IsAlphaNum(_current_ch)) {
            current_token.value += _current_ch;
            NextChar();
          }

          if (AsmKeywords.contains(current_token.value)) {
            current_token.type = AsmKeywords[current_token.value];
            tokens.push_back(current_token);
          } else {
            current_token.type = TokenType::kIdentifier;
            tokens.push_back(current_token);
          }

        } else if (IsDigit(_current_ch)) {
          while (IsDigit(_current_ch)) {
            current_token.value += _current_ch;
            NextChar();
          }

          current_token.type = TokenType::kNumber;
          tokens.push_back(current_token);
        } else {
          NewToken(TokenType::kIllegal, s, tokens);
          throw std::invalid_argument(
              "Illegal character received: (ASCII " + std::to_string(static_cast<int>
                  (_current_ch)) + ") : '" + s + "'"
          );
        }
        break;
    }
    SkipWhitespace();
  }
  if(nl) NewToken(TokenType::kNL, "\n", tokens);
  NewToken(TokenType::kEof, "\0", tokens);
  return tokens;

}

std::vector<Token> Lexer::Tokens() {

  if (_is_c_code) {
    std::vector<Token> tokens;

    Token current_token = {.type = TokenType::kIdentifier, .value = ""};

    std::string s;
    // initialisation of _current_ch and _next_ch and put _position at 0;
    _position = -2;
    NextChar();
    NextChar();

    bool run = true;

    while (_position < _input.size() && run) {

      SkipWhitespaceNL();

      // when keywords are single characters we need a string
      s = _current_ch;

      current_token.value = "";

      switch (_current_ch) {
        case 0:
          NewToken(TokenType::kEof, s, tokens);
          break;

        case '+':
          NewToken(TokenType::kPlus, s, tokens);
          break;

        case '*':
          NewToken(TokenType::kStar, s, tokens);
          break;

        case '/':
          NewToken(TokenType::kSlash, s, tokens);
          break;

        case '=':
          NewToken(TokenType::kEqual, s, tokens);
          break;

        case ';':
          NewToken(TokenType::kSemiColon, s, tokens);
          break;

        case '(':
          NewToken(TokenType::kLeftParenthesis, s, tokens);
          break;

        case ')':
          NewToken(TokenType::kRightParenthesis, s, tokens);
          break;

        case '{':
          NewToken(TokenType::kLeftBracket, s, tokens);
          break;

        case '}':
          NewToken(TokenType::kRightBracket, s, tokens);
          break;

        case '~':
          NewToken(TokenType::kTilde, s, tokens);
          break;

        case '-':
          if (_next_ch == '-') {
            NewToken(TokenType::kDecrement, "--", tokens);
            NextChar();
          } else
            NewToken(TokenType::kMinus, s, tokens);

          break;

        default:
          if (IsLetter(_current_ch)) {
            while (IsAlphaNum(_current_ch)) {
              current_token.value += _current_ch;
              NextChar();
            }

            if (CKeywords.contains(current_token.value)) {
              current_token.type = CKeywords[current_token.value];
              tokens.push_back(current_token);
            } else {
              current_token.type = TokenType::kIdentifier;
              tokens.push_back(current_token);
            }

          } else if (IsDigit(_current_ch)) {
            while (IsDigit(_current_ch)) {
              current_token.value += _current_ch;
              NextChar();
            }

            current_token.type = TokenType::kNumber;
            tokens.push_back(current_token);
          } else {
            NewToken(TokenType::kIllegal, s, tokens);
            throw std::invalid_argument(
                "Illegal character received: (ASCII " + std::to_string(static_cast<int>
                    (_current_ch)) + ") : '" + s + "'"
            );
          }

          break;
      }
    }

    return tokens;


  } else
    return TokenizeAsm();


}
