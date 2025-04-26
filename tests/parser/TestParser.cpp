#include "lexer.hh"
#include "token.hh"
#include "parser.hh"
#include "ast.hh"
#include <iostream>
#include <string>
#include <gtest/gtest.h>

TEST(ParserTest, BasicIntDeclaration) {
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

  if (pDeclaration) {
    EXPECT_EQ(pDeclaration->identifier()->name(), "test");
    EXPECT_EQ(pDeclaration->type(), Type::kInt);
  }
}

TEST(ParserTest, BasicProgramTest) {
  std::string input = R"(int test(){
    return 0;
    int feur = 3;
    return 10;
    })";
  Lexer lexer(input);
  std::vector<Token> tokens = lexer.Tokens();

  Parser parser(tokens);
  parser.ParseProgram();
  const Program& p = parser.program();

  const std::vector<std::unique_ptr<Instruction>>& instructions = p.instructions();

  auto pDeclaration = dynamic_cast<FunctionDeclaration const*>
                      (instructions[0].get());

  ASSERT_EQ(instructions.size(), 1);

  EXPECT_EQ(instructions[0]->TypeInstruction(), "FunctionDeclaration");
  EXPECT_NE(pDeclaration, nullptr) << "Expected non-null Declaration pointer";

  if (pDeclaration) {
    EXPECT_EQ(pDeclaration->identifier()->name(), "test");
    EXPECT_EQ(pDeclaration->type(), Type::kInt);


    ASSERT_EQ(pDeclaration->instructions().size(), 3);

    EXPECT_EQ(pDeclaration->instructions()[0]->TypeInstruction(),
              "ReturnStatement");

    auto p_return_statement = dynamic_cast<ReturnStatement const*>
                              (pDeclaration->instructions()[0].get());

    EXPECT_NE(p_return_statement,
              nullptr) << "Expected non-null return statement pointer";

    if (p_return_statement)
      EXPECT_EQ(p_return_statement->return_value()->Evaluate()->DebugResult(), "0");

  }

}
