#include "Token.h"
#include <sstream> 
#include <string>  

std::string tokenTypeToString(TokenType type) {
    switch (type) {
    case LeftParen:    return "LeftParen";
    case RightParen:   return "RightParen";
    case LeftBrace:    return "LeftBrace";
    case RightBrace:   return "RightBrace";
    case Comma:        return "Comma";
    case Dot:          return "Dot";
    case Minus:        return "Minus";
    case Plus:         return "Plus";
    case Semicolon:    return "Semicolon";
    case Slash:        return "Slash";
    case Star:         return "Star";
    case Bang:         return "Bang";
    case BangEqual:    return "BangEqual";
    case Equal:        return "Equal";
    case EqualEqual:   return "EqualEqual";
    case Greater:      return "Greater";
    case GreaterEqual: return "GreaterEqual";
    case Less:         return "Less";
    case LessEqual:    return "LessEqual";
    case Identifier:   return "Identifier";
    case String:       return "String";
    case Number:       return "Number";
    case And:          return "And";
    case Class:        return "Class";
    case Else:         return "Else";
    case False:        return "False";
    case For:          return "For";
    case Func:         return "Func"; 
    case If:           return "If";
    case Nil:          return "Nil";
    case Or:           return "Or";
    case Print:        return "Print";
    case Return:       return "Return";
    case Super:        return "Super";
    case This:         return "This";
    case True:         return "True";
    case Var:          return "Var";
    case While:        return "While";
    case Error:        return "Error";
    case Eof:          return "Eof"; 
    default:           return "Unknown";
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
