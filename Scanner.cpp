#include "Scanner.h"
#include <iostream>
#include <cctype>
#include <charconv>

#include <unordered_map>

static const std::unordered_map<std::string_view, TokenType> keywords = {
    {"var",    Var},
    {"if",     If},
    {"else",   Else},
    {"print",  Print},
    {"func",   Func},
    {"for",    For},
    {"while",  While},
    {"true",   True},
    {"false",  False},
    {"nil",    Nil},
    {"return", Return},
    {"and",    And},
    {"class",  Class},
    {"or",     Or},
    {"super",  Super},
    {"this",   This}
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
        if (c == '/' && current + 1 < source.length() && source[current + 1] == '*') {
            advance();
            advance();
            while (true) {
                if (isAtEnd()) {
                    std::cerr << "[Hata Satır " << line << "] Çok satırlı yorum kapanış işareti ('*/') bekleniyor." << std::endl;
                    return;
                }
                if (peek() == '*' && current + 1 < source.length() && source[current + 1] == '/') {
                    advance();
                    advance();
                    break;
                }
                if (peek() == '\n') {
                    line++;
                }
                advance();
            }
            start = current;
            continue;
        }
        break;
    }
    if (isAtEnd()) return;

    start = current;

    char c = advance();

    switch (c) {
    case '(': addToken(LeftParen); break;
    case ')': addToken(RightParen); break;
    case '{': addToken(LeftBrace); break;
    case '}': addToken(RightBrace); break;
    case ',': addToken(Comma); break;
    case '.': addToken(Dot); break;
    case '-': addToken(Minus); break;
    case '+': addToken(Plus); break;
    case ';': addToken(Semicolon); break;
    case '*': addToken(Star); break;


    case '!': addToken(match('=') ? BangEqual : Bang); break;
    case '=': addToken(match('=') ? EqualEqual : Equal); break;
    case '<': addToken(match('=') ? LessEqual : Less); break;
    case '>': addToken(match('=') ? GreaterEqual : Greater); break;

    case '/':
        addToken(Slash);
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

    bool isDecimal = false;

    if (peek() == '.') {
        if (isdigit(source[current + 1]) || source[current + 1] == 'f' || source[current + 1] == 'F')
        {
            isDecimal = true;
            advance();
            while (isdigit(peek())) {
                advance();
            }
        }
    }

    bool isFloat = false;
    if (peek() == 'f' || peek() == 'F') {
        isFloat = true;
        advance();
    }
    std::string_view numberView(source.data() + start, current - start);

    if (isFloat) {
        std::string_view parseView(numberView.data(), numberView.size() - 1);
        float value;
        auto [ptr, ec] = std::from_chars(parseView.data(), parseView.data() + parseView.size(), value);

        if (ec == std::errc()) {
            addToken(Number, value);
        }
        else {
            std::cerr << "[Hata Satır " << line << "] Geçersiz float biçimi: " << parseView << std::endl;
        }
    }
    else if (isDecimal) {
        double value;
        auto [ptr, ec] = std::from_chars(numberView.data(), numberView.data() + numberView.size(), value);

        if (ec == std::errc()) {
            addToken(Number, value);
        }
        else {
            std::cerr << "[Hata Satır " << line << "] Geçersiz double biçimi: " << numberView << std::endl;
        }
    }
    else {
        int value;
        auto [ptr_int, ec_int] = std::from_chars(numberView.data(), numberView.data() + numberView.size(), value);

        if (ec_int == std::errc()) {
            addToken(Number, value);
        }
        else if (ec_int == std::errc::result_out_of_range) {
            double double_value;
            auto [ptr_dbl, ec_dbl] = std::from_chars(numberView.data(), numberView.data() + numberView.size(), double_value);

            if (ec_dbl == std::errc()) {
                addToken(Number, double_value);
            }
            else {
                std::cerr << "[Hata Satır " << line << "] Geçersiz sayı (int overflow, double parse failed): " << numberView << std::endl;
            }
        }
        else {
            std::cerr << "[Hata Satır " << line << "] Geçersiz tamsayı biçimi: " << numberView << std::endl;
        }
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
        addToken(Identifier);
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
    addToken(String, std::string(literalView));
}