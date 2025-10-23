#pragma once

#include <memory>
#include <string>
#include <variant>
#include "Token.h"

struct Binary;
struct Grouping;
struct Literal;
struct Unary;

struct Expr {
	struct Visitor {
		virtual void visitBinaryExpr(Binary* expr) = 0;
		virtual void visitGroupingExpr(Grouping* expr) = 0;
		virtual void visitLiteralExpr(Literal* expr) = 0;
		virtual void visitUnaryExpr(Unary* expr) = 0;
	};

	virtual ~Expr() = default;
	virtual void accept(Visitor* visitor) = 0;
};


struct Binary : Expr {
	std::unique_ptr<Expr> left;
	Token op;
	std::unique_ptr<Expr> right;

	Binary(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right)
		: left(std::move(left)), op(std::move(op)), right(std::move(right)) {}

	void accept(Visitor* visitor) override {
		visitor->visitBinaryExpr(this);
	}
};

struct Grouping : Expr {
	std::unique_ptr<Expr> expression;

	Grouping(std::unique_ptr<Expr> expression)
		: expression(std::move(expression)) {}

	void accept(Visitor* visitor) override {
		visitor->visitGroupingExpr(this);
	}
};

struct Literal : Expr {
	std::variant<std::monostate, bool, int, float, double, std::string> value;

	template <typename T>
	explicit Literal(T&& val)
		: value(std::forward<T>(val)) {}

	void accept(Visitor* visitor) override {
		visitor->visitLiteralExpr(this);
	}
};

struct Unary : Expr {
	Token op;
	std::unique_ptr<Expr> right;

	Unary(Token op, std::unique_ptr<Expr> right)
		: op(std::move(op)), right(std::move(right)) {}

	void accept(Visitor* visitor) override {
		visitor->visitUnaryExpr(this);
	}
};