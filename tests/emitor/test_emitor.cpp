#include "ast_parser.hh"
#include "emission.hh"
#include "lexer.hh"
#include "scug.hh"
#include "parser.hh"
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

  std::string expected_res = R"(.test
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

  std::string expected_res = R"(.test
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

  std::string expected_res = R"(.test
  mov [ebp-4], 1
  neg [ebp-4]
  mov [ebp-8], [ebp-4]
  not [ebp-8]
  mov eax, [ebp-8]
  ret
)";

  EXPECT_EQ(res, expected_res);
}
