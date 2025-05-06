#include "emission.hh"
#include "nast.hh"
#include <iostream>
#include <memory>
#include <string>

std::string Emitor::Emit(std::unique_ptr<nast::Program> program) {
  std::string result;

  const std::vector<std::unique_ptr<nast::Instruction>> &instruction =
      program->instructions();

  std::string indent = "";

  if (auto function = dynamic_cast<nast::FunctionDeclaration const *>(
          instruction[0].get())) {
    result += indent + "." + function->identifier()->name() + "\n";
    indent += "  ";

    const std::vector<std::unique_ptr<nast::Instruction>> &func_instructions =
        function->instructions();

    for (int i = 0; i < func_instructions.size(); i++) {
      std::cout << "instruction type : "
                << func_instructions[i]->TypeInstruction();
      if (auto func_return =
              dynamic_cast<nast::Return const *>(func_instructions[i].get())) {

        result += indent + "mov " + func_return->move()->StringRegister() +
                  ", " + std::to_string(func_return->move()->value()) + "\n";

        result += indent + "ret\n";
      }
    }
  }
  return result;
}
