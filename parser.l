%option nounput noinput noyywrap
%option reentrant never-interactive bison-bridge

%{
  #define PARSERSTYPE long double
  #define YYSTYPE PARSERSTYPE
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
