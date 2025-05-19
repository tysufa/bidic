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
    std::shared_ptr<scug::Instruction> instr = ParseInstruction(_index,_scav->instructions());
    std::cout<<"premier parse fini"<<std::endl;
    while(instr){
        ++_index;
        std::cout<<"while hors"<<std::endl;
        program->add_instruction(instr);
        instr = ParseInstruction(_index,_scav->instructions());
    }

    return program;
}


std::shared_ptr<scug::Instruction> Ast_Parser_Asm::ParseInstruction(int &_index,const std::vector<std::shared_ptr<scav::Instruction>> &inst){
    std::cout<<"parse instr"<<_index<<std::endl;
    if(inst[_index]->isoperation()==true){
        auto operation = std::dynamic_pointer_cast<scav::Operation>(inst[_index]);
        operation->update(_context);
        if(operation->get_dst()=="eax"){
            ++_index;
            while(inst[_index]->isoperation()==true && std::dynamic_pointer_cast<scav::Operation>(inst[_index])->get_dst()=="eax"){
                std::dynamic_pointer_cast<scav::Operation>(inst[_index])->update(_context);
                ++_index;
            }
            if(inst[_index]->isoperation()==true){
                auto operation=std::dynamic_pointer_cast<scav::Operation>(inst[_index]);
                operation->update(_context);
                if(inst[_index]->TypeInstruction()=="MoveInstruction"){
                    return std::make_shared<scug::Move>(operation->get_dst(),_context[operation->get_dst()]);
                }else throw std::runtime_error("no other operation than move at the moment");
                // return ParseOperation();
            }
            else if(inst[_index]->TypeInstruction()=="Return"){
                return std::make_shared<scug::Return>(_context["eax"]);
            }
        }
    }
    if(inst[_index]->TypeInstruction()=="FunctionDeclaration"){
        std::cout<<"function def"<<std::endl;
        auto func = std::dynamic_pointer_cast<scav::FunctionDeclaration>(inst[_index]);
        return ParseDeclaration(func);
    } 
    else if(inst[_index]->TypeInstruction()=="Return"){
        ++_index;
        return std::make_shared<scug::Return>(_context["eax"]);
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
    std::shared_ptr<scug::Instruction> instr = ParseInstruction(_index,func->instructions());
    std::cout<<"premier parse func fini"<<std::endl;
    while (instr) {
        ++_index;
        std::cout<<"while dans"<<std::endl;
        nasm_func->add_instruction(instr);
        instr = ParseInstruction(_index,func->instructions());
    }

    return nasm_func;
}