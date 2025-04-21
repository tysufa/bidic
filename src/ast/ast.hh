#pragma once
#include "types.hh"
#include <vector>
#include <memory>
#include <string>

// executable units : if, while, return
class Statement{
    public:
        Statement()=default;

    private:

};

class Identifier{
    public:
        Identifier()=default;

    private:
        std::string _name;
};

// identifiers with their type and storage
class Declaration{
    public:
        Declaration()=default;
    
    private:
        Type _type;
        std::unique_ptr<Identifier> _identifier;
};

// combination of variables, constants, operators and function calls that evaluate to a single value
class Expression{
    public:
        Expression()=default;

    private:
        int _value;
};

class Program{
    public:
        Program()=default;
    
    private:
        std::vector<std::unique_ptr<Statement>> _statements;
};

class ReturnStatement : public Statement{
    public:
        ReturnStatement()=default;
    
    private:
        std::unique_ptr<Expression> _return_value;
};

class FunctionDefinition : public Declaration{
    public:
        FunctionDefinition()=default;

    private:

};