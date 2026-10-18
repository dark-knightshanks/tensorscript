#include <iostream>
#include "lexer.h"
#include "parser.h"
#include "sema.h"

using namespace tensorscript;

int main() {
    // Test 1: Valid Program
    std::cout << "--- Test 1: Valid Program ---\n";
    std::string validSource = 
        "a = input(4x4)\n"
        "b = input(4x4)\n"
        "c = a + b\n"
        "d = matmul(c, b)\n"
        "return d";

    try {
        Lexer lex(validSource);
        Parser parser(std::move(lex));
        auto module = parser.parseModule();

        SemanticAnalyzer sema;
        sema.analyze(*module);
        std::cout << "SUCCESS: Valid program parsed and verified!\n\n";
    } catch (const std::exception& e) {
        std::cerr << "FAILED: " << e.what() << "\n\n";
    }

    // Test 2: Invalid Matmul Shapes (4x3 times 4x4)
    std::cout << "--- Test 2: Intentionally Invalid Matmul (4x3 times 4x4) ---\n";
    std::string invalidMatmul = 
        "a = input(4x3)\n"
        "b = input(4x4)\n"
        "c = matmul(a, b)\n"
        "return c";

    try {
        Lexer lex(invalidMatmul);
        Parser parser(std::move(lex));
        auto module = parser.parseModule();

        SemanticAnalyzer sema;
        sema.analyze(*module);
        std::cout << "FAILED: Should have thrown dimension mismatch!\n\n";
    } catch (const std::exception& e) {
        std::cout << "CORRECTLY CAUGHT ERROR: " << e.what() << "\n\n";
    }

    // Test 3: Undeclared Variable
    std::cout << "--- Test 3: Undeclared Variable ---\n";
    std::string undeclaredVar = 
        "a = input(4x4)\n"
        "c = a + unknown_var\n"
        "return c";

    try {
        Lexer lex(undeclaredVar);
        Parser parser(std::move(lex));
        auto module = parser.parseModule();

        SemanticAnalyzer sema;
        sema.analyze(*module);
        std::cout << "FAILED: Should have thrown undeclared variable error!\n\n";
    } catch (const std::exception& e) {
        std::cout << "CORRECTLY CAUGHT ERROR: " << e.what() << "\n\n";
    }

    return 0;
}
