#pragma once
#include <string>
#include <memory>
#include <stdexcept>
#include "token.h"
#include "lexer.h"
#include "ast.h"

namespace tensorscript{

    class Parser {
        public: 
        explicit Parser(Lexer lex);
        std::unique_ptr<ModuleAST> parseModule();
        private:
        Lexer lex;
        Token currentToken;                                                                                                                     
                                                                                                                                                
        // Helper functions for inspecting tokens:                                                                                              
        Token advance();                                                                                                                        
        const Token& peek() const;                                                                                                              
        Token expect(Tokentype expectedType, const std::string& errorMsg);                                                                      
                                                                                                                                                
        // Parsing methods (from high-level to low-level):                                                                                      
        std::unique_ptr<StmtAST> parseStatement();                                                                                              
        std::unique_ptr<StmtAST> parseAssignment();                                                                                             
        std::unique_ptr<StmtAST> parseReturn();                                                                                                 
                                                                                                                                                
        std::unique_ptr<ExprAST> parseExpression(); // + and -                                                                                  
        std::unique_ptr<ExprAST> parseTerm();       // *                                                                                        
        std::unique_ptr<ExprAST> parsePrimary();    // numbers, shapes, vars, calls, (expr)  

    };

}
