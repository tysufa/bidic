#include "emission.hh"
#include "scug.hh"
#include <memory>
#include <string>

std::string Emitor::ReplacePseudoRegister(const std::string& pseudo_reg) {
  auto it = _registers.find(pseudo_reg);

  if (it != _registers.end())
    return it->second;
  else {
    _current_stack_placement += 4;
    _registers[pseudo_reg] = "[ebp-" + std::to_string(_current_stack_placement) +
                             "]";
    return _registers[pseudo_reg];
  }
}

std::string Emitor::ExpressionToStr(const std::shared_ptr<scug::Expression>&
                                    expr) {
  if (const auto& var = dynamic_cast<scug::Variable const*>(expr.get()))
    return ReplacePseudoRegister(var->name());
  else
    return /*std::to_string*/(expr->Evaluate());
}

std::string Emitor::Emit() {
  std::string result;


  // make my own list of instructions
  auto instr = _program->instructions();

  for (int i = 0; i < instr.size(); i++)
    _instructions.push_back(std::move(instr[i]));

  std::string indent = "";
  const scug::Return* p_return;
  scug::FunctionDeclaration* p_function;
  const scug::Unary* p_unary;
  const scug::Binary* p_binary;

  while (_current_instruction < _instructions.size()) {
    p_return = dynamic_cast<scug::Return const*>
               (_instructions[_current_instruction].get());

    p_function = dynamic_cast<scug::FunctionDeclaration*>(
                     _instructions[_current_instruction].get());

    p_unary = dynamic_cast<scug::Unary const*>(
                  _instructions[_current_instruction].get());

    p_binary = dynamic_cast<scug::Binary const*>(
                   _instructions[_current_instruction].get());

    if (p_binary) {
      auto src_variable_1 = dynamic_cast<scug::Variable const*>
                            (p_binary->src1().get());
      auto src_variable_2 = dynamic_cast<scug::Variable const*>
                            (p_binary->src2().get());

      if (p_binary->binary_operation() != BinaryOperation::kDivide) {

        switch (p_binary->binary_operation()) {
          case BinaryOperation::kPlus:
            if (src_variable_2) {
              result += indent + "mov " + ReplacePseudoRegister(p_binary->dst()->name()) +
                        ", "  + ExpressionToStr(p_binary->src2()) + "\n";
              result += indent + "add " + ReplacePseudoRegister(p_binary->dst()->name()) +
                        ", " + ExpressionToStr(p_binary->src1()) + "\n";
            } else if (src_variable_1) {
              result += indent + "mov " + ReplacePseudoRegister(p_binary->dst()->name()) +
                        ", "  + ExpressionToStr(p_binary->src1()) + "\n";
              result += indent + "add " + ReplacePseudoRegister(p_binary->dst()->name()) +
                        ", " + ExpressionToStr(p_binary->src2()) + "\n";
            } else {
              result += indent + "mov " + ReplacePseudoRegister(p_binary->dst()->name()) +
                        ", " + ExpressionToStr(p_binary->src1()) + "\n";
              result += indent + "add " + ReplacePseudoRegister(p_binary->dst()->name()) +
                        ", " + ExpressionToStr(p_binary->src2()) + "\n";
            }

            break;

          case BinaryOperation::kMultply:
            result += indent + "mov " + ReplacePseudoRegister(p_binary->dst()->name()) +
                      ", " + ExpressionToStr(p_binary->src1()) + "\n";
            result += indent + "imul " + ReplacePseudoRegister(p_binary->dst()->name()) +
                      ", " + ExpressionToStr(p_binary->src2()) + "\n";
            break;
        }

      }

    } else if (p_unary) {
      auto src_variable = dynamic_cast<scug::Variable const*>(p_unary->src().get());


      if (src_variable)
        result += indent + "mov " + ReplacePseudoRegister(p_unary->dst()->name()) + ", "
                  + ReplacePseudoRegister(src_variable->name()) + "\n";
      else // if it's not a variable, it is a Constant, we don't check to go fast
        result += indent + "mov " + ReplacePseudoRegister(p_unary->dst()->name()) + ", "
                  + /*std::to_string*/(p_unary->src()->Evaluate()) + "\n";

      result += indent + (p_unary->unary_operation() == UnaryOperation::kNegate ?
                          "neg " : "not ") + ReplacePseudoRegister(p_unary->dst()->name()) + "\n";

    } else if (p_return) {
      auto constant_ret_val = dynamic_cast<scug::Constant const*>
                              (p_return->return_value().get());

      auto variable_ret_val = dynamic_cast<scug::Variable const*>
                              (p_return->return_value().get());

      if (constant_ret_val)
        result += indent + "mov eax, " + std::to_string(constant_ret_val->value()) +
                  "\n";
      else if (variable_ret_val)
        result += indent + "mov eax, " + ReplacePseudoRegister(variable_ret_val->name())
                  + "\n";



      result += indent + "ret\n";

    } else if (p_function) {

      result += indent + p_function->identifier()->name() + ":\n";
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
