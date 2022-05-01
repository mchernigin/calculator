%option nounput noinput noyywrap
%option reentrant never-interactive bison-bridge
%option prefix="parser"

%{
  #include "parser_type.h"
  #include "parser.tab.h"
%}

EXP ([Ee][-+]?[0-9]+)

%%

[ \t]                 ;
[+\-*/()]             { return yytext[0]; }
[0-9]+"."[0-9]*{EXP}? |
"."?[0-9]+{EXP}?      { *yylval = strtold (yytext, NULL); return NUM; }
.                     { printf ("Unknown character: \"%s\"", yytext); }

%%
