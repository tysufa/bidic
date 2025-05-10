#include "ast_parser.hh"
#include "ast.hh"
#include "nast.hh"
#include "token.hh"
#include <algorithm>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

std::unique_ptr<nast::Expression> ParseAstExpression(nast::Program* nast,
    const std::unique_ptr<Expression>& expr) {

  auto constant = dynamic_cast<IntExpression const*>(expr.get());




  if (constant) {
    LiteralValue constant_val = constant->Evaluate()->value();

    if (int* intVal = std::get_if<int>(&constant_val)) {
      return std::make_unique<nast::Constant>(*intVal);  // Use the extracted int
    }
  }

  auto unary = dynamic_cast<PrefixExpression const*>(expr.get());

  if (unary) {
    auto src = ParseAstExpression(std::move(nast), unary->expression_value());

    auto dst = std::make_unique<nast::Variable>("tempo.0");

    UnaryOperation op = (unary->prefix_type() == TokenType::kMinus ?
                         UnaryOperation::kNegate : UnaryOperation::kComplement);

    return std::make_unique<nast::Unary>(op, std::move(src),
                                         std::move(dst));
  }

  else
    throw std::runtime_error("expected LiteralExpression or PrefixExpression");
}

std::unique_ptr<nast::Program> eval(std::unique_ptr<Program> ast) {
  auto nast = std::make_unique<nast::Program>();

  const std::vector<std::unique_ptr<Instruction>>& instruction = ast->instructions();



  if (auto function = dynamic_cast<FunctionDeclaration const*>
                      (instruction[0].get())) {
    auto ident = std::make_unique<nast::Identifier>(function->identifier()->name());
    auto nasm_func = std::make_unique<nast::FunctionDeclaration>(std::move(ident));

    if (auto ret = dynamic_cast<ReturnStatement const*>
                   (function->instructions()[0].get())) {
      const auto& return_expression = ret->return_value();
      auto return_literal = return_expression->Evaluate();


      auto nast_return_expr = ParseAstExpression(nast.get(), return_expression);

      auto return_value = dynamic_cast<IntLiteral const*>
                          (return_literal.get());

      nasm_func->add_instruction(std::make_unique<nast::Return>(std::move(
                                     nast_return_expr)));
      nast->add_instruction(std::move(nasm_func));

      // if (return_value) {
      //
      //   auto move = std::make_unique<nast::Move>(Register::eax,
      //               std::make_unique<nast::Constant>(return_value->value()));
      //
      //   auto constant = std::make_unique<nast::Constant>(return_value->value());
      //
      //   nasm_func->add_instruction(std::make_unique<nast::Return>(std::move(constant)));
      //   nast->add_instruction(std::move(nasm_func));
      // }
    }
  }

  return nast;
}
