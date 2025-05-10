#include "ast.hh"
#include "token.hh"

std::unique_ptr<Literal> PrefixExpression::Evaluate() const {
  std::unique_ptr<Literal> eval = _expression_value->Evaluate();

  LiteralValue eval_val = eval->value();
  // if evaluated prefix is an int, int_eval is an int *, else it's a nullptr
  int* int_eval = std::get_if<int>(&eval_val);

  // auto int_eval = static_cast<const IntLiteral*>(eval.get());


  if (int_eval) {
    switch (_prefix_type) {
      case TokenType::kMinus:
        return std::make_unique<IntLiteral>(-(*int_eval));
        break;

      case TokenType::kTilde:
        return std::make_unique<IntLiteral>(~(*int_eval));
        break;

      default:
        throw std::runtime_error("Unknown prefix type: " + StringTokenType(
                                     _prefix_type));
    }
  }

  else
    return _expression_value->Evaluate();
}
