#pragma once

#include <memory>
#include <string>
#include <vector>

// enum class Register { eax, ebx, ecx, edx, ebp, esp, esi, edi };

enum class UnaryOperation {kNegate, kComplement};
enum class BinaryOperation {kMinus, kPlus, kDivide, kMultply, kModulo};

inline std::string binopto_string(BinaryOperation b){
  switch (b)
  {
  case BinaryOperation::kMinus:
    return "-";
    break;
  case BinaryOperation::kPlus:
    return "+";
    break;
  case BinaryOperation::kDivide:
    return "/";
    break;
  case BinaryOperation::kMultply:
    return "*";
    break;
  case BinaryOperation::kModulo:
    return "%";
    break;
  default:
    throw std::runtime_error("unknow binary operation");
    break;
  }
}

inline std::string unopto_string(UnaryOperation u){
  switch(u){
    case UnaryOperation::kNegate:
      return "-";
      break;
    case UnaryOperation::kComplement:
      return "~";
      break;
    default:
      throw std::runtime_error("unknow unary operation");
      break;
  }
}

// inline std::string RegisterToString(Register r) {
//   static const char* names[] = {"eax", "ebx", "ecx", "edx",
//                                 "ebp", "esp", "esi", "edi"
//                                };
//   return names[static_cast<int>(r)]; // 'value' is the enum's underlying integer
// }

namespace scug {
using namespace scug;


class Expression {
 public:
  virtual std::string Evaluate() const = 0;
  //TODO: value() should be replaced by Evaluate(), it is here juste to test
  //that you can replace Constant by Expression
  virtual int value() const = 0;

  virtual std::string ExpressionType() const = 0;

};

class Variable : public Expression {
 public:
  Variable(const std::string& name)
    : _name(name) {}

  const std::string& name() const {return _name;}

  std::string Evaluate() const override {return _name;}
  int value() const override {return 0;}

  std::string ExpressionType() const override {return "Variable";}

 private:
  std::string _name;
};

// TODO: For now Constant is only integers, later we need to make it a virtual
// class and make subclasses for other types
class Constant : public Expression{
 public:
  Constant(int value)
    : _value(value) {}

  int value() const override {return _value;}
  std::string Evaluate() const override {return std::to_string(_value);}

  // std::string TypeInstruction() const override {return "Constant";}
  std::string ExpressionType() const override {return "Constant";}

 private:
  int _value;
};

class Instruction {
 public:
  virtual std::string TypeInstruction() const = 0; 
 private:
};

class Program {
 public:
  Program() = default;

  std::vector<std::shared_ptr<Instruction>> instructions() {
    return std::move(_instructions);
  }
  void add_instruction(std::shared_ptr<scug::Instruction> instruction) {
    _instructions.push_back(std::move(instruction));
  }

 private:
  std::vector<std::shared_ptr<Instruction>> _instructions;
};

class Identifier {
 public:
  Identifier(const std::string &name) : _name(name) {}

  const std::string& name() const { return _name; }

 private:
  std::string _name;
};

class FunctionDeclaration : public scug::Instruction {
 public:
  FunctionDeclaration(std::unique_ptr<Identifier> ident)
    : _identifier(std::move(ident)) {}

  std::string TypeInstruction() const override {
    return "FunctionDeclaration";
  };

  std::vector<std::shared_ptr<scug::Instruction>> instructions() {
    return std::move(_instructions);
  }

  const std::unique_ptr<Identifier>& identifier() const { return _identifier; }

  void add_instruction(std::shared_ptr<Instruction> instruction) {
    _instructions.push_back(std::move(instruction));
  }

 private:
  std::vector<std::shared_ptr<Instruction>> _instructions;
  std::unique_ptr<Identifier> _identifier;
};

class Move : public Instruction {
 public:
  Move(std::string reg, std::shared_ptr<Expression> value)
    : _register(reg), _value(value) {}

