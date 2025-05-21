#include "ast_parser.hh"
#include "ast_parser_asm.hh"
#include "emission.hh"
#include "emission_asm.hh"
#include "lexer.hh"
#include "scug.hh"
#include "scav.hh"
#include "parser.hh"
#include "parser_asm.hh"
#include "token.hh"
#include <gtest/gtest.h>
#include <memory>
#include <string>

TEST(EmitorTest, BasicProgramTest) {
  std::string input = R"(int test(){
    return 1;
    })";
  Lexer lexer(input);
  std::vector<Token> tokens = lexer.Tokens();

  Parser parser(tokens);

  std::unique_ptr<scug::Program> scug;
  // the std::move is automatic
  scug = eval(parser.ParseProgram());

  Emitor emitor(std::move(scug));
  std::string res = emitor.Emit();

  std::string expected_res = R"(test:
  mov eax, 1
  ret
)";

  EXPECT_EQ(res, expected_res);
}

TEST(EmitorTest, UnaryOperator) {
  std::string input = R"(int test(){
    return -1;
    })";
  Lexer lexer(input);
  std::vector<Token> tokens = lexer.Tokens();

  Parser parser(tokens);

  std::unique_ptr<scug::Program> scug;
  // the std::move is automatic
  scug = eval(parser.ParseProgram());

  Emitor emitor(std::move(scug));
  std::string res = emitor.Emit();

  std::string expected_res = R"(test:
  mov [ebp-4], 1
  neg [ebp-4]
  mov eax, [ebp-4]
  ret
)";

  EXPECT_EQ(res, expected_res);
}


TEST(EmitorTest, MultipleUnaryOperators) {
  std::string input = R"(int test(){
    return ~(-1);
    })";
  Lexer lexer(input);
  std::vector<Token> tokens = lexer.Tokens();

  Parser parser(tokens);

  std::unique_ptr<scug::Program> scug;
  // the std::move is automatic
  scug = eval(parser.ParseProgram());

  Emitor emitor(std::move(scug));
  std::string res = emitor.Emit();

  std::string expected_res = R"(test:
  mov [ebp-4], 1
  neg [ebp-4]
  mov [ebp-8], [ebp-4]
  not [ebp-8]
  mov eax, [ebp-8]
  ret
)";

  EXPECT_EQ(res, expected_res);
}

TEST(EmitorTest, BinaryOperators) {
  std::string input = R"(int test(){
    return 1+2*3;
    })";
  Lexer lexer(input);
  std::vector<Token> tokens = lexer.Tokens();

  Parser parser(tokens);

  std::unique_ptr<scug::Program> scug;
  // the std::move is automatic
  scug = eval(parser.ParseProgram());

  Emitor emitor(std::move(scug));
  std::string res = emitor.Emit();

  std::string expected_res = R"(test:
  mov [ebp-4], 2
  imul [ebp-4], 3
  mov [ebp-8], [ebp-4]
  add [ebp-8], 1
  mov eax, [ebp-8]
  ret
)";

  EXPECT_EQ(res, expected_res);
}

TEST(AsmEmitorTest, BasicProgramTest) {
  std::string input = R"(func:
    mov eax, 4
    imul eax, 4
    mov [ebp-4], eax
    add [ebp-8], eax
    ret
  ret)";
  Lexer lexer(input,false);
  std::vector<Token> tokens = lexer.Tokens();

  Parser_asm parser(tokens);
  Ast_Parser_Asm ast_parser(parser.ParseProgram());

  std::unique_ptr<scug::Program> scug(ast_parser.ParseProgram());

  Emitor_asm emitor(std::move(scug));
  std::string res=emitor.Emit();

  std::string expected_res=R"(int func(){
  int ebp4,ebp8;
  ebp4 = (4*4);
  ebp8 = (ebp8+(4*4));
  return (4*4);
}
return (4*4);
)";
  std::cout<<std::endl;
  std::cout<<"resultat:"<<std::endl;
  std::cout<<emitor.Emit()<<std::endl;

  EXPECT_EQ(res, expected_res);
}

TEST(AsmEmitorTest, MainTest) {
  std::string input = R"(main:
    mov eax, 2
    imul eax, 3
    add eax, 1
    ret)";
  Lexer lexer(input,false);
  std::vector<Token> tokens = lexer.Tokens();

  Parser_asm parser(tokens);
  Ast_Parser_Asm ast_parser(parser.ParseProgram());

  std::unique_ptr<scug::Program> scug(ast_parser.ParseProgram());

  Emitor_asm emitor(std::move(scug));
  std::string res=emitor.Emit();

  std::string expected_res=R"(int main(){
  return ((2*3)+1);
}
)";

  std::cout<<std::endl;
  std::cout<<"resultat:"<<std::endl;
  std::cout<<res<<std::endl;
  std::cout<<std::endl<<"resultat attendu :"<<std::endl;
  std::cout<<expected_res<<std::endl;

  EXPECT_EQ(res, expected_res);
}

TEST(AsmEmitorTest, TestPhilemon) {
  std::string input = R"(main:  
  mov eax, 2
  imul eax, 3
  mov [ebp-4], eax
  mov [ebp-8], [ebp-4]
  add [ebp-8], 1
  mov eax, [ebp-8]
  ret)";
  Lexer lexer(input,false);
  std::vector<Token> tokens = lexer.Tokens();

  Parser_asm parser(tokens);
  Ast_Parser_Asm ast_parser(parser.ParseProgram());

  std::unique_ptr<scug::Program> scug(ast_parser.ParseProgram());

  Emitor_asm emitor(std::move(scug));
  std::string res=emitor.Emit();

  std::string expected_res=R"(int main(){
  int ebp4,ebp8;
  ebp4 = (2*3);
  ebp8 = ebp4;
  ebp8 = (ebp8+1);
  return ebp8;
}
)";

  std::cout<<std::endl;
  std::cout<<"resultat:"<<std::endl;
  std::cout<<res<<std::endl;
  std::cout<<std::endl<<"resultat attendu :"<<std::endl;
  std::cout<<expected_res<<std::endl;

  EXPECT_EQ(res, expected_res);
}

TEST(AsmEmitorTest, TestUnary) {
  std::string input = R"(test:
  mov eax, 1
  neg eax
  not eax
  ret
)";
  Lexer lexer(input,false);
  std::vector<Token> tokens = lexer.Tokens();

  Parser_asm parser(tokens);
  Ast_Parser_Asm ast_parser(parser.ParseProgram());

  std::unique_ptr<scug::Program> scug(ast_parser.ParseProgram());

  Emitor_asm emitor(std::move(scug));
  std::string res=emitor.Emit();

  std::string expected_res = R"(int test(){
  return ~(-(1));
}
)";

  std::cout<<std::endl;
  std::cout<<"resultat:"<<std::endl;
  std::cout<<res<<std::endl;
  std::cout<<std::endl<<"resultat attendu :"<<std::endl;
  std::cout<<expected_res<<std::endl;

  EXPECT_EQ(res, expected_res);
}