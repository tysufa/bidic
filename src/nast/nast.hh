#pragma once

#include <memory>
#include <string>
#include <vector>

enum class Register { eax, ebx, ecx, edx, ebp, esp, esi, edi };

enum class UnaryOperation {kNegate, kComplement};

inline std::string RegisterToString(Register r) {
  static const char* names[] = {"eax", "ebx", "ecx", "edx",
                                "ebp", "esp", "esi", "edi"
                               };
  return names[static_cast<int>(r)]; // 'value' is the enum's underlying integer
}

namespace nast {
using namespace nast;

class Variable {
 public:
  Variable(const std::string& name)
    : _name(name) {}

 private:
  std::string _name;
};

class Expression {
 public:

};

// TODO: For now Constant is only integers, later we need to make it a virtual
// class and make subclasses for other types
class Constant {
 public:
  Constant(int value)
    : _value(value) {}

  int value() const {return _value;}


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

  const std::vector<std::unique_ptr<Instruction>> &instructions() const {
    return _instructions;
  }
  void add_instruction(std::unique_ptr<nast::Instruction> instruction) {
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

class FunctionDeclaration : public nast::Instruction {
 public:
  FunctionDeclaration(std::unique_ptr<Identifier> ident)
    : _identifier(std::move(ident)) {}

  std::string TypeInstruction() const override {
    return "FunctionDeclaration";
  };

  const std::vector<std::unique_ptr<nast::Instruction>> &instructions() const {
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

class Move : public Instruction {
 public:
  Move(Register reg, std::unique_ptr<Constant> value)
    : _register(reg), _value(std::move(value)) {}

  std::string TypeInstruction() const override { return "MoveInstruction"; };

  Register get_register() const { return _register; };
  std::string get_register_str() { return RegisterToString(_register); };
  const std::unique_ptr<Constant>& value() const { return _value; };

 private:
  Register _register;
  std::unique_ptr<Constant> _value;
};

class Return : public Instruction {
 public:
  Return(std::unique_ptr<Move> mov) : _move(std::move(mov)) {}

  std::string TypeInstruction() const override { return "ReturnInstruction"; }

  const std::unique_ptr<Move>& move() const { return _move; }

 private:
  std::unique_ptr<Move> _move;
};

class Unary : public Expression{
 public:
  Unary(UnaryOperation unary_op, std::unique_ptr<Constant> src, std::unique_ptr<Variable> dst)
    : _unary_operation(unary_op), _src(std::move(src)), _dst(std::move(dst)) {}

 private:
  std::unique_ptr<Constant> _src;
  std::unique_ptr<Variable> _dst;
  UnaryOperation _unary_operation;
};

} // namespace nast
