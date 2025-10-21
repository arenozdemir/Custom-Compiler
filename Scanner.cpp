#include "Scanner.h"
#include <iostream>
#include <cctype>
#include <charconv>

#include <unordered_map>

static const std::unordered_map<std::string_view, TokenType> keywords = {
    {"var", TokenType::TOKEN_VAR},
    {"if", TokenType::TOKEN_IF},
    {"else", TokenType::TOKEN_ELSE},
    {"print", TokenType::TOKEN_PRINT},
	{"func", TokenType::TOKEN_FUNCTION},
	{"return", TokenType::TOKEN_RETURN},
};

void Scanner::scanToken()
{
    while (true) {
        if (isAtEnd()) return;

        char c = peek();

        if (isspace(c)) {
            if (c == '\n') {
                line++;
            }
            advance(); 
            start = current; 
            continue;
        }

        if (c == '/' && current + 1 < source.length() && source[current + 1] == '/') {
            while (peek() != '\n' && !isAtEnd()) {
                advance();
            }
            continue;
        }

        break;
    }

    if (isAtEnd()) return;

    start = current; 

    char c = advance();

    switch (c) {
    case '(': addToken(TOKEN_LEFT_PAREN); break;
    case ')': addToken(TOKEN_RIGHT_PAREN); break;
    case '{': addToken(TOKEN_LEFT_BRACE); break;
    case '}': addToken(TOKEN_RIGHT_BRACE); break;
    case ',': addToken(TOKEN_COMMA); break;
    case '.': addToken(TOKEN_DOT); break;
    case '-': addToken(TOKEN_MINUS); break;
    case '+': addToken(TOKEN_PLUS); break;
    case ';': addToken(TOKEN_SEMICOLON); break;
    case '*': addToken(TOKEN_STAR); break;


    case '!': addToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG); break;
    case '=': addToken(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL); break;
    case '<': addToken(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS); break;
    case '>': addToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER); break;

    case '/':
        addToken(TOKEN_SLASH); 
        break;
    case '"':
        scanString();
        break;

    default:
        if (isdigit(c)) {
            scanNumber();
        }
        else if (isalpha(c) || c == '_') {
            scanIdentifier();
        }
        else {
            std::cerr << "[Hata Satır " << line << "] Beklenmedik karakter: '" << c << "'" << std::endl;
        }
        break;
    }
}

void Scanner::scanNumber()
{
    while (isdigit(peek())) {
        advance();
    }
    if (peek() == '.' && isdigit(source[current + 1])) {
        advance(); 
        while (isdigit(peek())) {
            advance(); 
        }
    }

    std::string_view numberView(source.data() + start, current - start);

    double value;
    auto [ptr, ec] = std::from_chars(numberView.data(), numberView.data() + numberView.size(), value);

    if (ec == std::errc()) {
        addToken(TOKEN_NUMBER, value);
    }
    else {
        std::cerr << "[Hata Satır " << line << "] Geçersiz sayı biçimi: " << numberView << std::endl;
    }
}

void Scanner::scanIdentifier()
{
    while (isalnum(peek()) || peek() == '_') {
        advance();
    }

    std::string_view text(source.data() + start, current - start);

    auto it = keywords.find(text);

    if (it == keywords.end()) {
        addToken(TOKEN_IDENTIFIER);
    }
    else {
        addToken(it->second); 
    }
}

void Scanner::scanString()
{
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') line++; 
        advance();
    }

    if (isAtEnd()) {
        std::cerr << "[Hata Satır " << line << "] Kapanış tırnak işareti ('\"') bekleniyor." << std::endl;
        return;
    }

    advance();

    std::string_view literalView(source.data() + start + 1, current - start - 2);
    addToken(TOKEN_STRING, std::string(literalView));
}