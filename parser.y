%{
    #include "ast.h"
    #include "parser.h"
    #include "lexer.h"

    static void
    yyerror (__attribute__ ((unused)) yyscan_t scanner, const char *msg)
    {
        fprintf (stderr, "ERROR: %s\n", msg);
    }
%}

%param {void *scanner}
%define api.pure full

%token NUM
%left '+' '-'
%left '*' '/'
%precedence NEG

%%

calclist: exp { EVAL($1) }

exp:
  NUM               { $$ = EVAL_NUM ($1);     }
| exp '+' exp       { $$ = EVAL_ADD ($1, $3); }
| exp '-' exp       { $$ = EVAL_SUB ($1, $3); }
| exp '*' exp       { $$ = EVAL_MUL ($1, $3); }
| exp '/' exp       { $$ = EVAL_DIV ($1, $3); }
| '-' exp %prec NEG { $$ = EVAL_NEG ($2);     }
| '(' exp ')'       { $$ = $2;                }

%%
