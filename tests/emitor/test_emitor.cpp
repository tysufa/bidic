#include "lexer.hh"
#include "token.hh"
#include "parser.hh"
#include "nast.hh"
#include "ast_parser.hh"
#include <memory>
#include <string>
#include <gtest/gtest.h>

TEST(AstParserTest, BasicProgramTest) {
  std::string input = R"(int test(){
    return 1;
    })";
  Lexer lexer(input);
  std::vector<Token> tokens = lexer.Tokens();

  Parser parser(tokens);

  std::unique_ptr<nast::Program> res;
  // the std::move is automatic
  res = eval(parser.ParseProgram());
}
