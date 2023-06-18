%{
    #include "lexer.h"

    static void
    yyerror (yyscan_t scanner, const char *msg)
    {
        (void) scanner; // Suppress warning of unused scanner
        fprintf (stderr, "error: %s\n", msg);
    }
%}

%param {void *scanner}
%define api.pure full

%token NUM
%left '+' '-'
%left '*' '/'
%precedence NEG

%%

calclist: exp {
    (void) yynerrs; // Suppress warning of unused variable
    EVAL_RESULT ($1);
}

exp:
  NUM               { EVAL_NUM ($$, $1);     }
| exp '+' exp       { EVAL_ADD ($$, $1, $3); }
| exp '-' exp       { EVAL_SUB ($$, $1, $3); }
| exp '*' exp       { EVAL_MUL ($$, $1, $3); }
| exp '/' exp       { EVAL_DIV ($$, $1, $3); }
| '-' exp %prec NEG { EVAL_NEG ($$, $2);     }
| '(' exp ')'       { $$ = $2;                }

%%
