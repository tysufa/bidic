#include "ast.hh"
#include "token.hh"

std::unique_ptr<Literal> PrefixExpression::Evaluate() const {
  std::unique_ptr<Literal> eval = _expression_value->Evaluate();
  auto int_eval = static_cast<const IntLiteral*>(eval.get());

  if (int_eval) {
    switch (_prefix_type) {
      case TokenType::kMinus:
        return std::make_unique<IntLiteral>(-int_eval->value());
        break;

      case TokenType::kTilde:
        return std::make_unique<IntLiteral>(~int_eval->value());
        break;

      default:
        throw std::runtime_error("Unknown prefix type: " + StringTokenType(
                                     _prefix_type));
    }
  }

  else
    return _expression_value->Evaluate();
}
