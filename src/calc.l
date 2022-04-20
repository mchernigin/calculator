%{
#define YYSTYPE long double
#include "calc.tab.h"
%}

%option nounput
%option noinput
%option noyywrap

%%

[ \t]                   ;
[+-]?([0-9]*[.])?[0-9]+ { yylval = strtold(yytext, NULL); return NUM; }
[+\-*/]                 { return yytext[0]; }
[()]                    { return yytext[0]; }
.                       { printf("Unknown character: \"%s\"", yytext); }

%%
