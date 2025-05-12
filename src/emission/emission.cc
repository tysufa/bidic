#include "emission.hh"
#include "nast.hh"
#include <iostream>
#include <memory>
#include <string>
#include <type_traits>

std::string Emitor::Emit() {
  std::string result;


  // make my own list of instructions
  auto instr = _program->instructions();

  for (int i = 0; i < instr.size(); i++)
    _instructions.push_back(std::move(instr[i]));

  std::string indent = "";
  const nast::Return* p_return;
  nast::FunctionDeclaration* p_function;
  const nast::Unary* p_unary;

  while (_current_instruction < _instructions.size()) {
    p_return = dynamic_cast<nast::Return const*>
               (_instructions[_current_instruction].get());

    p_function = dynamic_cast<nast::FunctionDeclaration*>(
                     _instructions[_current_instruction].get());

    p_unary = dynamic_cast<nast::Unary const*>(
                  _instructions[_current_instruction].get());

    if (p_unary) {
      auto src_variable = dynamic_cast<nast::Variable const*>(p_unary->src().get());

      if (src_variable)
        result += indent + "mov " + p_unary->dst()->name() + ", " + src_variable->name()
                  + "\n";
      else // if it's not a variable, it is a Constant, we don't check to go fast
        result += indent + "mov " + p_unary->dst()->name() + ", " +
                  std::to_string(p_unary->src()->Evaluate()) + "\n";

      result += indent + (p_unary->unary_operation() == UnaryOperation::kNegate ?
                          "neg " : "not ") + p_unary->dst()->name() + "\n";

    } else if (p_return) {
      auto constant_ret_val = dynamic_cast<nast::Constant const*>
                              (p_return->return_value().get());

      auto variable_ret_val = dynamic_cast<nast::Variable const*>
                              (p_return->return_value().get());

      if (constant_ret_val)
        result += indent + "mov eax, " + std::to_string(constant_ret_val->value()) +
                  "\n";
      else if (variable_ret_val)
        result += indent + "mov eax, " + variable_ret_val->name() + "\n";



      result += indent + "ret\n";

    } else if (p_function) {

      result += indent + "." + p_function->identifier()->name() + "\n";
      indent += "  ";

      // add new instructions from function to the list
      auto function_instr = p_function->instructions();

      for (int i = 0; i < function_instr.size(); i++)
        _instructions.push_back(std::move(function_instr[i]));

    }

    _current_instruction++;

  }

  return result;
}
