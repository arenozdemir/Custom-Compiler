#include <iostream>
#include <string>
#include <vector>
#include "Scanner.h"
#include "Token.h"

int main() {
    std::string source_code = R"(
        int x = 123.45;
        var y = "inceden compiler";
        // yorum satırı
        var z = x + 10; 
        
        if (z > 100) {
            z = z + 1;
        } else {
            z = 0;
        }
        != == <= >=
    )";

    std::cout << "--- Source Code ---" << std::endl;
    std::cout << source_code << std::endl;
    std::cout << "--------------------" << std::endl;
    std::cout << "--- Tokens ---" << std::endl;

    Scanner scanner(source_code);

    std::vector<Token> tokens = scanner.scanTokens();

    for (const auto& token : tokens) {
        std::cout << token.toString() << std::endl;
    }

    return 0;
}