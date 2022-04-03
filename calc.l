%{
#include "calc.tab.h"
#define YY_DECL int yylex()
%}

%%

[ \t]   ;
[\n]    { return EOL; }
[0-9]+  { yylval = atoi(yytext); return INT; }
[+\-*/] { return yytext[0]; }
[()]    { return yytext[0]; }
.       { printf("Unknown character: \"%s\"", yytext); }

%%
