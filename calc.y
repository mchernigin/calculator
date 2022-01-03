%{
#include <stdio.h>

extern int yylex();
extern int yyparse();

void yyerror(const char * msg);
%}

%token INT_TOK
%token PLUS_TOK MINUS_TOK MUL_TOK DIV_TOK ABS_TOK
%token EOL_TOK
%token OPAR_TOK CPAR_TOK

%%

calclist:
 | calclist exp EOL_TOK { printf("= %d\n", $2); }
 ;

exp: factor
 | exp PLUS_TOK factor { $$ = $1 + $3; }
 | exp MINUS_TOK factor { $$ = $1 - $3; }
 ;

factor: term
 | factor MUL_TOK term { $$ = $1 * $3; }
 | factor DIV_TOK term { $$ = $1 / $3; }
 ;

term: INT_TOK
 | ABS_TOK term ABS_TOK { $$ = $2 >= 0 ? $2 : -$2; }
 | OPAR_TOK exp CPAR_TOK { $$ = $2; }
 ;

%%

int main(int argc, char **argv)
{
    yyparse();
}

void yyerror(const char * msg)
{
    fprintf(stderr, "ERROR: %s\n", msg);
}