#include "lexer.hh"
#include "token.hh"
#include "parser.hh"
#include "ast.hh"
#include "nast.hh"
#include "ast_parser.hh"
#include <iostream>
#include <memory>
#include <string>
#include <gtest/gtest.h>

TEST(AstParserTest, BasicProgramTest) {
  std::string input = R"(int test(){
    return 1;
    })";
  Lexer lexer(input);
  std::vector<Token> tokens = lexer.Tokens();

  Parser parser(tokens);

  std::unique_ptr<nast::Program> res;
  // the std::move is automatic
  res = eval(parser.ParseProgram());

  const std::vector<std::unique_ptr<nast::Instruction>>& instructions = res->instructions();

  ASSERT_EQ(res->instructions().size(), 1);

  EXPECT_EQ(instructions[0]->TypeInstruction(), "FunctionDeclaration");

  auto pDeclaration = dynamic_cast<nast::FunctionDeclaration const*>
                      (instructions[0].get());

  EXPECT_NE(pDeclaration, nullptr) << "Expected non-null Declaration pointer";

  if (pDeclaration) {
    EXPECT_EQ(pDeclaration->identifier()->name(), "test");


    ASSERT_EQ(pDeclaration->instructions().size(), 1);

    EXPECT_EQ(pDeclaration->instructions()[0]->TypeInstruction(),
              "ReturnInstruction");

    auto p_return_statement = dynamic_cast<nast::Return const*>
                              (pDeclaration->instructions()[0].get());

    EXPECT_NE(p_return_statement,
              nullptr) << "Expected non-null return statement pointer";

    if (p_return_statement) {
      // EXPECT_EQ(p_return_statement->move()->get_register(), Register::eax);
      // EXPECT_EQ(p_return_statement->move()->value()->value(), 1);
      EXPECT_EQ(p_return_statement->return_value()->Evaluate(), 1);
    }
  }
}


TEST(AstParserTest, UnaryOperator) {
  std::string input = R"(int test(){
    return -2;
    })";
  Lexer lexer(input);
  std::vector<Token> tokens = lexer.Tokens();

  Parser parser(tokens);

  std::unique_ptr<nast::Program> res;

  res = eval(parser.ParseProgram());

  const std::vector<std::unique_ptr<nast::Instruction>>& instructions = res->instructions();

  auto pDeclaration = dynamic_cast<nast::FunctionDeclaration const*>
                      (instructions[0].get());

  if (pDeclaration) {
    ASSERT_EQ(pDeclaration->instructions().size(), 1);

    EXPECT_EQ(pDeclaration->instructions()[0]->TypeInstruction(),
              "ReturnInstruction");

    auto p_return_statement = dynamic_cast<nast::Return const*>
                              (pDeclaration->instructions()[0].get());

    EXPECT_NE(p_return_statement,
              nullptr) << "Expected non-null return statement pointer";

    if (p_return_statement) {
      auto p_unary_expression = dynamic_cast<nast::Unary const*>
                                (p_return_statement->return_value().get());

      EXPECT_NE(p_unary_expression, nullptr);

      if (p_unary_expression) {
        EXPECT_EQ(p_unary_expression->unary_operation(), UnaryOperation::kNegate);
        EXPECT_EQ(p_unary_expression->dst()->name(), "tempo.0");

        EXPECT_EQ(p_return_statement->return_value()->Evaluate(), -2);
      }
    }
  }
}
