#include "ast.hh"
#include "lexer.hh"
#include "parser.hh"
#include "token.hh"
#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <string>

TEST(ParserTest, BasicIntDeclaration) {
  std::string input = "int test = 34;";
  Lexer lexer(input);
  std::vector<Token> tokens = lexer.Tokens();
  Parser parser(tokens);
  std::unique_ptr<Program> p = parser.ParseProgram();

  const std::vector<std::unique_ptr<Instruction>> &instructions =
      p->instructions();

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
  std::unique_ptr<Program> p = parser.ParseProgram();

  const std::vector<std::unique_ptr<Instruction>> &instructions =
      p->instructions();

  auto pDeclaration =
      dynamic_cast<FunctionDeclaration const*>(instructions[0].get());

  ASSERT_EQ(instructions.size(), 1);

  EXPECT_EQ(instructions[0]->TypeInstruction(), "FunctionDeclaration");
  EXPECT_NE(pDeclaration, nullptr) << "Expected non-null Declaration pointer";

  if (pDeclaration) {
    EXPECT_EQ(pDeclaration->identifier()->name(), "test");
    EXPECT_EQ(pDeclaration->type(), Type::kInt);

    ASSERT_EQ(pDeclaration->instructions().size(), 3);

    EXPECT_EQ(pDeclaration->instructions()[0]->TypeInstruction(),
              "ReturnStatement");

    auto p_return_statement =
        dynamic_cast<ReturnStatement*>(pDeclaration->instructions()[0].get());

    EXPECT_NE(p_return_statement, nullptr)
        << "Expected non-null return statement pointer";

    if (p_return_statement)
      EXPECT_EQ(std::get<int>(*p_return_statement->return_value()->Evaluate()), 0);
  }
}

TEST(ParserTest, UnaryOperator) {
  {
    std::string input = R"(int test(){
    return -1;
    })";
    Lexer lexer(input);
    std::vector<Token> tokens = lexer.Tokens();

    Parser parser(tokens);
    std::unique_ptr<Program> p = parser.ParseProgram();

    const std::vector<std::unique_ptr<Instruction>> &instructions =
        p->instructions();

    auto pDeclaration =
        dynamic_cast<FunctionDeclaration const*>(instructions[0].get());

    ASSERT_EQ(instructions.size(), 1);

    EXPECT_EQ(instructions[0]->TypeInstruction(), "FunctionDeclaration");
    EXPECT_NE(pDeclaration, nullptr) << "Expected non-null Declaration pointer";

    if (pDeclaration) {
      EXPECT_EQ(pDeclaration->identifier()->name(), "test");
      EXPECT_EQ(pDeclaration->type(), Type::kInt);

      ASSERT_EQ(pDeclaration->instructions().size(), 1);

      EXPECT_EQ(pDeclaration->instructions()[0]->TypeInstruction(),
                "ReturnStatement");

      auto p_return_statement = dynamic_cast<ReturnStatement*>(
                                    pDeclaration->instructions()[0].get());

      EXPECT_NE(p_return_statement, nullptr)
          << "Expected non-null return statement pointer";

      if (p_return_statement) {
        auto p_prefix_expr = dynamic_cast<PrefixExpression const*>(
                                 p_return_statement->return_value().get());
        EXPECT_NE(p_prefix_expr, nullptr)
            << "Expected non-null PrefixExpression pointer";
        EXPECT_EQ(std::get<int>(*p_return_statement->return_value()->Evaluate()), -1);
      }
    }
  }
  {
    std::string input = R"(int test(){
    return ~(-3);
    })";
    Lexer lexer(input);
    std::vector<Token> tokens = lexer.Tokens();

    Parser parser(tokens);
    std::unique_ptr<Program> p = parser.ParseProgram();

    const std::vector<std::unique_ptr<Instruction>> &instructions =
        p->instructions();

    auto pDeclaration =
        dynamic_cast<FunctionDeclaration const*>(instructions[0].get());

    ASSERT_EQ(instructions.size(), 1);

    auto p_return_statement = dynamic_cast<ReturnStatement const*>(
                                  pDeclaration->instructions()[0].get());

    EXPECT_NE(p_return_statement, nullptr)
        << "Expected non-null return statement pointer";

    if (p_return_statement) {
      auto p_prefix_expr = dynamic_cast<PrefixExpression const*>(
                               p_return_statement->return_value().get());
      EXPECT_NE(p_prefix_expr, nullptr)
          << "Expected non-null PrefixExpression pointer";
      EXPECT_EQ(std::get<int>(*p_return_statement->return_value()->Evaluate()), 2);
    }
  }
}

TEST(ParserTest, BinaryOperators) {
  {
    std::string input = R"(int test(){
    return 1+2*3;
    })";
    Lexer lexer(input);
    std::vector<Token> tokens = lexer.Tokens();

    Parser parser(tokens);
    std::unique_ptr<Program> p = parser.ParseProgram();

    const std::vector<std::unique_ptr<Instruction>> &instructions =
        p->instructions();

    ASSERT_EQ(instructions.size(), 1);

    EXPECT_EQ(instructions[0]->TypeInstruction(), "FunctionDeclaration");

    auto p_function =
        dynamic_cast<FunctionDeclaration const*>(instructions[0].get());

    EXPECT_NE(p_function, nullptr) << "Expected non-null Declaration pointer";

    if (p_function) {
      EXPECT_EQ(p_function->identifier()->name(), "test");
      EXPECT_EQ(p_function->type(), Type::kInt);

      ASSERT_EQ(p_function->instructions().size(), 1);

      EXPECT_EQ(p_function->instructions()[0]->TypeInstruction(),
                "ReturnStatement");

      auto p_return_statement =
          dynamic_cast<ReturnStatement*>(p_function->instructions()[0].get());

      EXPECT_NE(p_return_statement, nullptr)
          << "Expected non-null return statement pointer";

      if (p_return_statement) {
        const auto &return_value = p_return_statement->return_value();

        auto p_bin = dynamic_cast<BinaryExpression const*>(return_value.get());
        EXPECT_NE(p_bin, nullptr);

        if (p_bin) {
          EXPECT_EQ(std::get<int>(*p_bin->Evaluate()), 7);

          EXPECT_EQ(p_bin->operation(), TokenType::kPlus);
          const auto &left = p_bin->left();
          const auto &right = p_bin->right();

          auto p_left = dynamic_cast<Constant const*>(left.get());
          // auto p_left = dynamic_cast<IntExpression const*>(left.get());
          EXPECT_NE(p_left, nullptr);

          if (p_left)
            EXPECT_EQ(std::get<int>(*p_left->Evaluate()), 1);

          auto p_right = dynamic_cast<BinaryExpression const*>(right.get());
          EXPECT_NE(p_right, nullptr);

          if (p_right) {
            EXPECT_EQ(p_right->operation(), TokenType::kStar);

            EXPECT_NE(p_right->left(), nullptr);
            EXPECT_EQ(std::get<int>(*p_right->left()->Evaluate()), 2);

            EXPECT_NE(p_right->right(), nullptr);
            EXPECT_EQ(std::get<int>(*p_right->right()->Evaluate()), 3);
          }
        }
      }
    }
  }
}
