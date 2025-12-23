#include <iostream>

extern int yyparse();

int main() {
    std::cout << "Enter an expression:\n";
    yyparse();
    return 0;
}
