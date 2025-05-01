#include "ast_parser.hh"
#include "ast.hh"
#include "nast.hh"
#include <memory>
#include <vector>

std::unique_ptr<nast::Program> eval(std::unique_ptr<Program> ast) {
  auto nast = std::make_unique<nast::Program>();

  const std::vector<std::unique_ptr<Instruction>>& instruction = ast->instructions();



  if (auto function = dynamic_cast<FunctionDeclaration const*>
                      (instruction[0].get())) {
    auto ident = std::make_unique<nast::Identifier>(function->identifier()->name());
    auto nasm_func = std::make_unique<nast::FunctionDeclaration>(std::move(ident));

    if (auto ret = dynamic_cast<ReturnStatement const*>
                   (function->instructions()[0].get())) {
      auto return_expresion = ret->return_value()->Evaluate();

      if (auto return_value = dynamic_cast<IntLiteral const*>
                              (return_expresion.get())) {

        auto move = std::make_unique<nast::Move>(Register::eax, return_value->value());

        nasm_func->add_instruction(std::make_unique<nast::Return>(std::move(move)));
        nast->add_instruction(std::move(nasm_func));
      }

    }

  }

  return nast;
}
