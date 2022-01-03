%{
#include "calc.tab.h"
#define YY_DECL int yylex()
%}

%%

[ \t]          ;
[0-9]+         { yylval = atoi(yytext); return INT_TOK; }
"+"            { return PLUS_TOK; }
"-"            { return MINUS_TOK; }
"*"            { return MUL_TOK; }
"/"            { return DIV_TOK; }
"|"            { return ABS_TOK; }
\n             { return EOL_TOK; }
"("            { return OPAR_TOK; }
")"            { return CPAR_TOK; }
.              { printf("Mystery character %s\n", yytext); }

%%