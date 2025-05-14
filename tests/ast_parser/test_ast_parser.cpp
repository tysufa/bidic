#include "lexer.hh"
#include "token.hh"
#include "parser.hh"
#include "ast.hh"
#include "scug.hh"
#include "ast_parser.hh"
#include <iostream>
#include <memory>
#include <string>
#include <gtest/gtest.h>

#define CONVERT(var, type) dynamic_cast<type>(var.get())

TEST(AstParserTest, BasicProgramTest) {
  std::string input = R"(int test(){
    return 1;
    })";
  Lexer lexer(input);
  std::vector<Token> tokens = lexer.Tokens();

  Parser parser(tokens);

  std::unique_ptr<scug::Program> res;
  // the std::move is automatic
  res = eval(parser.ParseProgram());

  std::vector<std::unique_ptr<scug::Instruction>> instructions = std::move(res->instructions());

  ASSERT_EQ(instructions.size(), 1);

  EXPECT_EQ(instructions[0]->TypeInstruction(), "FunctionDeclaration");

  auto p_function_declaration = CONVERT(instructions[0], scug::FunctionDeclaration*);

  EXPECT_NE(p_function_declaration, nullptr) << "Expected non-null func pointer";

  if (p_function_declaration) {
    auto func_instr = p_function_declaration->instructions();

    EXPECT_EQ(p_function_declaration->identifier()->name(), "test");


    ASSERT_EQ(func_instr.size(), 1);

    EXPECT_EQ(func_instr[0]->TypeInstruction(),
              "Return");

    auto p_return_statement = dynamic_cast<scug::Return const*>
                              (func_instr[0].get());

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

  std::unique_ptr<scug::Program> res;

  res = eval(parser.ParseProgram());

  const std::vector<std::unique_ptr<scug::Instruction>>& instructions = res->instructions();

  auto p_func_declaration = dynamic_cast<scug::FunctionDeclaration*>
                            (instructions[0].get());

  ASSERT_NE(p_func_declaration, nullptr);

  if (p_func_declaration) {
    auto func_instr = p_func_declaration->instructions();
    ASSERT_EQ(func_instr.size(), 2);

    EXPECT_EQ(func_instr[0]->TypeInstruction(),
              "Unary");

    auto unary = std::move(func_instr[0]);
    auto p_unary = dynamic_cast<scug::Unary const*>(unary.get());
    EXPECT_EQ(p_unary->unary_operation(), UnaryOperation::kNegate);
    EXPECT_EQ(p_unary->dst()->ExpressionType(), "Variable");
    EXPECT_EQ(p_unary->src()->ExpressionType(), "Constant");

    EXPECT_EQ(func_instr[1]->TypeInstruction(),
              "Return");
    auto p_return_statement = dynamic_cast<scug::Return const*>
                              (func_instr[1].get());

    EXPECT_NE(p_return_statement,
              nullptr) << "Expected non-null return statement pointer";

    if (p_return_statement) {
      EXPECT_NE(p_return_statement->return_value(), nullptr);
      EXPECT_EQ(p_return_statement->return_value()->ExpressionType(), "Variable");

      const auto& var = p_return_statement->return_value();
      auto p_var = dynamic_cast<scug::Variable const*> (var.get());
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

  std::unique_ptr<scug::Program> res = eval(parser.ParseProgram());

  auto instr = res->instructions();

  auto p_function = dynamic_cast<scug::FunctionDeclaration*>
                    (instr[0].get());

  if (p_function) {
    auto func_instr = p_function->instructions();

    ASSERT_EQ(func_instr.size(), 3);

    auto unary1 = std::move(func_instr[0]);
    auto p_unary1 = dynamic_cast<scug::Unary const*>(unary1.get());

    EXPECT_EQ(p_unary1->unary_operation(), UnaryOperation::kNegate);
    EXPECT_EQ(p_unary1->dst()->ExpressionType(), "Variable");
    EXPECT_EQ(p_unary1->src()->ExpressionType(), "Constant");

    auto unary2 = std::move(func_instr[1]);
    auto p_unary2 = dynamic_cast<scug::Unary const*>(unary2.get());

    EXPECT_EQ(p_unary2->unary_operation(), UnaryOperation::kComplement);
    EXPECT_EQ(p_unary2->dst()->ExpressionType(), "Variable");
    EXPECT_EQ(p_unary2->src()->ExpressionType(), "Variable");

    EXPECT_EQ(func_instr[2]->TypeInstruction(),
              "Return");
    auto p_return_statement = CONVERT(std::move(func_instr[2]),
                                      scug::Return*);

    EXPECT_NE(p_return_statement,
              nullptr) << "Expected non-null return statement pointer";

    if (p_return_statement) {
      EXPECT_NE(p_return_statement->return_value(), nullptr);
      EXPECT_EQ(p_return_statement->return_value()->ExpressionType(), "Variable");

      const auto& var = p_return_statement->return_value();
      auto p_var = dynamic_cast<scug::Variable const*> (var.get());
      EXPECT_EQ(p_var->name(), "tempo.1");
    }
  }
}


TEST(AstParserTest, BinaryOperators) {
  std::string input = R"(int test(){
    return 1+2*3;
    })";
  Lexer lexer(input);
  std::vector<Token> tokens = lexer.Tokens();

  Parser parser(tokens);

  std::unique_ptr<scug::Program> res = eval(parser.ParseProgram());

  auto instr = res->instructions();

  auto p_function = dynamic_cast<scug::FunctionDeclaration*>
                    (instr[0].get());

  if (p_function) {
    auto func_instr = p_function->instructions();

    ASSERT_EQ(func_instr.size(), 3);

    // auto unary1 = std::move(func_instr[0]);
    // auto p_unary1 = dynamic_cast<nast::Unary const*>(unary1.get());
    //
    // EXPECT_EQ(p_unary1->unary_operation(), UnaryOperation::kNegate);
    // EXPECT_EQ(p_unary1->dst()->ExpressionType(), "Variable");
    // EXPECT_EQ(p_unary1->src()->ExpressionType(), "Constant");
    //
    // auto unary2 = std::move(func_instr[1]);
    // auto p_unary2 = dynamic_cast<nast::Unary const*>(unary2.get());
    //
    // EXPECT_EQ(p_unary2->unary_operation(), UnaryOperation::kComplement);
    // EXPECT_EQ(p_unary2->dst()->ExpressionType(), "Variable");
    // EXPECT_EQ(p_unary2->src()->ExpressionType(), "Variable");
    //
    // EXPECT_EQ(func_instr[2]->TypeInstruction(),
    //           "Return");
    // auto p_return_statement = CONVERT(std::move(func_instr[2]),
    //                                   nast::Return*);
    //
    // EXPECT_NE(p_return_statement,
    //           nullptr) << "Expected non-null return statement pointer";
    //
    // if (p_return_statement) {
    //   EXPECT_NE(p_return_statement->return_value(), nullptr);
    //   EXPECT_EQ(p_return_statement->return_value()->ExpressionType(), "Variable");
    //
    //   const auto& var = p_return_statement->return_value();
    //   auto p_var = dynamic_cast<nast::Variable const*> (var.get());
    //   EXPECT_EQ(p_var->name(), "tempo.1");
    // }
  }
}
