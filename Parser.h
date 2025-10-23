#pragma once
#include <vector>
#include <memory>
#include <stdexcept>
#include <initializer_list>
#include <variant>
#include <string>
#include <iostream>
#include "Expr.h"
#include "Token.h"

struct Parser
{
	std::vector<Token> tokens;
	int current = 0;

	bool hadError = false;

private:
	struct ParseError : public std::runtime_error {
		using std::runtime_error::runtime_error;
	};

public:
	Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {}

	std::unique_ptr<Expr> parse() {
		try {
			if (!isAtEnd()) {
				return Expression();
			}
			return nullptr;
		}
		catch (const ParseError&) {
			return nullptr; // Hata durumunda null döndür
		}
	}

private:
	std::unique_ptr<Expr> Expression() {
		return Equality();
	}

	std::unique_ptr<Expr> Equality() {
		std::unique_ptr<Expr> expr = Comparision();
		while (match({ BangEqual, EqualEqual })) {
			Token opr = previous();
			std::unique_ptr<Expr> right = Comparision();
			expr = std::make_unique<Binary>(std::move(expr), opr, std::move(right));
		}
		return expr;
	}

	std::unique_ptr<Expr> Comparision() {
		std::unique_ptr<Expr> expr = Term();
		while (match({ Greater,GreaterEqual,Less,LessEqual }))
		{
			Token opr = previous();
			std::unique_ptr<Expr> right = Term();
			expr = std::make_unique<Binary>(std::move(expr), opr, std::move(right));
		}
		return expr;
	}

	std::unique_ptr<Expr> Term() {
		std::unique_ptr<Expr> expr = Factor();
		while (match({ Minus,Plus }))
		{
			Token opr = previous();
			std::unique_ptr<Expr> right = Factor();
			expr = std::make_unique<Binary>(std::move(expr), opr, std::move(right));
		}
		return expr;
	}

	std::unique_ptr<Expr> Factor() {
		std::unique_ptr<Expr> expr = unary();
		while (match({ Slash,Star }))
		{
			Token opr = previous();
			std::unique_ptr<Expr> right = unary();
			expr = std::make_unique<Binary>(std::move(expr), opr, std::move(right));
		}
		return expr;
	}

	std::unique_ptr<Expr> unary() {
		if (match({ Bang, Minus }))
		{
			Token opr = previous();
			std::unique_ptr<Expr> right = unary();
			return std::make_unique<Unary>(opr, std::move(right));
		}
		return Primary();
	}

	std::unique_ptr<Expr> Primary() {
		if (match({ False })) return std::make_unique<Literal>(false);
		if (match({ True }))  return std::make_unique<Literal>(true);
		if (match({ Nil }))   return std::make_unique<Literal>(std::monostate{});

		if (match({ Number, String })) {
			return std::make_unique<Literal>(previous().literal);
		}

		if (match({ LeftParen })) {
			std::unique_ptr<Expr> expr = Expression();
			consume(RightParen, "Parantez ')' bekleniyor.");
			return std::make_unique<Grouping>(std::move(expr));
		}

		throw error(peek(), "İfade (expression) bekleniyor.");
	}


	Token consume(TokenType type, const char* message) {
		if (check(type)) return advance();
		throw error(peek(), message);
	}

	ParseError error(const Token& token, const std::string& message) {
		report(token, message); 
		return ParseError(message);
	}

	void report(const Token& token, const std::string& message) {
		if (token.type == Eof) {
			reportError(token.line, " at end", message);
		}
		else {
			reportError(token.line, " at '" + token.lexeme + "'", message);
		}
	}

	void reportError(int line, const std::string& where, const std::string& message) {
		std::cerr << "[line " << line << "] Hata" << where << ": " << message << std::endl;
		hadError = true; 
	}

	void synchronize() {
		advance();
		while (!isAtEnd()) {
			if (previous().type == Semicolon) return;

			switch (peek().type) {
			case Class:
			case Func:
			case Var:
			case For:
			case If:
			case While:
			case Print:
			case Return:
				return;
			default:
				;
			}
			advance();
		}
	}

	bool match(std::initializer_list<TokenType> types) {
		for (TokenType type : types) {
			if (check(type)) {
				advance();
				return true;
			}
		}
		return false;
	}

	bool check(TokenType type) {
		if (isAtEnd()) return false;
		return peek().type == type;
	}

	bool isAtEnd() {
		return peek().type == Eof;
	}

	Token peek() {
		return tokens[current];
	}

	Token previous() {
		return tokens[current - 1];
	}

	Token advance() {
		if (!isAtEnd()) current++;
		return previous();
	}
};