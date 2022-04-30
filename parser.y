%{
  #define PARSERSTYPE long double
  #define YYSTYPE PARSERSTYPE

  #include <stdio.h>
  #include "parser.lex.h"

  void
  yyerror (__attribute__((unused)) yyscan_t scanner, const char *msg)
  {
    fprintf (stderr, "ERROR: %s\n", msg);
  }

  extern int yylex();
%}

%param {void *scanner}
%define api.prefix {parser}
%define api.pure full

%token NUM
%left '+' '-'
%left '*' '/'
%precedence NEG

%defines

%%

calclist: exp {  }

exp:
  NUM               { $$ = $1;      }
| exp '+' exp       { $$ = $1 + $3; }
| exp '-' exp       { $$ = $1 - $3; }
| exp '*' exp       { $$ = $1 * $3; }
| exp '/' exp       { $$ = $1 / $3; }
| '-' exp %prec NEG { $$ = -$2;     }
| '(' exp ')'       { $$ = $2;      }
;

%%
