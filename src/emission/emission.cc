#include "emission.hh"
#include <memory>

std::string Emitor::Emit(std::unique_ptr<nast::Program> program) {
  std::string result;

  const std::vector<std::unique_ptr<nast::Instruction >> & instruction = program->instructions();

  if (auto function = dynamic_cast<nast::FunctionDeclaration const*>
                      (instruction[0].get())) {
    result += "." + function->identifier()->name();
  }

  return result;
}
