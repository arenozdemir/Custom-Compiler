#pragma once
#include <variant>
#include <string_view>
#include <utility>
#include <type_traits>

enum TokenType
{
	TOKEN_EOF,
	TOKEN_IDENTIFIER,
	TOKEN_NUMBER,
	TOKEN_STRING,
	TOKEN_PLUS,
	TOKEN_MINUS,
	TOKEN_STAR,
	TOKEN_SLASH,
	TOKEN_LEFT_PAREN,
	TOKEN_RIGHT_PAREN,
	TOKEN_LEFT_BRACE,
	TOKEN_RIGHT_BRACE,
	TOKEN_COMMA,
	TOKEN_DOT,
	TOKEN_SEMICOLON,
	TOKEN_EQUAL,
	TOKEN_PRINT,
	TOKEN_VAR,
	TOKEN_FUNCTION,
	TOKEN_IF,
	TOKEN_ELSE,
	TOKEN_BANG,
	TOKEN_BANG_EQUAL,
	TOKEN_EQUAL_EQUAL,
	TOKEN_LESS,
	TOKEN_LESS_EQUAL,
	TOKEN_GREATER,
	TOKEN_GREATER_EQUAL,
	TOKEN_RETURN,
};

struct Token
{
	TokenType type;
	std::string lexeme;
	int line;
	std::variant<std::monostate, int, double, std::string> literal;

	Token(TokenType type, std::string lexeme, int line)
		: type(type), lexeme(std::move(lexeme)), line(line), literal(std::monostate{}) {}

	template <typename T>
	Token(TokenType type, std::string lexeme, int line, T&& literal_value)
		requires (!std::is_same_v<std::decay_t<T>, std::monostate>)
	: type(type), lexeme(std::move(lexeme)), line(line), literal(std::forward<T>(literal_value)) {}

	std::string toString() const;
};