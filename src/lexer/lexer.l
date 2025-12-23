%{
#include <string>
#include "ast.h"
#include "parser.tab.h"

extern int yylineno;
%}

%option noyywrap
%option yylineno

%%

[ \t\r]+                { /* ignore whitespace */ }
\n                      { /* ignore newlines */ }

"//".*                  { /* single-line comment */ }
"/*"([^*]|\*+[^*/])*\*+"/" { /* multi-line comment */ }

"var"                   { return VAR; }
"if"                    { return IF; }
"else"                  { return ELSE; }
"while"                 { return WHILE; }

[a-zA-Z_][a-zA-Z0-9_]*  { 
                          yylval.str = new std::string(yytext); 
                          return IDENTIFIER; 
                        }

[0-9]+                  { 
                          yylval.num = atoi(yytext); 
                          return INTEGER; 
                        }

"=="                    { return EQ; }
"!="                    { return NE; }
"<="                    { return LE; }
">="                    { return GE; }
"<"                     { return LT; }
">"                     { return GT; }

"="                     { return ASSIGN; }
"+"                     { return PLUS; }
"-"                     { return MINUS; }
"*"                     { return MULT; }
"/"                     { return DIV; }

"("                     { return LPAREN; }
")"                     { return RPAREN; }
"{"                     { return LBRACE; }
"}"                     { return RBRACE; }
";"                     { return SEMICOLON; }

.                       { 
                          fprintf(stderr, "Line %d: Unrecognized character '%s'\n", 
                                  yylineno, yytext);
                          return yytext[0];
                        }

%%