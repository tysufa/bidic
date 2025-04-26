#pragma once
#include "token.hh"
#include <string>
#include <vector>

/// @brief Lexer class, take an input and generate a token vector
///
/// @param input the code you want to compile
class Lexer {
 public:
  Lexer(std::string const & input)
    :_input(input) {}

  std::string const & input() {
    return _input;
  };

  std::vector<Token> Tokens();

 private:
  bool IsAlphaNum(char ch) const;
  bool IsLetter(char ch) const {
    return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z');
  }
  bool IsDigit(char ch) const {
    return ('0' <= ch && ch <= '9');
  }

  /// @brief update _current_ch and _next_ch
  void NextChar();
  void NewToken(TokenType type, const std::string& value,
                std::vector<Token>& tokens);
  void SkipWhitespace();

 private:
  std::string _input;
  char _current_ch;
  char _next_ch;
  int _position;
};
