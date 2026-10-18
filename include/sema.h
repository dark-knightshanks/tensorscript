#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include "ast.h"

namespace tensorscript{
    class SemanticAnalyzer{
        public:
        void analyze(ModuleAST& module);
        private:
        std::unordered_map<std::string, std::vector<int64_t>> SymbolTable;
        void analyzeStmt(StmtAST* stmt);
        std::vector<int64_t> inferExpr(ExprAST* expr);
    };
}
