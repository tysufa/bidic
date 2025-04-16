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
        Lexer lexer("int test = 3;");
        TEST(lexer.input() == "int test = 3;");
        TEST(lexer.Tokens()[0].type == TokenType::kInt);
        TEST(lexer.Tokens()[1].type == TokenType::kVariable);
        TEST(lexer.Tokens()[1].value == "test");
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