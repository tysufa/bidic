#include "ast_parser.hh"
#include "ast.hh"
#include "scug.hh"
#include "token.hh"
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

std::string MakeTemporaryVariableName() {
  static unsigned int var_number = 0;
  return "tempo." + std::to_string(var_number++);
}

std::unique_ptr<scug::Expression> ParseAstExpression(
    scug::FunctionDeclaration* scug,
    const std::unique_ptr<ast::Expression>& expr) {

  auto constant = dynamic_cast<ast::Constant const*>(expr.get());

  if (constant) {
    // we get the value of the constant
    std::unique_ptr<ast::ConstantValue> constant_val = constant->Evaluate();

    // we check the different possible value types (int, str...)
    if (int* int_val = std::get_if<int>(constant_val.get())) {
      return std::make_unique<scug::Constant>(*int_val);  // Use the extracted int
    } else
      throw std::runtime_error("Constant is of unexpected type");
  }

  auto unary = dynamic_cast<ast::PrefixExpression const*>(expr.get());

  if (unary) {
    auto src = ParseAstExpression(std::move(scug), unary->expression_value());

    std::string variable_name = MakeTemporaryVariableName();
    auto dst = std::make_unique<scug::Variable>(variable_name);

    UnaryOperation op = (unary->prefix_type() == TokenType::kMinus ?
                         UnaryOperation::kNegate : UnaryOperation::kComplement);

    scug->add_instruction(std::make_unique<scug::Unary>(op, std::move(src),
                          std::move(dst)));

    // we can't use dst again since we moved it into our Unary operator
    return std::make_unique<scug::Variable>(variable_name);
  }

  auto binary = dynamic_cast<ast::BinaryExpression const*>(expr.get());

  if (binary) {
    auto src1 = ParseAstExpression(std::move(scug), binary->left());
    auto src2 = ParseAstExpression(std::move(scug), binary->right());

    std::string variable_name = MakeTemporaryVariableName();
    auto dst = std::make_unique<scug::Variable>(variable_name);

    BinaryOperation op;

    // TODO: create a separate function to handle this logic
    switch (binary->operation()) {
      case TokenType::kPlus:
        op = BinaryOperation::kPlus;
        break;

      case TokenType::kMinus:
        op = BinaryOperation::kMinus;
        break;

      case TokenType::kStar:
        op = BinaryOperation::kMultply;
        break;

      case TokenType::kSlash:
        op = BinaryOperation::kDivide;
        break;

      default:
        std::runtime_error("unexpected binary operation : " + StringTokenType(
                               binary->operation()));
        break;
    }

    scug->add_instruction(std::make_unique<scug::Binary>(op, std::move(src1),
                          std::move(src2), std::move(dst)));

    return std::make_unique<scug::Variable>(variable_name);
  }

  else
    throw std::runtime_error("expected Constant or Prefix");
}

std::unique_ptr<scug::Program> eval(std::unique_ptr<ast::Program> ast) {
  auto scug = std::make_unique<scug::Program>();

  const std::vector<std::unique_ptr<ast::Instruction>>& instruction = ast->instructions();


  if (auto function = dynamic_cast<ast::FunctionDeclaration const*>
                      (instruction[0].get())) {

    // creating nast Function
    auto ident = std::make_unique<scug::Identifier>(function->identifier()->name());
    auto nasm_func = std::make_unique<scug::FunctionDeclaration>(std::move(ident));

    if (auto ret = dynamic_cast<ast::ReturnStatement const*>
                   (function->instructions()[0].get())) {

      const std::unique_ptr<ast::Expression>& return_expression = ret->return_value();

      // ParseAstExpression() should return a Constant or a Variable
      auto nast_return_expr = ParseAstExpression(nasm_func.get(), return_expression);

      nasm_func->add_instruction(std::make_unique<scug::Return>(std::move(
                                     nast_return_expr)));
      scug->add_instruction(std::move(nasm_func));
    }
  }

  return scug;
}
