#include "ast.hh"
#include "token.hh"
#include <memory>
#include <stdexcept>
#include <string>

std::unique_ptr<ConstantValue> BinaryExpression::Evaluate() const {
  std::unique_ptr<ConstantValue> left = _left->Evaluate();
  std::unique_ptr<ConstantValue> right = _right->Evaluate();


  int* left_int = std::get_if<int>(left.get());
  int* right_int = std::get_if<int>(right.get());

  if (left_int && right_int) {
    switch (_operation) {
      case TokenType::kPlus:
        return std::make_unique<ConstantValue>(*left_int + *right_int);
        break;

      case TokenType::kStar:
        return std::make_unique<ConstantValue>(*left_int * *right_int);
        break;

      case TokenType::kSlash:
        return std::make_unique<ConstantValue>(*left_int / *right_int);
        break;

      case TokenType::kMinus:
        return std::make_unique<ConstantValue>(*left_int - *right_int);
        break;

      default:
        throw std::invalid_argument("expected binary operation, got " + StringTokenType(
                                        _operation) + " instead");
    }
  } else
    throw std::invalid_argument("left or/and right is/are of unexpected type");
}
std::unique_ptr<ConstantValue> PrefixExpression::Evaluate() const {
  std::unique_ptr<ConstantValue> eval = _expression_value->Evaluate();

  // ConstantValue eval_val = eval->value();
  // if evaluated prefix is an int, int_eval is an int *, else it's a nullptr
  int* int_eval = std::get_if<int>(eval.get());


  if (int_eval) {
    switch (_prefix_type) {
      case TokenType::kMinus:
        return std::make_unique<ConstantValue>(-(*int_eval));
        break;

      case TokenType::kTilde:
        return std::make_unique<ConstantValue>(~(*int_eval));
        break;

      default:
        throw std::runtime_error("Unknown prefix type: " +
                                 StringTokenType(_prefix_type));
    }
  }

  else
    return _expression_value->Evaluate();
}
