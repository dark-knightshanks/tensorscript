#pragma once
#include <string>
#include <vector>
#include <memory>
#include <cstdint>

namespace tensorscript {
    struct ExprAST{
        virtual ~ExprAST() = default;
    };
    struct StmtAST{
        virtual ~StmtAST() = default;
    };
    struct VarExprAST : public ExprAST {
        std::string name;
        VarExprAST(std::string name): name(std::move(name)) {};
    };

    struct BinaryExprAST : public ExprAST {
        char op; //holds operators i.e. parents
        std::unique_ptr<ExprAST> lhs;
        std::unique_ptr<ExprAST> rhs;
        BinaryExprAST(char op, std::unique_ptr<ExprAST> lhs, std::unique_ptr<ExprAST> rhs):
                      op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {};
    };

    struct ShapeExprAST : public ExprAST {
        std::vector<int64_t> dims;
        ShapeExprAST(std::vector<int64_t> dims) : dims(dims) {};
    };

    struct NumExprAST : public ExprAST {
        double val;
        NumExprAST(double val) : val(val) {};
    };

    struct CallExprAST : public ExprAST {
        std::string callee;
        std::vector<std::unique_ptr<ExprAST>> args;
        CallExprAST(std::string callee, std::vector<std::unique_ptr<ExprAST>> args) :
                    callee(callee), args(std::move(args)) {};
    };

    struct AssignStmtAST : public StmtAST {
        std::string var_name;
        std::unique_ptr<ExprAST> value;
        AssignStmtAST(std::string var_name, std::unique_ptr<ExprAST> value) :
                      var_name(var_name), value(std::move(value)) {};
    };

    struct ReturnStmtAST : public StmtAST {
        std::unique_ptr<ExprAST> value;
        ReturnStmtAST(std::unique_ptr<ExprAST> value) : value(std::move(value)) {};
    };

    struct ModuleAST {
        std::vector<std::unique_ptr<StmtAST>> statements;
        ModuleAST(std::vector<std::unique_ptr<StmtAST>> statements) : statements(std::move(statements)) {};
    };


}
