#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

extern int yyparse();
extern FILE* yyin;
extern ASTNode* root;

int main(int argc, char** argv) {
    if (argc > 1) {
        FILE* file = fopen(argv[1], "r");
        if (!file) {
            fprintf(stderr, "Error: Cannot open file '%s'\n", argv[1]);
            return 1;
        }
        yyin = file;
    }
    
    int parseResult = yyparse();
    
    if (argc > 1) {
        fclose(yyin);
    }
    
    if (parseResult != 0) {
        fprintf(stderr, "Parsing failed\n");
        return 1;
    }
    
    if (root) {
        RuntimeContext ctx;
        root->execute(ctx);
        delete root;
    }
    
    return 0;
}