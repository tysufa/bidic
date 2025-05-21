#include "emission_asm.hh"
#include "scug.hh"
#include <iostream>
#include <memory>
#include <string>
#include <algorithm>

// std::string Emitor_asm::ReplacePseudoRegister(const std::string& pseudo_reg) {
//   auto it = _registers.find(pseudo_reg);

//   if (it != _registers.end())
//     return it->second;
//   else {
//     _current_stack_placement += 4;
//     _registers[pseudo_reg] = "[ebp-" + std::to_string(_current_stack_placement) +
//                              "]";
//     return _registers[pseudo_reg];
//   }
// }

// std::string Emitor_asm::ExpressionToStr(const std::shared_ptr<scug::Expression>&
//                                     expr) {
//   if (const auto& var = dynamic_cast<scug::Variable const*>(expr.get()))
//     return ReplacePseudoRegister(var->name());
//   else
//     return std::to_string(expr->Evaluate());
// }

std::string Emitor_asm::Emit() {


  // make my own list of instructions
  auto instr = _program->instructions();


//   std::string indent = "";
//   const scug::Return* p_return;
//   scug::FunctionDeclaration* p_function;
//   const scug::Unary* p_unary;
//   const scug::Binary* p_binary;
    std::cout<<"before first emit "<<_index<<" "<<instr.size()<<std::endl;

    std::string result=Declaration(instr);

  while (_index < instr.size()) {
    std::cout<<"first emit"<<std::endl;
    result+=EmitInstruction(_index,instr);
    ++_index;
  }

  return result;
}

std::string Emitor_asm::EmitInstruction(int &index, std::vector<std::shared_ptr<scug::Instruction>> instr) {
    std::string res(_indent);
    std::string type=instr[index]->TypeInstruction();
    if(type=="FunctionDeclaration"){
        auto cast = std::dynamic_pointer_cast<scug::FunctionDeclaration>(instr[index]);
        res+="int "+cast->identifier()->name()+"(){\n";
        res+=EmitDeclaration(cast);
        res+="}\n";
    }
    else if(type=="MoveInstruction"){
        auto cast = std::dynamic_pointer_cast<scug::Move>(instr[index]);
        res+=cast->get_register()+" = "+cast->value()->Evaluate()+";\n";
    }
    else if(type=="Return"){
        auto cast = std::dynamic_pointer_cast<scug::Return>(instr[index]);
        res+="return "+cast->return_value()->Evaluate()+";\n";
    }
    return res;
}

std::string Emitor_asm::EmitDeclaration(std::shared_ptr<scug::FunctionDeclaration> func){   
    std::string ind=_indent;
    _indent= _indent+"  ";
    auto instr=func->instructions();
    std::string result=Declaration(instr);
    int index=0;
    while(index<instr.size()){
        result+=EmitInstruction(index,instr);
        ++index;
    }
    _indent=ind;
    return result;
}

std::vector<std::string> Emitor_asm::init(std::vector<std::shared_ptr<scug::Instruction>> instr){
    std::vector<std::string> v;
    auto i=instr.begin();
    while(i!=instr.end()){
        auto cast = std::dynamic_pointer_cast<scug::Move>(*i);
        if(cast && cast->get_register()!="eax" && std::find(v.begin(), v.end(), cast->get_register()) == v.end()){
            v.push_back(cast->get_register());
        }
        ++i;
    }
    return v;
}

std::string Emitor_asm::Declaration(std::vector<std::shared_ptr<scug::Instruction>> instr){
    std::string result="";
    std::vector<std::string> v(init(instr));
    if(v.size()!=0){
        result+=_indent+"int ";
        for(auto a : v){
            result+=a+",";
        }
        result.pop_back();
        result+=";\n";
    }
    return result;
}