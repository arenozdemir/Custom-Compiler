#pragma once
#include <string>
#include <vector>
#include <string_view>   
#include <variant>       
#include <utility>      
#include "Token.h"        

struct Scanner
{
    std::string source;
    std::vector<Token> tokens;
    int start = 0;
    int current = 0;
    int line = 1;

    Scanner(std::string source_code) : source(std::move(source_code)) {}

    void scanToken();
    void scanNumber();
    void scanIdentifier();
    void scanString();


    bool isAtEnd() const {
        return current >= source.length();
    }

    char advance() {
        return source[current++];
    }

    char peek() const {
        if (isAtEnd()) return '\0';
        return source[current];
    }

    bool match(char expected) {
        if (isAtEnd()) return false;
        if (source[current] != expected) return false;

        current++; 
        return true;
    }


    void addToken(TokenType type) {
        std::string_view text_view(source.data() + start, current - start);
        tokens.emplace_back(type, std::string(text_view), line);
    }

    template <typename T>
    void addToken(TokenType type, T&& literal) {
        std::string_view text_view(source.data() + start, current - start);
        tokens.emplace_back(type, std::string(text_view), line, std::forward<T>(literal));
    }

    std::vector<Token> scanTokens()
    {
        while (!isAtEnd()) {
            start = current;
            scanToken();
        }
        tokens.emplace_back(Eof, "", line);
        return std::move(tokens);
    }
};