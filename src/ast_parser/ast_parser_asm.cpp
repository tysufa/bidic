#include "ast_parser_asm.hh"
#include "scav.hh"
#include "scug.hh"
#include "token.hh"
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

std::unique_ptr<scug::Program> ParseProgram(std::unique_ptr<scav::Program> scav) {
    auto program = std::make_unique<scug::Program>();

    const std::vector<std::shared_ptr<scav::Instruction>>& instruction = scav->instructions();
    auto it = instruction.begin();
    while(it!=instruction.end()){
        program->add_instruction(ParseInstruction(*it));
        ++it;
    }

    return program;
}


std::unique_ptr<scug::Instruction> ParseInstruction(std::shared_ptr<scav::Instruction> scav){

    if(scav->TypeInstruction()=="FunctionDeclaration"){
        auto function = std::dynamic_pointer_cast<scav::FunctionDeclaration>(scav);
        return ParseDeclaration(std::make_unique<scug::Identifier>(function->identifier()->name()),function->instructions());
    } 
    else if(scav->TypeInstruction()=="MoveInstruction"){
        auto move = std::dynamic_pointer_cast<scav::Move>(scav);
        if(move->value()[0]=='e'){
            // contexte[move->get_register()]=contexte[move->value()];
        }
        else{
            // contexte[move->get_register()]=Constant(std::stoi(move->value()));
        }
    }



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

std::unique_ptr<scug::FunctionDeclaration> ParseDeclaration(std::unique_ptr<scug::Identifier> id, const std::vector<std::shared_ptr<scav::Instruction>> &inst){
    auto nasm_func = std::make_unique<scug::FunctionDeclaration>(std::move(id));

    auto it = inst.begin();
    while (it!=inst.end()) {
        nasm_func->add_instruction(ParseInstruction(*it));
        ++it;
    }

    return nasm_func;
}