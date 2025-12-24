%option noyywrap
%option yylineno
%{
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "test.tab.h"
%}

%%
"var"      { printf("TOKEN: VAR\n"); return VAR; }
"if"       { printf("TOKEN: IF\n"); return IF; }
"else"     { printf("TOKEN: ELSE\n"); return ELSE; }
"while"    { printf("TOKEN: WHILE\n"); return WHILE; }


[0-9]+ {
    printf("TOKEN: INTEGER (%s)\n", yytext);
    yylval.ival=atoi(yytext);
    return INTEGER;
}
\"[^\"]*\" {
    printf("TOKEN: STRING (%s)\n", yytext);
     yylval.sval=strdup(yytext);
    return STRING;   // lexer-only mode
}


[a-zA-Z_][a-zA-Z0-9_]* {
    printf("TOKEN: IDENTIFIER (%s)\n", yytext);
    yylval.sval=strdup(yytext);
    return IDENTIFIER;
}

"=="       { printf("TOKEN: EQ\n"); return EQ; }
"!="       { printf("TOKEN: NEQ\n"); return NEQ; }
"<="       { printf("TOKEN: LE\n"); return LE; }
">="       { printf("TOKEN: GE\n"); return GE; }
"<"        { printf("TOKEN: LT\n"); return LT; }
">"        { printf("TOKEN: GT\n"); return GT; }
"&&"       {printf("TOKEN: AND\n"); return AND;}
"||"       {printf("TOKEN: OR\n"); return OR;}
"++"       { printf("TOKEN:INC\n"); return INC;}
"--"       { printf("TOKEN: DEC\n"); return DEC;}


"!"        {printf("TOKEN: NOT\n"); return NOT;}

"+"        { printf("TOKEN: PLUS\n"); return PLUS; }
"-"        { printf("TOKEN: MINUS\n"); return MINUS; }
"*"        { printf("TOKEN: MUL\n"); return MUL; }
"/"        { printf("TOKEN: DIV\n"); return DIV; }

"="        { printf("TOKEN: ASSIGN\n"); return ASSIGN; }
";"        { printf("TOKEN: SEMI\n"); return SEMI; }

"("        { printf("TOKEN: LPAREN\n"); return LPAREN; }
")"        { printf("TOKEN: RPAREN\n"); return RPAREN; }
"{"        { printf("TOKEN: LBRACES\n"); return LBRACES; }
"}"        { printf("TOKEN: RBRACES\n"); return RBRACES; }

[ \t\n]+   { }     
"//".*     { }     
.          { printf("UNKNOWN: %s\n", yytext); return 1; }
%%
