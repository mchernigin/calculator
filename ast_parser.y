%{
    #include "ast_calc.h"
    #include "ast_lexer.lex.h"

    void
    asterror (__attribute__ ((unused)) yyscan_t scanner, const char *msg)
    {
        fprintf (stderr, "ERROR: %s\n", msg);
    }
%}

%param {void *scanner}
%define api.token.prefix {AST}
%define api.prefix {ast}
%define api.pure full

%union {
    ast_node_t *ast;
    calc_value_t d;
}

%token <d> NUM
%left '+' '-'
%left '*' '/'
%precedence NEG

%type <ast> exp

%%

calclist: exp {
    ast_node_t **res = astget_extra (scanner);
    *res = $1;
}

exp:
  NUM               { $$ = node_value_create ($1);            }
| exp '+' exp       { $$ = node_op_create (NT_PLUS, $1, $3);  }
| exp '-' exp       { $$ = node_op_create (NT_MINUS, $1, $3); }
| exp '*' exp       { $$ = node_op_create (NT_MUL, $1, $3);   }
| exp '/' exp       { $$ = node_op_create (NT_DIV, $1, $3);   }
| '-' exp %prec NEG { $$ = node_op_create (NT_NEG, $2, NULL); }
| '(' exp ')'       { $$ = $2;                                }
;

%%