  std::string TypeInstruction() const override { return "MoveInstruction"; };

  std::string get_register() const { return _register; };
  // std::string get_register_str() { return RegisterToString(_register); };
  std::shared_ptr<Expression> value() const { return _value; };

 private:
  std::string _register;
  std::shared_ptr<Expression> _value;
};

class Return : public Instruction {
 public:
  Return(std::shared_ptr<Expression> return_value)
    : _return_value(std::move(return_value)) {}

  std::string TypeInstruction() const override { return "Return"; }

  const std::shared_ptr<Expression>& return_value() const {return _return_value;}

  // std::unique_ptr<Move> move() const {
  //   return std::make_unique<Move>(
  //       Register::eax, std::make_unique<Constant>(_return_value->value())
  //   );
  // }

 private:
  std::shared_ptr<Expression> _return_value;
};

class Binary : public Instruction{
 public:
  Binary(BinaryOperation binary_op, std::shared_ptr<Expression> src1, std::shared_ptr<Expression> src2, std::shared_ptr<Variable> dst)
    : _binary_operation(binary_op), _src1(std::move(src1)), _src2(std::move(src2)), _dst(std::move(dst)) {}

  BinaryOperation binary_operation() const {return _binary_operation;}
  const std::shared_ptr<Variable>& dst() const {return _dst;}
  const std::shared_ptr<Expression>& src1() const {return _src1;}
  const std::shared_ptr<Expression>& src2() const {return _src2;}

  std::string TypeInstruction() const override {return "Binary";};

 private:
  std::shared_ptr<Expression> _src1;
  std::shared_ptr<Expression> _src2;
  std::shared_ptr<Variable> _dst;
  BinaryOperation _binary_operation;
};

class BinaryExpression : public Expression{
 public:
  BinaryExpression(BinaryOperation binary_op, std::shared_ptr<Expression> src1, std::shared_ptr<Expression> src2)
    : _binary_operation(binary_op), _src1(std::move(src1)), _src2(std::move(src2)) {}

  BinaryOperation binary_operation() const {return _binary_operation;}
  const std::shared_ptr<Expression>& src1() const {return _src1;}
  const std::shared_ptr<Expression>& src2() const {return _src2;}

  std::string Evaluate() const override{return "("+_src1->Evaluate()+binopto_string(_binary_operation)+_src2->Evaluate()+")";}
  int value() const override{return 0;}
  std::string ExpressionType() const override {return "Binary";};

 private:
  std::shared_ptr<Expression> _src1;
  std::shared_ptr<Expression> _src2;
  BinaryOperation _binary_operation;
};

class Unary : public Instruction{
 public:
  Unary(UnaryOperation unary_op, std::unique_ptr<Expression> src, std::unique_ptr<Variable> dst)
    : _unary_operation(unary_op), _src(std::move(src)), _dst(std::move(dst)) {}

  UnaryOperation unary_operation() const {return _unary_operation;}
  const std::unique_ptr<Variable>& dst() const {return _dst;}
  const std::unique_ptr<Expression>& src() const {return _src;}

  std::string TypeInstruction() const override {return "Unary";};

 private:
  std::unique_ptr<Expression> _src;
  std::unique_ptr<Variable> _dst;
  UnaryOperation _unary_operation;
};

class UnaryExpression : public Expression{
 public:
  UnaryExpression(UnaryOperation unary_op, std::shared_ptr<Expression> src)
    : _unary_operation(unary_op), _src(std::move(src)) {}

  UnaryOperation unary_operation() const {return _unary_operation;}
  const std::shared_ptr<Expression>& src() const {return _src;}
  std::string Evaluate() const override{return unopto_string(_unary_operation)+"("+_src->Evaluate()+")";}
  int value() const override{return 0;}

  std::string ExpressionType() const override {return "Unary";};

 private:
  std::shared_ptr<Expression> _src;
  UnaryOperation _unary_operation;
};

} // namespace nast
