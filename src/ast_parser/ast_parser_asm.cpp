#include "ast_parser_asm.hh"
#include "scav.hh"
#include "scug.hh"
#include "token.hh"
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include <iostream>

std::unique_ptr<scug::Program> Ast_Parser_Asm::ParseProgram() {
    auto program = std::make_unique<scug::Program>();

    int _index=0;
    std::cout<<"premier parse commencé"<<std::endl;
    std::shared_ptr<scug::Instruction> instr;
    std::cout<<"premier parse fini"<<std::endl;
    while(_index<_scav->instructions().size()){
        std::cout<<"while hors"<<std::endl;
        instr = ParseInstruction(_index,_scav->instructions());
        program->add_instruction(instr);
        ++_index;
    }

    return program;
}


std::shared_ptr<scug::Instruction> Ast_Parser_Asm::ParseInstruction(int &_index,const std::vector<std::shared_ptr<scav::Instruction>> &inst){
    std::cout<<"parse instr"<<_index<<std::endl;
    while(inst[_index]->isoperation()==true && std::dynamic_pointer_cast<scav::Operation>(inst[_index])->get_dst()=="eax"){
        std::cout<<_index<<" kazueyfgqkzeuyfgqzef   "<<inst[_index]->TypeInstruction()<<std::endl;
        std::dynamic_pointer_cast<scav::Operation>(inst[_index])->update(_eax);
        ++_index;
    }
    if(inst[_index]->isoperation()==true){
        std::cout<<_index<<" operation"<<std::endl;
        // auto operation = std::dynamic_pointer_cast<scav::Operation>(inst[_index]);

        // if(operation->get_dst()=="eax"){
        //     operation->update(_eax);
        //     ++_index;

        // }
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
        else if(inst[_index]->TypeInstruction()=="MultInstruction"){
            if(operation->value()=="eax"){
                return std::make_shared<scug::Move>(operation->get_dst(),std::make_shared<scug::BinaryExpression>(BinaryOperation::kMultply,dst,_eax));
            }
            else{
                return std::make_shared<scug::Move>(operation->get_dst(),std::make_shared<scug::BinaryExpression>(BinaryOperation::kMultply,dst,operation->Evaluate()));
            }        }
        else if(inst[_index]->TypeInstruction()=="AddInstruction"){
            if(operation->value()=="eax"){
                return std::make_shared<scug::Move>(operation->get_dst(),std::make_shared<scug::BinaryExpression>(BinaryOperation::kPlus,dst,_eax));
            }
            else{
                return std::make_shared<scug::Move>(operation->get_dst(),std::make_shared<scug::BinaryExpression>(BinaryOperation::kPlus,dst,operation->Evaluate()));
            }        }
        else throw std::runtime_error("no other operation than move at the moment");
        // return ParseOperation();
    }
    else if(inst[_index]->TypeInstruction()=="FunctionDeclaration"){
        std::cout<<"function def"<<std::endl;
        auto func = std::dynamic_pointer_cast<scav::FunctionDeclaration>(inst[_index]);
        return ParseDeclaration(func);
    } 
    else if(inst[_index]->TypeInstruction()=="Return"){
        std::cout<<"ret"<<std::endl;
        return std::make_shared<scug::Return>(_eax);
    }
    else throw std::runtime_error("invalid instruction");


    // if (auto function = dynamic_cast<scav::FunctionDeclaration const*>
    //                     (instruction[0].get())) {

    // // creating nscav Function
    // auto ident = std::make_shared<scug::Identifier>(function->identifier()->name());
    // auto nasm_func = std::make_shared<scug::FunctionDeclaration>(std::move(ident));

    // if (auto ret = dynamic_cast<scav::ReturnStatement const*>
    //                 (function->instructions()[0].get())) {

    //     const std::unique_ptr<scav::Expression>& return_expression = ret->return_value();

    //     // ParseAstExpression() should return a Constant or a Variable
    //     auto nscav_return_expr = ParseAstExpression(nasm_func.get(), return_expression);

    //     nasm_func->add_instruction(std::make_shared<scug::Return>(std::move(
    //                                     nscav_return_expr)));
    //     scug->add_instruction(std::move(nasm_func));
    // }
    // }

    // return scug;
}

std::unique_ptr<scug::FunctionDeclaration> Ast_Parser_Asm::ParseDeclaration(std::shared_ptr<scav::FunctionDeclaration> func){
    auto nasm_func = std::make_unique<scug::FunctionDeclaration>(std::make_unique<scug::Identifier>(func->identifier()->name()));

    
    int _index=0;
    std::cout<<"premier parse func commencé"<<std::endl;
    std::shared_ptr<scug::Instruction> instr;
    std::cout<<"premier parse func fini"<<std::endl;
    while (_index<func->instructions().size()) {
        std::cout<<"while dans"<<std::endl;
        instr = ParseInstruction(_index,func->instructions());
        nasm_func->add_instruction(instr);
        ++_index;
    }

    return nasm_func;
}