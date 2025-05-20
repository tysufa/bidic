#pragma once

#include "../scug/scug.hh"
#include <memory>
#include <string>
#include <vector>
#include <map>

// enum class Register { eax, ebx, ecx, edx, ebp, esp, esi, edi };


// inline std::string RegisterToString(Register r) {
//   static const char* names[] = {"eax", "ebx", "ecx", "edx",
//                                 "ebp", "esp", "esi", "edi"
//                                };
//   return names[static_cast<int>(r)]; // 'value' is the enum's underlying integer
// }

namespace scav {
using namespace scav;


class Expression {
 public:
  virtual int Evaluate() const = 0;
  //TODO: value() should be replaced by Evaluate(), it is here juste to test
  //that you can replace Constant by Expression
  virtual int value() const = 0;

  virtual std::string ExpressionType() const = 0;

};

class Variable : public scav::Expression {
 public:
  Variable(const std::string& name)
    : _name(name) {}

  const std::string& name() const {return _name;}

  int Evaluate() const override {return 0;}
  int value() const override {return 0;}

  std::string ExpressionType() const override {return "Variable";}

 private:
  std::string _name;
};

// TODO: For now Constant is only integers, later we need to make it a virtual
// class and make subclasses for other types
class Constant : public scav::Expression {
 public:
  Constant(int value)
    : _value(value) {}

  int value() const override {return _value;}
  int Evaluate() const override {return _value;}

  std::string ExpressionType() const override {return "Constant";}

 private:
  int _value;
};

class Instruction {
 public:
  virtual std::string TypeInstruction() const = 0;
  virtual bool isoperation() const {return false;}

 private:
};

class Program {
 public:
  Program() = default;

  std::vector<std::shared_ptr<Instruction>> instructions() {
    return _instructions;
  }
  void add_instruction(std::shared_ptr<scav::Instruction> instruction) {
    _instructions.push_back(instruction);
  }

 private:
  std::vector<std::shared_ptr<Instruction>> _instructions;
};

class Identifier {
 public:
  Identifier(const std::string &name) : _name(name) {}

  const std::string& name() const { return _name; }

 private:
  std::string _name;
};

class FunctionDeclaration : public scav::Instruction {
 public:
  FunctionDeclaration(std::shared_ptr<Identifier> ident)
    : _identifier(ident) {}

  std::string TypeInstruction() const override {
    return "FunctionDeclaration";
  };

  const std::vector<std::shared_ptr<scav::Instruction>> &instructions() const {
    return _instructions;
  }

  const std::shared_ptr<Identifier>& identifier() const { return _identifier; }

  void add_instruction(std::shared_ptr<Instruction> instruction) {
    _instructions.push_back(instruction);
  }

 private:
  std::vector<std::shared_ptr<Instruction>> _instructions;
  std::shared_ptr<Identifier> _identifier;
};

class Operation : public scav::Instruction{
  public:
  Operation(std::string dst, std::string value) : _dst(dst), _value(value){}

  bool isoperation() const override {return true;}

  std::string get_dst() const {return _dst;}
  std::string value() const {return _value;}
  std::shared_ptr<scug::Expression> Evaluate() const {
    if(_value[0]=='e'){
      return std::make_shared<scug::Variable>(_value);
    }
    else{
      return std::make_shared<scug::Constant>(std::stoi(_value));
    }
  }
  virtual void update(std::shared_ptr<scug::Expression> &eax) const =0;

  private:
  std::string _dst;
  std::string _value;
};

class Move : public scav::Operation {
 public:
  Move(std::string dst, std::string value)
    : Operation(dst,value) {}

  std::string TypeInstruction() const override { return "MoveInstruction"; };

  void update(std::shared_ptr<scug::Expression> &eax) const override{
    if(value()=="eax"){
    }
    else{
      eax=Evaluate();
    }
  } 

 private:

};

class Mult : public scav::Operation {
 public:
  Mult(std::string dst, std::string value)
    : Operation(dst,value) {}

  std::string TypeInstruction() const override { return "ImulInstruction"; };


  void update(std::shared_ptr<scug::Expression> &eax) const override{
    if(value()=="eax"){
      eax=std::make_shared<scug::BinaryExpression>(BinaryOperation::kMultply, eax, eax);
    }
    eax=std::make_shared<scug::BinaryExpression>(BinaryOperation::kMultply, eax, Evaluate());
    // if(_value[0]=='e'){
    //   if(_value=="eax"){

    //   }
    //   else{
    //     eax=std::make_shared<scug::Variable>(_value);
    //   }
    //   eax=std::make_shared<scug::BinaryExpression>(BinaryOperation::kMultply, contexte[get_dst()], contexte[_value]);
    // }
    // else{
    //   eax=std::make_shared<scug::BinaryExpression>(BinaryOperation::kMultply, contexte[get_dst()],std::make_shared<scug::Constant>(std::stoi(_value)));
    // }
  } 

};

class Add : public scav::Operation {
 public:
  Add(std::string dst, std::string value)
    : Operation(dst,value) {}

  std::string TypeInstruction() const override { return "AddInstruction"; };

  void update(std::shared_ptr<scug::Expression> &eax) const override{
    if(value()=="eax"){
      eax=std::make_shared<scug::BinaryExpression>(BinaryOperation::kPlus, eax, eax);
    }
    eax=std::make_shared<scug::BinaryExpression>(BinaryOperation::kPlus, eax, Evaluate());
    // if(_value[0]=='e'){
    //   eax=std::make_shared<scug::BinaryExpression>(BinaryOperation::kPlus, contexte[get_dst()], contexte[_value]);
    // }
    // else{
    //   eax=std::make_shared<scug::BinaryExpression>(BinaryOperation::kPlus, contexte[get_dst()],std::make_shared<scug::Constant>(std::stoi(_value)));
    // }
  } 

};

class Return : public scav::Instruction {
  public:
    Return(){}

    std::string TypeInstruction() const override { return "Return";}
};

/*
class Unary : public Instruction{
 public:
  Unary(UnaryOperation unary_op, std::shared_ptr<Expression> src, std::shared_ptr<Variable> dst)
    : _unary_operation(unary_op), _src(src), _dst(dst) {}

  UnaryOperation unary_operation() const {return _unary_operation;}
  const std::shared_ptr<Variable>& dst() const {return _dst;}
  const std::shared_ptr<Expression>& src() const {return _src;}

  std::string TypeInstruction() const override {return "Unary";};

 private:
  std::shared_ptr<Expression> _src;
  std::shared_ptr<Variable> _dst;
  UnaryOperation _unary_operation;
};
*/

} // namespace scav
