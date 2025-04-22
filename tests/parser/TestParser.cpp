#include "lexer.hh"
#include "token.hh"
#include "parser.hh"
#include "ast.hh"
#include <iostream>
#include <string>
#include <gtest/gtest.h>

TEST(ParserTest, BasicIntDeclaration){
  std::string input = "int test = 34;";
  Lexer lexer(input);
  std::vector<Token> tokens = lexer.Tokens();
  Parser parser(tokens);
  Program p = parser.ParseProgram();
  ASSERT_EQ(p.instructions().size(), 1);
}

// TEST(ParserTest, BasicProgramTest){
//   std::string input = R"(int main(){
//       return 0;
//     })";
//   Lexer lexer(input);
//   std::vector<Token> tokens = lexer.Tokens();
//   std::vector<TokenType> expected_tokens = {
//       TokenType::kInt, TokenType::kMain, TokenType::kLeftParenthesis,
//       TokenType::kRightParenthesis, TokenType::kReturn, TokenType::kNumber,
//       TokenType::kSemiColon, TokenType::kRightBracket
//   };
//   ASSERT_EQ(tokens.size(), expected_tokens.size());
//   for (int i = 0; i < tokens.size(); i++){
//     EXPECT_EQ(tokens[i].type, expected_tokens[i]);
//   }
// }
