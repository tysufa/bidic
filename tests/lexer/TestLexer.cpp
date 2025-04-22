#include "../../src/lexer/lexer.hh"
#include <iostream>
#include <string>

// Simple test macro
#define TEST(condition) \
    if (!(condition)) { \
        std::cerr << "FAIL: " << #condition << " (line " << __LINE__ << ")\n"; \
        success = false; \
    } else { \
        std::cout << "PASS: " << #condition << "\n"; \
    }

void runLexerTests(){
    bool success = true;
    // Test 1 : input
    {
        std::string input = "int test = 34;";
        Lexer lexer(input);
        std::vector<Token> tokens = lexer.Tokens();
        TEST(lexer.input() == input);
        TEST(tokens[0].type == TokenType::kInt);
        TEST(tokens[1].type == TokenType::kVariable);
        TEST(tokens[1].value == "test");
        TEST(tokens[2].type == TokenType::kEqual);
        TEST(tokens[3].type == TokenType::kNumber);
        TEST(tokens[3].value == "34");
        TEST(tokens[4].type == TokenType::kSemiColon);
    }

    if (!success) {
        std::cerr << "\n⚠️  Some tests failed!\n";
        exit(1);
    }
    std::cout << "\n✅ All tests passed!\n";
}

int main() {
    runLexerTests();
    return 0;
}
