#include "ast_parser_asm.hh"
#include "scav.hh"
#include "scug.hh"
#include "token.hh"
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

std::unique_ptr<scug::Program> Ast_Parser_Asm::ParseProgram() {
    auto program = std::make_unique<scug::Program>();

    int _index=0;
    std::shared_ptr<scug::Instruction> instr;
    while(_index<_scav->instructions().size()){
        instr = ParseInstruction(_index,_scav->instructions());
        program->add_instruction(instr);
        ++_index;
    }

    return program;
}


std::shared_ptr<scug::Instruction> Ast_Parser_Asm::ParseInstruction(int &_index,const std::vector<std::shared_ptr<scav::Instruction>> &inst){
    while(inst[_index]->isoperation()==true && std::dynamic_pointer_cast<scav::Operation>(inst[_index])->get_dst()=="eax"){
        std::dynamic_pointer_cast<scav::Operation>(inst[_index])->update(_eax);
        ++_index;
    }
    if(inst[_index]->isoperation()==true){
        auto operation=std::dynamic_pointer_cast<scav::Operation>(inst[_index]);
        auto dst=std::make_shared<scug::Variable>(operation->get_dst());
        if(inst[_index]->TypeInstruction()=="MoveInstruction"){
            if(operation->value()=="eax"){
                return std::make_shared<scug::Move>(operation->get_dst(),_eax);
            }
            else{
                return std::make_shared<scug::Move>(operation->get_dst(),operation->Evaluate());
            }
        }
        else if(inst[_index]->TypeInstruction()=="ImulInstruction"){
            if(operation->value()=="eax"){
                return std::make_shared<scug::Move>(operation->get_dst(),std::make_shared<scug::BinaryExpression>(BinaryOperation::kMultply,dst,_eax));
            }
            else{
                return std::make_shared<scug::Move>(operation->get_dst(),std::make_shared<scug::BinaryExpression>(BinaryOperation::kMultply,dst,operation->Evaluate()));
            }        
        }
        else if(inst[_index]->TypeInstruction()=="AddInstruction"){
            if(operation->value()=="eax"){
                return std::make_shared<scug::Move>(operation->get_dst(),std::make_shared<scug::BinaryExpression>(BinaryOperation::kPlus,dst,_eax));
            }
            else{
                return std::make_shared<scug::Move>(operation->get_dst(),std::make_shared<scug::BinaryExpression>(BinaryOperation::kPlus,dst,operation->Evaluate()));
            }        
        }
        else if(inst[_index]->TypeInstruction()=="NegInstruction"){
            if(operation->value()=="eax"){
                return std::make_shared<scug::Move>(operation->get_dst(),std::make_shared<scug::UnaryExpression>(UnaryOperation::kNegate,_eax));
            }
            else{
                return std::make_shared<scug::Move>(operation->get_dst(),std::make_shared<scug::UnaryExpression>(UnaryOperation::kNegate,operation->Evaluate()));
            }        
        }
        else if(inst[_index]->TypeInstruction()=="NotInstruction"){
            if(operation->value()=="eax"){
                return std::make_shared<scug::Move>(operation->get_dst(),std::make_shared<scug::UnaryExpression>(UnaryOperation::kComplement,_eax));
            }
            else{
                return std::make_shared<scug::Move>(operation->get_dst(),std::make_shared<scug::UnaryExpression>(UnaryOperation::kComplement,operation->Evaluate()));
            }        
        }
        else throw std::runtime_error("no other operation than move at the moment : "+inst[_index]->TypeInstruction());
    }
    else if(inst[_index]->TypeInstruction()=="FunctionDeclaration"){
        auto func = std::dynamic_pointer_cast<scav::FunctionDeclaration>(inst[_index]);
        return ParseDeclaration(func);
    } 
    else if(inst[_index]->TypeInstruction()=="Return"){
        return std::make_shared<scug::Return>(_eax);
    }
    else throw std::runtime_error("invalid instruction");
}

std::unique_ptr<scug::FunctionDeclaration> Ast_Parser_Asm::ParseDeclaration(std::shared_ptr<scav::FunctionDeclaration> func){
    auto nasm_func = std::make_unique<scug::FunctionDeclaration>(std::make_unique<scug::Identifier>(func->identifier()->name()));

    int _index=0;
    std::shared_ptr<scug::Instruction> instr;
    while (_index<func->instructions().size()) {
        instr = ParseInstruction(_index,func->instructions());
        nasm_func->add_instruction(instr);
        ++_index;
    }

    return nasm_func;
}