%{
#include "calc.tab.h"
%}

%option nounput
%option noinput
%option noyywrap

%%

[ \t]   ;
[0-9]+  { yylval = atoi(yytext); return NUM; }
[+\-*/] { return yytext[0]; }
[()]    { return yytext[0]; }
.       { printf("Unknown character: \"%s\"", yytext); }

%%
