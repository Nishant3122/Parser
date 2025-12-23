%{
#include <iostream>
using namespace std;

int yylex();
void yyerror(const char *s);
%}

%token INTEGER

%%
input:
      /* empty */
    | input line
    ;

line:
      '\n'
    | expr '\n' { cout << "Parsed successfully" << endl; }
    ;

expr:
      INTEGER
    | expr '+' expr
    | expr '-' expr
    ;
%%

void yyerror(const char *s) {
    cerr << "Parse error: " << s << endl;
}
