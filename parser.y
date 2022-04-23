%{
#include <stdio.h>
#include "calc.lex.h"
#define YYSTYPE long double

void
yyerror (const char *msg)
{
  fprintf (stderr, "ERROR: %s\n", msg);
}

%}

%token NUM
%left '+' '-'
%left '*' '/'
%precedence NEG

%%

calclist: exp {  }

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
