#pragma once
#include "../nast/nast.hh"
#include <memory>
#include <string>
#include <vector>

class Emitor {
 public:
  Emitor(std::unique_ptr<nast::Program> program)
    : _program(std::move(program)), _current_instruction(0) {}
  std::string Emit();

 private:
  int _current_instruction;
  std::vector<std::unique_ptr<nast::Instruction>> _instructions;

  std::unique_ptr<nast::Program> _program;
};
