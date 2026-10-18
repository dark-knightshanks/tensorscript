#include "iostream"
#include "lexer.h"
#include "parser.h"
using namespace tensorscript;

int main(){
    std::string source = "a = input(4x4)\nb = input(4x4)\nc = a + b\nd = matmul(c, b)\nreturn d";
    Lexer lex(source);
    Parser parse(std::move(lex));
    auto module = parse.parseModule();
    std::cout << "Succesfully parsed " << module->statements.size()<< " statements" << std::endl;
    return 0;
}
