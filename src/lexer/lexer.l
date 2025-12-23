%{
#include "parser.tab.h"
#include <cstdlib>
%}

%option noyywrap

%%
[0-9]+      { return INTEGER; }
"+"         { return '+'; }
"-"         { return '-'; }
"\n"        { return '\n'; }
[ \t]+      ;     /* ignore whitespace */
.           ;     /* ignore everything else */
%%
