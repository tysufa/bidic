#include <gtest/gtest.h>
#include "lexer.hh"
#include "token.hh"

TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

TEST(HelloTest, OtherAssertion) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

TEST(LexerTest, BasicIntDeclaration){
  std::string input = "int test = 34;";
  Lexer lexer(input);
  std::vector<Token> tokens = lexer.Tokens();
  TokenType test_expected = TokenType::kVariable;
  EXPECT_EQ(lexer.input(), input);
  /*EXPECT_EQ(static_cast<int>(tokens[0].type), static_cast<int>(test_expected));*/
  EXPECT_EQ(tokens[1].type, TokenType::kVariable);
  EXPECT_EQ(tokens[1].value, "test");
  EXPECT_EQ(tokens[2].type, TokenType::kEqual);
  EXPECT_EQ(tokens[3].type, TokenType::kNumber);
  EXPECT_EQ(tokens[3].value, "34");
  EXPECT_EQ(tokens[4].type, TokenType::kSemiColon);
}
