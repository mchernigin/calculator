%{
#include <stdio.h>
#include "ast.h"
#include "calc.lex.h"

void
yyerror (const char *msg)
{
  fprintf (stderr, "ERROR: %s\n", msg);
}

%}

%union {
  ast_t *ast;
  long double d;
}

%token <d> NUM
%left '+' '-'
%left '*' '/'
%precedence NEG
%type <ast> exp

%%

calclist: exp { EVAL($1); }

exp:
  NUM               { NUM($$, $1);     }
| exp '+' exp       { ADD($$, $1, $3); }
| exp '-' exp       { SUB($$, $1, $3); }
| exp '*' exp       { MUL($$, $1, $3); }
| exp '/' exp       { DIV($$, $1, $3); }
| '-' exp %prec NEG { NEG($$, $2);     }
| '(' exp ')'       { $$ = $2;         }
;

%%
