#pragma once

#include <string>
#include <memory>
#include <vector>

enum class Register { eax, ebx, ecx, edx, ebp, esp, };

namespace nast {
using namespace nast;

class Instruction {
 public:
  virtual std::string TypeInstruction() const = 0;

 private:

};

class Program {
 public:
  Program() = default;

  const std::vector<std::unique_ptr<Instruction>>& instructions() const {
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
  Identifier(const std::string& name)
    : _name(name) {}

  const std::string& name() const {
    return _name;
  }

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

  const std::vector<std::unique_ptr<Instruction>>& instructions() const {
    return _instructions;
  }

  const std::unique_ptr<Identifier>& identifier() const {
    return _identifier;
  }

  void add_instruction(std::unique_ptr<Instruction> instruction) {
    _instructions.push_back(std::move(instruction));
  }


 private:
  std::vector<std::unique_ptr<Instruction>> _instructions;
  std::unique_ptr<Identifier> _identifier;
};


class Move : public Instruction{
 public:
  // TODO: change int value type to a general type
  Move(Register reg, int value)
    : _register(reg), _value(value) {}

  std::string TypeInstruction() const override {return "MoveInstruction";};

  Register get_register() const {return _register;};
  int value() const {return _value;};


 private:
  Register _register;
  int _value;
};

class Return : public Instruction{
 public:
  Return(std::unique_ptr<Move> mov)
    : _move(std::move(mov)) {}

  std::string TypeInstruction() const override {return "ReturnInstruction";}

  const std::unique_ptr<Move>& move() const {return _move;}

 private:
  std::unique_ptr<Move> _move;
};

}
