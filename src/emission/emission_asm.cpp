#include "emission_asm.hh"
#include "scug.hh"
#include <memory>
#include <string>
#include <algorithm>

std::string Emitor_asm::Emit() {
    auto instr = _program->instructions();

    std::string result=Declaration(instr);

    while (_index < instr.size()) {
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