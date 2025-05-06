#include "ast_parser.hh"
#include "emission.hh"
#include "lexer.hh"
#include "nast.hh"
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

  std::unique_ptr<nast::Program> nast;
  // the std::move is automatic
  nast = eval(parser.ParseProgram());

  Emitor emitor;
  std::string res = emitor.Emit(std::move(nast));

  std::string expected_res = R"(.test
  mov eax, 1
  ret
)";

  EXPECT_EQ(res, expected_res);
}
