%option noinput nounput noyywrap
%option reentrant never-interactive bison-bridge

%{
    #include "config.h"
    #include "parser.h"
    #ifndef YY_DO_BEFORE_ACTION
    #   include "lexer.h"
    #endif
%}

EXP ([Ee][-+]?[0-9]+)

%%

[ \t]                 ;
[+\-*/()]             { return yytext[0]; }
[0-9]+"."[0-9]*{EXP}? |
"."?[0-9]+{EXP}?      { *yylval = strtold (yytext, NULL); return NUM; }
.                     { printf ("ERROR: unknown character: \"%s\"", yytext); }

%%
