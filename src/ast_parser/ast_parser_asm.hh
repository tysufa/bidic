#pragma once
#include "scug.hh"
#include "scav.hh"
#include <memory>
#include <map>

class Ast_Parser_Asm {
    public:
    Ast_Parser_Asm(std::unique_ptr<scav::Program> scav) : _scav(std::move(scav)) {}

    std::unique_ptr<scug::Program> ParseProgram();
    std::shared_ptr<scug::Instruction> ParseInstruction(int&,const std::vector<std::shared_ptr<scav::Instruction>>&);
    std::unique_ptr<scug::FunctionDeclaration> ParseDeclaration(std::shared_ptr<scav::FunctionDeclaration>);

    private:
    std::shared_ptr<scug::Expression> _eax;
    std::unique_ptr<scav::Program> _scav;
};