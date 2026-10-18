#include <iostream>
#include <sstream>
#include "sema.h"

namespace tensorscript {

static std::string shapeToString(const std::vector<int64_t>& shape) {
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < shape.size(); ++i) {
        oss << shape[i] << (i + 1 < shape.size() ? "x" : "");
    }
    oss << "]";
    return oss.str();
}

void SemanticAnalyzer::analyze(ModuleAST& module) {
    SymbolTable.clear();
    for (auto& stmt : module.statements) {
        analyzeStmt(stmt.get());
    }
}

void SemanticAnalyzer::analyzeStmt(StmtAST* stmt) {
    if (auto assign = dynamic_cast<AssignStmtAST*>(stmt)) {
        auto shape = inferExpr(assign->value.get());
        SymbolTable[assign->var_name] = shape;
    } else if (auto ret = dynamic_cast<ReturnStmtAST*>(stmt)) {
        inferExpr(ret->value.get());
    }
}

std::vector<int64_t> SemanticAnalyzer::inferExpr(ExprAST* expr) {
    if (!expr) {
        throw std::runtime_error("Encountered null expression during semantic analysis");
    }

    // 1. Variable Reference: 'a'
    if (auto var = dynamic_cast<VarExprAST*>(expr)) {
        auto it = SymbolTable.find(var->name);
        if (it == SymbolTable.end()) {
            throw std::runtime_error("Use of undeclared variable: '" + var->name + "'");
        }
        var->inferredShape = it->second;
        return var->inferredShape;
    }

    // 2. Numeric Constant: '42.0'
    if (auto num = dynamic_cast<NumExprAST*>(expr)) {
        num->inferredShape = {1, 1};
        return num->inferredShape;
    }

    // 3. Shape Literal: '4x4'
    if (auto shape = dynamic_cast<ShapeExprAST*>(expr)) {
        shape->inferredShape = shape->dims;
        return shape->inferredShape;
    }

    // 4. Function Call: input(4x4) or matmul(c, b)
    if (auto call = dynamic_cast<CallExprAST*>(expr)) {
        if (call->callee == "input") {
            if (call->args.size() != 1) {
                throw std::runtime_error("'input' expects exactly 1 argument (e.g. input(4x4))");
            }
            auto shapeNode = dynamic_cast<ShapeExprAST*>(call->args[0].get());
            if (!shapeNode) {
                throw std::runtime_error("'input' argument must be a shape literal (e.g. 4x4)");
            }
            call->inferredShape = shapeNode->dims;
            return call->inferredShape;
        }

        if (call->callee == "matmul") {
            if (call->args.size() != 2) {
                throw std::runtime_error("'matmul' expects exactly 2 arguments: matmul(A, B)");
            }
            auto shapeA = inferExpr(call->args[0].get());
            auto shapeB = inferExpr(call->args[1].get());

            if (shapeA.size() != 2 || shapeB.size() != 2) {
                throw std::runtime_error("matmul requires 2D tensors, but got " + 
                                         shapeToString(shapeA) + " and " + shapeToString(shapeB));
            }

            // Check: cols(A) == rows(B)
            if (shapeA[1] != shapeB[0]) {
                throw std::runtime_error("Matmul inner dimension mismatch: " + 
                                         shapeToString(shapeA) + " vs " + shapeToString(shapeB));
            }

            // Output shape is [rows(A), cols(B)]
            call->inferredShape = {shapeA[0], shapeB[1]};
            return call->inferredShape;
        }

        throw std::runtime_error("Unknown function call: '" + call->callee + "'");
    }

    // 5. Binary Operations: +, -, *
    if (auto bin = dynamic_cast<BinaryExprAST*>(expr)) {
        auto shapeL = inferExpr(bin->lhs.get());
        auto shapeR = inferExpr(bin->rhs.get());

        if (shapeL != shapeR) {
            throw std::runtime_error(std::string("Shape mismatch in binary operator '") + bin->op + "': " + 
                                     shapeToString(shapeL) + " vs " + shapeToString(shapeR));
        }

        bin->inferredShape = shapeL;
        return bin->inferredShape;
    }

    throw std::runtime_error("Unknown expression node in semantic analyzer");
}

} // namespace tensorscript
