#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "Token.h"
#include "Expr.h"
#include "Scanner.h"
#include "Parser.h"
#include "AstPrinter.h" 

bool run(const std::string& source)
{
    std::cout << "--- Kaynak Kod ---\n" << source << "\n";

    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scanTokens();

    Parser parser(tokens);
    std::unique_ptr<Expr> expression = parser.parse();

    if (parser.hadError) {
        std::cout << "\n>>> Ayrıştırma Başarısız (Parser Hatası).\n";
    }
    else if (expression) {
        std::cout << "\n--- AST (Lisp Stili) ---" << std::endl;
        AstPrinter printer;
        std::cout << printer.print(expression.get()) << std::endl;
    }

    std::cout << "--------------------------------\n\n";

    return parser.hadError;
}

int main()
{
    bool hadAnyError = false;

    hadAnyError = run("-5 * (1 + 20.5f)") || hadAnyError;
    hadAnyError = run("1 + 2 == 3 * 4") || hadAnyError;
    hadAnyError = run("10 * (4 + 5") || hadAnyError;
    hadAnyError = run("10 / !") || hadAnyError;
    hadAnyError = run("var a = 5;") || hadAnyError;

    return hadAnyError ? 1 : 0;
}