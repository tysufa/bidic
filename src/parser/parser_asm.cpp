#include "parser_asm.hh"
#include "token.hh"
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

std::unique_ptr<scav::Program> Parser_asm::ParseProgram() {
  auto program = std::make_unique<scav::Program>();
  _current_token_index = -2;
  ConsumeToken();
  ConsumeToken();

  std::shared_ptr<scav::Instruction> instr = ParseInstruction();
  while(instr){
    program->add_instruction(instr);
    instr = ParseInstruction();
  }

  return std::move(program);
}

std::shared_ptr<scav::Instruction> Parser_asm::ParseInstruction() {
  switch (_current_token.type) {
    case TokenType::kIdentifier:
      return ParseFunctionDeclaration();
      break;
    case TokenType::kMov:
      return ParseMove();
      break;
    case TokenType::kImul:
      return ParseMult();
      break;
    case TokenType::kAdd:
      return ParseAdd();
      break;
    case TokenType::kNeg:
      return ParseNeg();
      break;
    case TokenType::kNot:
      return ParseNot();
      break;
    case TokenType::kReturn:
      return ParseReturn();
      break;
    case TokenType::kEof:
      return nullptr;
      break;

    default:
      throw std::runtime_error("Expected instruction, got " +
                               StringTokenType(_current_token.type) + " instead");
  }
}

std::shared_ptr<scav::FunctionDeclaration> Parser_asm::ParseFunctionDeclaration(){
  
  auto function =
      std::make_shared<scav::FunctionDeclaration>(std::make_shared<scav::Identifier>(ConsumeToken()));
  
  CheckCurToken(TokenType::kColon);
  CheckCurToken(TokenType::kNL);

  std::shared_ptr<scav::Instruction> function_instruction;
  std::string type("");

  while (type!="Return") {
    function_instruction = ParseInstruction();
    if(!function_instruction){
      throw std::runtime_error("Cannot end a function without a ret statement");
    }
    else{
      type=function_instruction.get()->TypeInstruction();
      function->add_instruction(function_instruction);
    }
  }

  return function;
}

std::shared_ptr<scav::Move> Parser_asm::ParseMove(){
  ConsumeToken();
  std::string reg=ParseRegister();
  CheckCurToken(TokenType::kComma);
  std::string value;
  if(_current_token.type==TokenType::kNumber){
    value=CheckCurToken(TokenType::kNumber);
  }
  else value=ParseRegister();
  CheckCurToken(TokenType::kNL);
  return std::make_shared<scav::Move>(reg,value);
}

std::shared_ptr<scav::Mult> Parser_asm::ParseMult(){
  ConsumeToken();
  std::string reg=ParseRegister();
  CheckCurToken(TokenType::kComma);
  std::string value;
  if(_current_token.type==TokenType::kNumber){
    value=CheckCurToken(TokenType::kNumber);
  }
  else value=ParseRegister();
  CheckCurToken(TokenType::kNL);
  return std::make_shared<scav::Mult>(reg,value);
}

std::shared_ptr<scav::Add> Parser_asm::ParseAdd(){
  ConsumeToken();
  std::string reg=ParseRegister();
  CheckCurToken(TokenType::kComma);
  std::string value;
  if(_current_token.type==TokenType::kNumber){
    value=CheckCurToken(TokenType::kNumber);
  }
  else value=ParseRegister();
  CheckCurToken(TokenType::kNL);
  return std::make_shared<scav::Add>(reg,value);
}

std::shared_ptr<scav::Neg> Parser_asm::ParseNeg(){
  ConsumeToken();
  std::string reg=ParseRegister();
  CheckCurToken(TokenType::kNL);
  return std::make_shared<scav::Neg>(reg);
}

std::shared_ptr<scav::Not> Parser_asm::ParseNot(){
  ConsumeToken();
  std::string reg=ParseRegister();
  CheckCurToken(TokenType::kNL);
  return std::make_shared<scav::Not>(reg);
}

std::string Parser_asm::ParseRegister(){
    if(_current_token.type==TokenType::kRegister)
      return ConsumeToken();
    else{
      CheckCurToken(TokenType::kLeftSquareBracket);
      std::string reg=CheckCurToken(TokenType::kRegister);
      std::string s;
      if(reg=="ebp"){
        s="ebp";
      }
      else throw std::runtime_error("Expected ebp register, got " +
                               StringTokenType(_current_token.type) + "register instead");
      CheckCurToken(TokenType::kMinus);
      s+=CheckCurToken(TokenType::kNumber);
      CheckCurToken(TokenType::kRightSquareBracket);
      return s;
    }
}

std::shared_ptr<scav::Return> Parser_asm::ParseReturn(){
  CheckCurToken(TokenType::kReturn);
  CheckCurToken(TokenType::kNL);
  return std::make_shared<scav::Return>();
}

std::string Parser_asm::ConsumeToken(){
  std::string s=_current_token.value;
  _current_token_index++;
  _current_token = _next_token;

  if (_current_token_index + 1 < _tokens.size()){
    _next_token = _tokens[_current_token_index + 1];

  }
  else{
    _next_token.type = TokenType::kEof;
  }
  return s;
}

std::string Parser_asm::CheckCurToken(const TokenType &tok) {
  if (_current_token.type == tok) {
    return ConsumeToken();
  } else{
    throw std::runtime_error("expected current token " + StringTokenType(tok) +
                          ", got " + StringTokenType(_current_token.type) +
                          " instead");
  }
    
}
