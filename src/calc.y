%{
#include <stdio.h>
#include <getopt.h>
#include <stdbool.h>

#define YYSTYPE long double
#include "common.h"

extern int yylex();
extern int yyparse();
extern int yy_scan_string();

void
yyerror(const char *msg)
{
    fprintf(stderr, "ERROR: %s\n", msg);
}

%}

%token NUM
%left '+' '-'
%left '*' '/'
%precedence NEG

%%

calclist: exp { printf("%Lg\n", $1); }

exp:
   NUM
 | exp '+' exp       { $$ = $1 + $3; }
 | exp '-' exp       { $$ = $1 - $3; }
 | exp '*' exp       { $$ = $1 * $3; }
 | exp '/' exp       { $$ = $1 / $3; }
 | '-' exp %prec NEG { $$ = -$2;     }
 | '(' exp ')'       { $$ = $2;      }
 ;

%%
