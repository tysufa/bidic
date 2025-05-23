#include "lexer.hh"
#include "token.hh"
#include <gtest/gtest.h>
#include <iostream>
#include <string>

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
  std::vector<TokenType> expected_tokens = {TokenType::kInt,
                                            TokenType::kMain,
                                            TokenType::kLeftParenthesis,
                                            TokenType::kRightParenthesis,
                                            TokenType::kLeftBracket,
                                            TokenType::kReturn,
                                            TokenType::kNumber,
                                            TokenType::kSemiColon,
                                            TokenType::kRightBracket};
  ASSERT_EQ(tokens.size(), expected_tokens.size());

  for (int i = 0; i < tokens.size(); i++) {
    EXPECT_EQ(tokens[i].type, expected_tokens[i])
        << "tokens[" << i << "].value = " << tokens[i].value << "\nexpected "
        << StringTokenType(tokens[i].type) << " got "
        << StringTokenType(expected_tokens[i]) << "instead\n";
  }
}

TEST(LexerTest, UnaryOperator) {
  std::string input = "return ~(-2);";
  Lexer lexer(input);
  std::vector<Token> tokens = lexer.Tokens();
  ASSERT_EQ(tokens.size(), 7);
  EXPECT_EQ(lexer.input(), input);
  EXPECT_EQ(tokens[0].type, TokenType::kReturn);
  EXPECT_EQ(tokens[1].type, TokenType::kTilde);
  EXPECT_EQ(tokens[2].type, TokenType::kLeftParenthesis);
  EXPECT_EQ(tokens[3].type, TokenType::kMinus);
  EXPECT_EQ(tokens[4].type, TokenType::kNumber);
  EXPECT_EQ(tokens[5].type, TokenType::kRightParenthesis);
  EXPECT_EQ(tokens[6].type, TokenType::kSemiColon);

  input = "return --2;";
  Lexer lexer2(input);
  tokens = lexer2.Tokens();
  ASSERT_EQ(tokens.size(), 4);
  EXPECT_EQ(tokens[0].type, TokenType::kReturn);
  EXPECT_EQ(tokens[1].type, TokenType::kDecrement);
  EXPECT_EQ(tokens[2].type, TokenType::kNumber);
  EXPECT_EQ(tokens[3].type, TokenType::kSemiColon);
}

TEST(LexerTest, BinaryOperators) {
  std::string input = "return 1+2*3/4;";
  Lexer lexer(input);
  std::vector<Token> tokens = lexer.Tokens();
  ASSERT_EQ(tokens.size(), 9);
  EXPECT_EQ(lexer.input(), input);
  EXPECT_EQ(tokens[0].type, TokenType::kReturn);
  EXPECT_EQ(tokens[1].type, TokenType::kNumber);
  EXPECT_EQ(tokens[2].type, TokenType::kPlus);
  EXPECT_EQ(tokens[3].type, TokenType::kNumber);
  EXPECT_EQ(tokens[4].type, TokenType::kStar);
  EXPECT_EQ(tokens[5].type, TokenType::kNumber);
  EXPECT_EQ(tokens[6].type, TokenType::kSlash);
  EXPECT_EQ(tokens[7].type, TokenType::kNumber);
  EXPECT_EQ(tokens[8].type, TokenType::kSemiColon);
}

TEST(AsmLexerTest, BasicProgramTest) {
  std::string input = R"(func:
    mov eax, identifier
    imul [ebp-4], 4
    ret
    )";
  Lexer lexer(input,false);
  std::vector<Token> tokens = lexer.Tokens();
  // ASSERT_EQ(tokens.size(), 21);
  EXPECT_EQ(lexer.input(), input);
  int i=0;
  EXPECT_EQ(tokens[i++].type, TokenType::kIdentifier);
  EXPECT_EQ(tokens[i++].type, TokenType::kColon);
  EXPECT_EQ(tokens[i++].type, TokenType::kNL);
  EXPECT_EQ(tokens[i++].type, TokenType::kMov);
  EXPECT_EQ(tokens[i++].type, TokenType::kRegister);
  EXPECT_EQ(tokens[i++].type, TokenType::kComma);
  EXPECT_EQ(tokens[i++].type, TokenType::kIdentifier); // not supposed to work in practice
  EXPECT_EQ(tokens[i++].type, TokenType::kNL);
  EXPECT_EQ(tokens[i++].type, TokenType::kImul);
  EXPECT_EQ(tokens[i++].type, TokenType::kLeftSquareBracket);
  EXPECT_EQ(tokens[i++].type, TokenType::kRegister);
  EXPECT_EQ(tokens[i++].type, TokenType::kMinus);
  EXPECT_EQ(tokens[i++].type, TokenType::kNumber);
  EXPECT_EQ(tokens[i++].type, TokenType::kRightSquareBracket);
  EXPECT_EQ(tokens[i++].type, TokenType::kComma);
  EXPECT_EQ(tokens[i++].type, TokenType::kNumber);
  EXPECT_EQ(tokens[i++].type, TokenType::kNL);
  EXPECT_EQ(tokens[i++].type, TokenType::kReturn);
  EXPECT_EQ(tokens[i++].type, TokenType::kNL);
  EXPECT_EQ(tokens[i++].type, TokenType::kEof);
}