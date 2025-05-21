#pragma once
#include "../scug/scug.hh"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Emitor_asm {
 public:
  Emitor_asm(std::unique_ptr<scug::Program> program)
    : _program(std::move(program)){}
    std::string Emit();

 private:

    std::string EmitInstruction(int &_index, std::vector<std::shared_ptr<scug::Instruction>> instr);
    std::string EmitDeclaration(std::shared_ptr<scug::FunctionDeclaration>);
    std::vector<std::string> init(std::vector<std::shared_ptr<scug::Instruction>> instr);
    std::string Declaration(std::vector<std::shared_ptr<scug::Instruction>> instr);

    std::string _indent="";
    int _index=0;
    std::shared_ptr<scug::Program> _program;
};