#include "ast_parser.hh"
#include "ast.hh"
#include "nast.hh"
#include "token.hh"
#include <algorithm>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

std::string MakeTemporaryVariableName() {
  static unsigned int var_number = 0;
  return "tempo." + std::to_string(var_number++);
}

std::unique_ptr<nast::Expression> ParseAstExpression(
    nast::FunctionDeclaration* nast,
    const std::unique_ptr<Expression>& expr) {

  auto constant = dynamic_cast<Constant const*>(expr.get());

  if (constant) {
    // we get the value of the constant
    std::unique_ptr<ConstantValue> constant_val = constant->Evaluate();

    // we check the different possible value types (int, str...)
    if (int* int_val = std::get_if<int>(constant_val.get())) {
      return std::make_unique<nast::Constant>(*int_val);  // Use the extracted int
    } else
      throw std::runtime_error("LiteralValue is of unexpected type");
  }

  auto unary = dynamic_cast<PrefixExpression const*>(expr.get());

  if (unary) {
    auto src = ParseAstExpression(std::move(nast), unary->expression_value());

    std::string variable_name = MakeTemporaryVariableName();
    auto dst = std::make_unique<nast::Variable>(variable_name);

    UnaryOperation op = (unary->prefix_type() == TokenType::kMinus ?
                         UnaryOperation::kNegate : UnaryOperation::kComplement);

    nast->add_instruction(std::make_unique<nast::Unary>(op, std::move(src),
                          std::move(dst)));

    // we can't use dst again since we moved it into our Unary operator
    return std::make_unique<nast::Variable>(variable_name);
  }

  else
    throw std::runtime_error("expected LiteralExpression or PrefixExpression");
}

std::unique_ptr<nast::Program> eval(std::unique_ptr<Program> ast) {
  auto nast = std::make_unique<nast::Program>();

  const std::vector<std::unique_ptr<Instruction>>& instruction = ast->instructions();


  if (auto function = dynamic_cast<FunctionDeclaration const*>
                      (instruction[0].get())) {

    // creating nast Function
    auto ident = std::make_unique<nast::Identifier>(function->identifier()->name());
    auto nasm_func = std::make_unique<nast::FunctionDeclaration>(std::move(ident));

    if (auto ret = dynamic_cast<ReturnStatement const*>
                   (function->instructions()[0].get())) {

      const std::unique_ptr<Expression>& return_expression = ret->return_value();

      // ParseAstExpression() should return a Constant or a Variable
      auto nast_return_expr = ParseAstExpression(nasm_func.get(), return_expression);

      nasm_func->add_instruction(std::make_unique<nast::Return>(std::move(
                                     nast_return_expr)));
      nast->add_instruction(std::move(nasm_func));
    }
  }

  return nast;
}
