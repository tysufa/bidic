#pragma once
#include "../scug/scug.hh"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Emitor {
 public:
  Emitor(std::unique_ptr<scug::Program> program)
    : _program(std::move(program)), _current_instruction(0),
      _current_stack_placement(0) {}
  std::string Emit();

 private:
  std::string ReplacePseudoRegister(const std::string& pseudo_reg);
  std::string ExpressionToStr(const std::shared_ptr<scug::Expression>& expr);

  int _current_stack_placement;

  int _current_instruction;
  std::vector<std::shared_ptr<scug::Instruction>> _instructions;

  std::shared_ptr<scug::Program> _program;
  std::unordered_map<std::string, std::string> _registers;
};
