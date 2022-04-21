%{
#define YYSTYPE long double
#include "ast.h"
#include "calc.tab.h"
%}

%option nounput noinput noyywrap

EXP ([Ee][-+]?[0-9]+)

%%

[ \t]                 ;
[+\-*/()]             { return yytext[0]; }
[0-9]+"."[0-9]*{EXP}? |
"."?[0-9]+{EXP}?      { yylval = strtold(yytext, NULL); return NUM; }
.                     { printf("Unknown character: \"%s\"", yytext); }

%%
