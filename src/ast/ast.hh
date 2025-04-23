#pragma once
#include "types.hh"
#include <vector>
#include <memory>
#include <string>

// any element executable code : Statement, Declaration, Expression, etc...
class Instruction{
    public:
        virtual std::string TypeInstruction() const =0;

    private:

};

// executable units : if, while, return
class Statement{
    public:
        Statement()=default;

    private:

};

class Identifier{
    public:
        Identifier(const std::string& name)
            :_name(name){}
        
        const std::string& name() const {return _name;}

    private:
        std::string _name;
};

// combination of variables, constants, operators and function calls that evaluate to a single value
class Expression{
    public:
        Expression()=default;

    private:
        int _value;
};

// identifiers with their type and storage
class Declaration : public Instruction{
    public:
        Declaration(std::unique_ptr<Identifier> ident, Type type)
            :_identifier(std::move(ident)), _type(type){}

        std::string TypeInstruction() const override {return "Declaration";};
        const std::unique_ptr<Identifier>& identifier() const {return _identifier;}
        Type type() const {return _type;}
    
    private:
        Type _type;
        std::unique_ptr<Identifier> _identifier;
};

class Program{
    public:
        Program()=default;

        const std::vector<std::unique_ptr<Instruction>>& instructions() const {
            return _instructions;
        }
        void add_instruction(std::unique_ptr<Instruction> instruction) {
            _instructions.push_back(std::move(instruction));
        }
    
    private:
        std::vector<std::unique_ptr<Instruction>> _instructions;
};

class ReturnStatement : public Statement{
    public:
        ReturnStatement()=default;
    
    private:
        std::unique_ptr<Expression> _return_value;
};

class FunctionDeclaration : public Declaration{
    public:
        FunctionDeclaration(std::unique_ptr<Identifier> ident, Type type)
            :Declaration(std::move(ident), type){}

        std::string TypeInstruction() const override {return "FunctionDeclaration";};

    private:
        std::vector<std::unique_ptr<Instruction>> _instructions;
};
