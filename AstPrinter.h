#pragma once
#pragma once
#include <sstream>
#include <string>
#include <vector>
#include <memory> 
#include "Expr.h" 

class AstPrinter : public Expr::Visitor {
public:
    std::string print(Expr* expr) {
        m_ss.str(""); 
        expr->accept(this);
        return m_ss.str();
    }

private:
    std::stringstream m_ss; 

    void visitLiteralExpr(Literal* expr) override {
        std::visit([this](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, std::monostate>) {
                m_ss << "nil";
            }
            else if constexpr (std::is_same_v<T, std::string>) {
                m_ss << "\"" << arg << "\"";
            }
            else if constexpr (std::is_same_v<T, float>) {
                m_ss << arg << "f";
            }
            else {
                m_ss << arg;
            }
            }, expr->value);
    }

    void visitUnaryExpr(Unary* expr) override {
        parenthesize(expr->op.lexeme, { expr->right.get() });
    }

    void visitBinaryExpr(Binary* expr) override {
        parenthesize(expr->op.lexeme, { expr->left.get(), expr->right.get() });
    }

    void visitGroupingExpr(Grouping* expr) override {
        parenthesize("group", { expr->expression.get() });
    }

    void parenthesize(const std::string& name, const std::vector<Expr*>& exprs) {
        m_ss << "(" << name;
        for (Expr* expr : exprs) {
            m_ss << " ";
            expr->accept(this); 
        }
        m_ss << ")";
    }
};