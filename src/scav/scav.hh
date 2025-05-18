#pragma once

#include <memory>
#include <string>
#include <vector>

// enum class Register { eax, ebx, ecx, edx, ebp, esp, esi, edi };


// inline std::string RegisterToString(Register r) {
//   static const char* names[] = {"eax", "ebx", "ecx", "edx",
//                                 "ebp", "esp", "esi", "edi"
//                                };
//   return names[static_cast<int>(r)]; // 'value' is the enum's underlying integer
// }

namespace scav {
using namespace scav;


class Expression {
 public:
  virtual int Evaluate() const = 0;
  //TODO: value() should be replaced by Evaluate(), it is here juste to test
  //that you can replace Constant by Expression
  virtual int value() const = 0;

  virtual std::string ExpressionType() const = 0;

};

class Variable : public scav::Expression {
 public:
  Variable(const std::string& name)
    : _name(name) {}

  const std::string& name() const {return _name;}

  int Evaluate() const override {return 0;}
  int value() const override {return 0;}

  std::string ExpressionType() const override {return "Variable";}

 private:
  std::string _name;
};

// TODO: For now Constant is only integers, later we need to make it a virtual
// class and make subclasses for other types
class Constant : public scav::Expression {
 public:
  Constant(int value)
    : _value(value) {}

  int value() const override {return _value;}
  int Evaluate() const override {return _value;}

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

  std::vector<std::unique_ptr<Instruction>> instructions() {
    return std::move(_instructions);
  }
  void add_instruction(std::unique_ptr<scav::Instruction> instruction) {
    _instructions.push_back(std::move(instruction));
  }

 private:
  std::vector<std::unique_ptr<Instruction>> _instructions;
};

class Identifier {
 public:
  Identifier(const std::string &name) : _name(name) {}

  const std::string& name() const { return _name; }

 private:
  std::string _name;
};

class FunctionDeclaration : public scav::Instruction {
 public:
  FunctionDeclaration(std::unique_ptr<Identifier> ident)
    : _identifier(std::move(ident)) {}

  std::string TypeInstruction() const override {
    return "FunctionDeclaration";
  };

  const std::vector<std::unique_ptr<scav::Instruction>> &instructions() const {
    return _instructions;
  }

  const std::unique_ptr<Identifier>& identifier() const { return _identifier; }

  void add_instruction(std::unique_ptr<Instruction> instruction) {
    _instructions.push_back(std::move(instruction));
  }

 private:
  std::vector<std::unique_ptr<Instruction>> _instructions;
  std::unique_ptr<Identifier> _identifier;
};

class Move : public scav::Instruction {
 public:
  Move(std::string reg, std::string value)
    : _register(reg), _value(value) {}

  std::string TypeInstruction() const override { return "MoveInstruction"; };

  std::string get_register() const { return _register; };
  // std::string get_register_str() { return RegisterToString(_register); };
  std::string value() const { return _value; };

 private:
  std::string _register;
  std::string _value;
};

class Return : public scav::Instruction {
  public:
    Return(){}

    std::string TypeInstruction() const override { return "Return";}
};

/*
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
*/

} // namespace scav
