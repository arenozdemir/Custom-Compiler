#pragma once
#include <variant>
#include <string_view>
#include <utility>
#include <type_traits>

enum TokenType {
    LeftParen, 
    RightParen, 
    LeftBrace,  
    RightBrace,
    Comma,      
    Dot,   
    Minus,   
    Plus,     
    Semicolon,  
    Star,      
    Slash,      

    Bang,       
    BangEqual,   
    Equal,       
    EqualEqual,
    Greater,     
    GreaterEqual, 
    Less,       
    LessEqual,    

    Identifier,
    String,     
    Number,    

    And,    
    Class,  
    Else,   
    False, 
    For,   
    Func,   
    If,    
    Nil,   
    Or,    
    Print,  
    Return, 
    Super,  
    This,   
    True,  
    Var,   
    While, 

    Error,    
    Eof       
};

struct Token
{
	TokenType type;
	std::string lexeme;
	int line;
	std::variant<std::monostate, int, double, float, std::string> literal;

	Token(TokenType type, std::string lexeme, int line)
		: type(type), lexeme(std::move(lexeme)), line(line), literal(std::monostate{}) {}

	template <typename T>
	Token(TokenType type, std::string lexeme, int line, T&& literal_value)
		requires (!std::is_same_v<std::decay_t<T>, std::monostate>)
	: type(type), lexeme(std::move(lexeme)), line(line), literal(std::forward<T>(literal_value)) {}

	std::string toString() const;
};