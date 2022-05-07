%option nounput noinput noyywrap
%option reentrant never-interactive bison-bridge
%option prefix="basic"

%{
    #include "basic_calc.h"
%}

EXP ([Ee][-+]?[0-9]+)

%%

[ \t]                 ;
[+\-*/()]             { return yytext[0]; }
[0-9]+"."[0-9]*{EXP}? |
"."?[0-9]+{EXP}?      { *yylval = strtold (yytext, NULL); return NUM; }
.                     { printf ("ERROR: unknown character: \"%s\"", yytext); }

%%
