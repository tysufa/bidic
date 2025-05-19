#pragma once
#include "scug.hh"
#include "scav.hh"
#include <memory>
#include <map>

class Ast_Parser_Asm {
    public:
    Ast_Parser_Asm(std::unique_ptr<scav::Program> scav) : _context(), _scav(std::move(scav)) {}

    std::unique_ptr<scug::Program> ParseProgram();
    std::shared_ptr<scug::Instruction> ParseInstruction(int&,const std::vector<std::shared_ptr<scav::Instruction>>&);
    std::unique_ptr<scug::FunctionDeclaration> ParseDeclaration(std::shared_ptr<scav::FunctionDeclaration>);


    std::shared_ptr<scug::Expression> get_contexte(std::string id){ return _context[id];}
    void set_context(std::string id, std::shared_ptr<scug::Expression> inst){_context[id]=inst;} 

    private:
    std::map<std::string,std::shared_ptr<scug::Expression>> _context;
    std::unique_ptr<scav::Program> _scav;
    // std::unique_ptr<scug::Program> scug;
};