#include "../../src/lexer/lexer.hh"
#include "../../src/ast/ast.hh"
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
        std::string input = R"(int main(){
            return 1;
        })";
        Lexer lexer(input);
        std::vector<Token> tokens = lexer.Tokens();
        Program p;
        Declaration d;
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