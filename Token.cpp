#include "Token.h"
#include <sstream> 
#include <string>  

std::string tokenTypeToString(TokenType type) {
    switch (type) {
    case TokenType::TOKEN_LEFT_PAREN:    return "LeftParen";
    case TokenType::TOKEN_RIGHT_PAREN:   return "RightParen";
    case TokenType::TOKEN_LEFT_BRACE:    return "LeftBrace";
    case TokenType::TOKEN_RIGHT_BRACE:   return "RightBrace";
    case TokenType::TOKEN_COMMA:        return "Comma";
    case TokenType::TOKEN_DOT:          return "Dot";
    case TokenType::TOKEN_MINUS:        return "Minus";
    case TokenType::TOKEN_PLUS:         return "Plus";
    case TokenType::TOKEN_SEMICOLON:    return "Semicolon";
    case TokenType::TOKEN_SLASH:        return "Slash";
    case TokenType::TOKEN_STAR:         return "Star";
    case TokenType::TOKEN_BANG:         return "Bang";
    case TokenType::TOKEN_BANG_EQUAL:    return "BangEqual";
    case TokenType::TOKEN_EQUAL:        return "Equal";
    case TokenType::TOKEN_EQUAL_EQUAL:   return "EqualEqual";
    case TokenType::TOKEN_GREATER:      return "Greater";
    case TokenType::TOKEN_GREATER_EQUAL: return "GreaterEqual";
    case TokenType::TOKEN_LESS:         return "Less";
    case TokenType::TOKEN_LESS_EQUAL:    return "LessEqual";
    case TokenType::TOKEN_IDENTIFIER:   return "Identifier";
    case TokenType::TOKEN_STRING:       return "String";
    case TokenType::TOKEN_NUMBER:       return "Number";
        //case TokenType::TOKEN_AND:          return "And";
        //case TokenType::Class:        return "Class";
    case TokenType::TOKEN_ELSE:         return "Else";
        //case TokenType::False:        return "False";
        //case TokenType::For:          return "For";
    case TokenType::TOKEN_FUNCTION:          return "Function";
    case TokenType::TOKEN_IF:           return "If";
        //case TokenType::Nil:          return "Nil";
        //case TokenType::Or:           return "Or";
    case TokenType::TOKEN_PRINT:        return "Print";
    case TokenType::TOKEN_RETURN:       return "Return";
        //case TokenType::Super:        return "Super";
        //case TokenType::This:         return "This";
        //case TokenType::True:         return "True";
    case TokenType::TOKEN_VAR:          return "Var";
        //case TokenType::While:        return "While";
        //case TokenType::Error:        return "Error";
    case TokenType::TOKEN_EOF:          return "Eof";
    default:                    return "Unknown"; // Her ihtimale karşı
    }
}

std::string Token::toString() const
{
    std::stringstream ss;

	ss << "Type: " << tokenTypeToString(type)
        << "\t| Lexeme: '" << lexeme << "'"
        << "\t| Literal: ";

    std::visit([&ss](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;

        if constexpr (std::is_same_v<T, std::monostate>) {
            ss << "null";
        }
        else if constexpr (std::is_same_v<T, std::string>) {
            ss << "\"" << arg << "\""; 
        }
        else {
            ss << arg; 
        }
        }, literal);

    return ss.str();
}
