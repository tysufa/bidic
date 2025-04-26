#include "lexer.hh"
#include "token.hh"
#include <iostream>
#include <string>
#include <gtest/gtest.h>

TEST(LexerTest, BasicIntDeclaration) {
  std::string input = "int test = 34;";
  Lexer lexer(input);
  std::vector<Token> tokens = lexer.Tokens();
  ASSERT_EQ(tokens.size(), 5);
  EXPECT_EQ(lexer.input(), input);
  EXPECT_EQ(tokens[0].type, TokenType::kInt);
  EXPECT_EQ(tokens[1].type, TokenType::kIdentifier);
  EXPECT_EQ(tokens[1].value, "test");
  EXPECT_EQ(tokens[2].type, TokenType::kEqual);
  EXPECT_EQ(tokens[3].type, TokenType::kNumber);
  EXPECT_EQ(tokens[3].value, "34");
  EXPECT_EQ(tokens[4].type, TokenType::kSemiColon);
}

TEST(LexerTest, BasicProgramTest) {
  std::string input = R"(int main(){
      return 0;
    })";
  Lexer lexer(input);
  std::vector<Token> tokens = lexer.Tokens();
  std::vector<TokenType> expected_tokens = {
    TokenType::kInt, TokenType::kMain, TokenType::kLeftParenthesis,
    TokenType::kRightParenthesis, TokenType::kLeftBracket, TokenType::kReturn,
    TokenType::kNumber, TokenType::kSemiColon, TokenType::kRightBracket
  };
  ASSERT_EQ(tokens.size(), expected_tokens.size());
  for (int i = 0; i < tokens.size(); i++) {
    EXPECT_EQ(tokens[i].type, expected_tokens[i]) << "tokens[" << i
      << "].value = " << tokens[i].value << "\nexpected " <<
      StringTokenType(tokens[i].type) << " got " <<
      StringTokenType(expected_tokens[i]) << "instead\n";
  }
}
