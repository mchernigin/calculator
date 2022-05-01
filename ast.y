%code top {
  #include <stdio.h>
  #include "ast.h"
  #include "ast.lex.h"

  void
  asterror (__attribute__((unused)) yyscan_t scanner, const char *msg)
  {
    fprintf (stderr, "ERROR: %s\n", msg);
  }
}

%param {void *scanner}
%define api.token.prefix {AST}
%define api.prefix {ast}
%define api.pure full

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

calclist: exp { long double *res = astget_extra (scanner); *res = ast_eval ($1); ast_free ($1); }

exp:
  NUM               { $$ = numval_create ($1);       }
| exp '+' exp       { $$ = ast_create ('+', $1, $3); }
| exp '-' exp       { $$ = ast_create ('-', $1, $3); }
| exp '*' exp       { $$ = ast_create ('*', $1, $3); }
| exp '/' exp       { $$ = ast_create ('/', $1, $3); }
| '-' exp %prec NEG { $$ = ast_create ('N', $2, NULL); }
| '(' exp ')'       { $$ = $2;                       }
;

%%
