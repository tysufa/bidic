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

  auto p_function_declaration = dynamic_cast<nast::FunctionDeclaration const*>
                                (instructions[0].get());

  EXPECT_NE(p_function_declaration, nullptr) << "Expected non-null func pointer";

  if (p_function_declaration) {
    EXPECT_EQ(p_function_declaration->identifier()->name(), "test");


    ASSERT_EQ(p_function_declaration->instructions().size(), 1);

    EXPECT_EQ(p_function_declaration->instructions()[0]->TypeInstruction(),
              "ReturnInstruction");

    auto p_return_statement = dynamic_cast<nast::Return const*>
                              (p_function_declaration->instructions()[0].get());

    EXPECT_NE(p_return_statement,
              nullptr) << "Expected non-null return statement pointer";

    if (p_return_statement)
      EXPECT_EQ(p_return_statement->return_value()->Evaluate(), 1);
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

  auto p_func_declaration = dynamic_cast<nast::FunctionDeclaration const*>
                            (instructions[0].get());

  ASSERT_NE(p_func_declaration, nullptr);

  if (p_func_declaration) {
    ASSERT_EQ(p_func_declaration->instructions().size(), 2);

    EXPECT_EQ(p_func_declaration->instructions()[0]->TypeInstruction(),
              "Unary");

    const auto& unary = p_func_declaration->instructions()[0];
    auto p_unary = dynamic_cast<nast::Unary const*>(unary.get());
    EXPECT_EQ(p_unary->unary_operation(), UnaryOperation::kNegate);
    EXPECT_EQ(p_unary->dst()->ExpressionType(), "Variable");
    EXPECT_EQ(p_unary->src()->ExpressionType(), "Constant");

    EXPECT_EQ(p_func_declaration->instructions()[1]->TypeInstruction(),
              "Return");
    auto p_return_statement = dynamic_cast<nast::Return const*>
                              (p_func_declaration->instructions()[1].get());

    EXPECT_NE(p_return_statement,
              nullptr) << "Expected non-null return statement pointer";

    if (p_return_statement) {
      EXPECT_NE(p_return_statement->return_value(), nullptr);
      EXPECT_EQ(p_return_statement->return_value()->ExpressionType(), "Variable");

      const auto& var = p_return_statement->return_value();
      auto p_var = dynamic_cast<nast::Variable const*> (var.get());
      EXPECT_EQ(p_var->name(), "tempo.0");
    }
  }
}

TEST(AstParserTest, MultipleUnaryOperators) {
  std::string input = R"(int test(){
    return ~(-2);
    })";
  Lexer lexer(input);
  std::vector<Token> tokens = lexer.Tokens();

  Parser parser(tokens);

  std::unique_ptr<nast::Program> res = eval(parser.ParseProgram());

  auto pDeclaration = dynamic_cast<nast::FunctionDeclaration const*>
                      (res->instructions()[0].get());

  if (pDeclaration) {
    ASSERT_EQ(pDeclaration->instructions().size(), 1);

    auto p_return_statement = dynamic_cast<nast::Return const*>
                              (pDeclaration->instructions()[0].get());

    if (p_return_statement) {
      auto p_unary_expression_complement = dynamic_cast<nast::Unary const*>
                                           (p_return_statement->return_value().get());

      EXPECT_NE(p_unary_expression_complement, nullptr);

      if (p_unary_expression_complement) {
        EXPECT_EQ(p_unary_expression_complement->unary_operation(),
                  UnaryOperation::kComplement);
        EXPECT_EQ(p_unary_expression_complement->dst()->name(), "tempo.1");

        auto p_unary_expression_negate = dynamic_cast<nast::Unary const*>
                                         ( p_unary_expression_complement->src().get() );

        EXPECT_NE(p_unary_expression_negate, nullptr);
        EXPECT_EQ(p_unary_expression_negate->unary_operation(),
                  UnaryOperation::kNegate);
        EXPECT_EQ(p_unary_expression_negate->dst()->name(), "tempo.0");


        EXPECT_EQ(p_return_statement->return_value()->Evaluate(), -2);
      }
    }
  }
}
