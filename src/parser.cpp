#include <iostream>
#include "parser.h"
#include "lexer.h"
#include "token.h"
#include "ast.h"

namespace tensorscript{
    
    Parser::Parser(Lexer lexinput) : lex(std::move(lexinput)){
        currentToken = lex.nexttoken();
    }
    Token Parser::advance(){
        auto c = currentToken;
        currentToken = lex.nexttoken();
        return c;
    }
    const Token& Parser::peek() const {
        return currentToken;
    }
    Token Parser::expect(Tokentype expectedtype, const std::string& errorMsg){
        if(peek().type == expectedtype){
            return advance();
        }
        else{
            throw std::runtime_error("Line " + std::to_string(peek().line) + ": " + errorMsg);
        }
    }
    std::unique_ptr<ModuleAST> Parser::parseModule(){
        std::vector<std::unique_ptr<StmtAST>> statements;
        while(peek().type != Tokentype::TOK_EOF){
            statements.push_back(parseStatement());
        }
       return std::make_unique<ModuleAST>(std::move(statements)); 
    }
    std::unique_ptr<StmtAST> Parser::parseStatement(){
        if(peek().type == Tokentype::RETURN){
            return parseReturn();
        }
        else if(peek().type == Tokentype::IDENTIFIER){
            return parseAssignment();
        }
        else{
            throw std::runtime_error("Line " + std::to_string(peek().line) +                                                                        
                                 ": Expected statement, but found '" + peek().lexeme + "'");
        }
    }
    std::unique_ptr<StmtAST> Parser::parseReturn(){
        advance();
        auto expr = parseExpression();
        return std::make_unique<ReturnStmtAST>(std::move(expr));
    }
    std::unique_ptr<StmtAST> Parser::parseAssignment() {
        std::string var_name = advance().lexeme;
        expect(Tokentype::EQUAL, "Expected '=' after variable name");
        auto expr = parseExpression();
        return std::make_unique<AssignStmtAST>(var_name, std::move(expr));
    }
    std::unique_ptr<ExprAST> Parser::parseExpression() {
        auto lhs = parseTerm();
        while(peek().type == Tokentype::ADD || peek().type == Tokentype::SUB) {
            char op = advance().lexeme[0];
            auto rhs = parseTerm();
            lhs = std::make_unique<BinaryExprAST>(op, std::move(lhs), std::move(rhs));
        }
        return lhs;
    }
    std::unique_ptr<ExprAST> Parser::parseTerm() {
        auto lhs = parsePrimary();
        while(peek().type == Tokentype::MUL) {
            char op = advance().lexeme[0];
            auto rhs = parsePrimary();
            lhs = std::make_unique<BinaryExprAST>(op, std::move(lhs), std::move(rhs));
        }
        return lhs;
    }
    std::unique_ptr<ExprAST> Parser::parsePrimary() {
        if(peek().type == Tokentype::NUM){
            auto tok = advance();
            return std::make_unique<NumExprAST>(std::move(tok.val));
        }
        else if(peek().type == Tokentype::SHAPE){
            auto tok = advance();
            return std::make_unique<ShapeExprAST>(std::move(tok.dims));
        }
        else if(peek().type == Tokentype::L_PAREN){
            advance();
            auto expr = parseExpression();
            expect(Tokentype::R_PAREN, "Expected ')' after expression");
            return expr;
        }
        else if(peek().type == Tokentype::MATMUL||
                peek().type == Tokentype::INPUT || 
                peek().type == Tokentype::IDENTIFIER){
            Token nameTok = advance();                                                                                                              
                                                                                                                                                
        // Is it followed by '('?                                                                                                               
            if (peek().type == Tokentype::L_PAREN) {                                                                                                
            // IT'S A FUNCTION CALL! (input or matmul)                                                                                          
                advance(); // consume '('                                                                                                           
                                                                                                                                            
                std::vector<std::unique_ptr<ExprAST>> args;                                                                                         
                if (peek().type != Tokentype::R_PAREN) {                                                                                            
                // Parse arguments separated by comma:                                                                                          
                    while (true) {                                                                                                                  
                        args.push_back(parseExpression());                                                                                          
                        if (peek().type == Tokentype::COMMA) {                                                                                      
                            advance(); // consume ','                                                                                               
                        } else {                                                                                                                    
                            break;                                                                                                                  
                        }                                                                                                                              
                    }                                                                                                                               
                }                                                                                                                                   
                expect(Tokentype::R_PAREN, "Expected ')' after function arguments");                                                                
                return std::make_unique<CallExprAST>(nameTok.lexeme, std::move(args));                                                              
            }
            return std::make_unique<VarExprAST>(nameTok.lexeme);
        }
        else {
           throw std::runtime_error("Line " + std::to_string(peek().line) + 
                                         ": Expected expression, but found '" + peek().lexeme + "'");
        }
    }
    
}
