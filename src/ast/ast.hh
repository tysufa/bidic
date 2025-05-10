#pragma once
#include "token.hh"
#include "types.hh"
#include <variant>
#include <vector>
#include <memory>
#include <string>

using LiteralValue = std::variant<int, float, std::string>;

// any element executable code : Statement, Declaration, Expression, etc...
class Instruction {
 public:
  virtual std::string TypeInstruction() const = 0;

 private:

};

// executable units : if, while, return
class Statement : public Instruction {
 public:
  Statement() = default;

 private:

};

class Identifier {
 public:
  Identifier(const std::string& name)
    : _name(name) {}

  const std::string& name() const {
    return _name;
  }

 private:
  std::string _name;
};

class Literal {
 public:
  virtual std::string DebugResult() const = 0;

  virtual LiteralValue value() const = 0;

};

class IntLiteral : public Literal {
 public:
  IntLiteral(int val)
    : _value(val) {}

  std::string DebugResult() const override { return std::to_string(_value); }

  LiteralValue value() const override { return _value; }

 private:
  int _value;
};

// combination of variables, constants, operators and function calls that evaluate to a single value
class Expression {
 public:
  virtual ~Expression() = default;

  virtual std::unique_ptr<Literal> Evaluate() const = 0;
  virtual std::string ExpressionType() const = 0;
};

class IntExpression : public Expression {
 public:
  IntExpression(int value)
    : _value(value) {}

  std::unique_ptr<Literal> Evaluate() const override {
    return std::make_unique<IntLiteral>(_value);
  };

  std::string ExpressionType() const override {return "Int";}

 private:
  int _value;
};

class PrefixExpression : public Expression {
 public:
  PrefixExpression(TokenType t, std::unique_ptr<Expression> value)
    : _prefix_type(t), _expression_value(std::move(value)) {}

  const std::unique_ptr<Expression>& expression_value() const {return _expression_value;}
  TokenType prefix_type() const {return _prefix_type;}

  std::unique_ptr<Literal> Evaluate() const override;
  std::string ExpressionType() const override {return "Prefix";}

 private:
  TokenType _prefix_type;
  std::unique_ptr<Expression> _expression_value;
};


// identifiers with their type and storage
class Declaration : public Instruction {
 public:
  Declaration(std::unique_ptr<Identifier> ident, Type type)
    : _identifier(std::move(ident)), _type(type) {}

  std::string TypeInstruction() const override {
    return "Declaration";
  };
  const std::unique_ptr<Identifier>& identifier() const {
    return _identifier;
  }
  Type type() const {
    return _type;
  }

 private:
  Type _type;
  std::unique_ptr<Identifier> _identifier;
};

class Program {
 public:
  Program() = default;

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
  ReturnStatement(std::unique_ptr<Expression> return_value)
    : _return_value(std::move(return_value)) {}

  std::string TypeInstruction() const override { return "ReturnStatement"; };

  const std::unique_ptr<Expression>& return_value() const { return _return_value; };

 private:
  std::unique_ptr<Expression> _return_value;
};

class FunctionDeclaration : public Declaration {
 public:
  FunctionDeclaration(std::unique_ptr<Identifier> ident, Type type)
    : Declaration(std::move(ident), type) {}

  std::string TypeInstruction() const override {
    return "FunctionDeclaration";
  };

  const std::vector<std::unique_ptr<Instruction>>& instructions() const {
    return _instructions;
  }

  void add_instruction(std::unique_ptr<Instruction> instruction) {
    _instructions.push_back(std::move(instruction));
  }


 private:
  std::vector<std::unique_ptr<Instruction>> _instructions;
};
