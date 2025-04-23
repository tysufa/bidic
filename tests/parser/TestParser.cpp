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
  parser.ParseProgram();
  const Program& p = parser.program();

  const std::vector<std::unique_ptr<Instruction>>& instructions = p.instructions();

  ASSERT_EQ(instructions.size(), 1);

  auto pDeclaration = dynamic_cast<Declaration const*>(instructions[0].get());
  EXPECT_EQ(instructions[0]->TypeInstruction(), "Declaration");
  EXPECT_NE(pDeclaration, nullptr) << "Expected non-null Declaration pointer";
  if (pDeclaration){
    EXPECT_EQ(pDeclaration->identifier()->name(), "test");
    EXPECT_EQ(pDeclaration->type(), Type::kInt);
  }
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
