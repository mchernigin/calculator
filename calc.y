%{
#include <stdio.h>

extern int yylex();
extern int yyparse();
extern int yy_scan_string();

void yyerror(const char * msg);
%}

%token INT
%token EOL
%left '+' '-'
%left '*' '/'

%%

calclist:
 | calclist exp EOL { printf ("= %d\n", $2); }
 | calclist exp     { printf ("%d\n", $2); }
 ;

exp:
 | exp '+' exp { $$ = $1 + $3; }
 | exp '-' exp { $$ = $1 - $3; }
 | exp '*' exp { $$ = $1 * $3; }
 | exp '/' exp { $$ = $1 / $3; }
 | '-' exp     { $$ = -$2; }
 | '(' exp ')' { $$ = $2; }
 | INT         { $$ = $1; }
 ;

%%

int main(int argc, char * argv[])
{
    if (argv[1])
        yy_scan_string(argv[1]);
    yyparse();
}

void yyerror(const char * msg)
{
    fprintf(stderr, "ERROR: %s\n", msg);
}