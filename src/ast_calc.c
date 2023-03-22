#include "ast_calc.h"
#include "ast.h"

#define EVAL(value) YYSTYPE *res = yyget_extra (scanner); *res = value;
#define EVAL_NUM(node)        node_value_create (*((calc_value_t *) &node));
#define EVAL_ADD(left, right) node_op_create (NT_PLUS, left, right)
#define EVAL_SUB(left, right) node_op_create (NT_MINUS, left, right)
#define EVAL_MUL(left, right) node_op_create (NT_MUL, left, right)
#define EVAL_DIV(left, right) node_op_create (NT_DIV, left, right)
#define EVAL_NEG(value)       node_op_create (NT_NEG, value, NULL)

#define yyparse ast_parse
#include "parser.c"

abstract_calc_t *
init_ast_calc (config_t *config)
{
    ast_calc_t *calc = (ast_calc_t *) malloc (sizeof (*calc));
    yyscan_t scanner = NULL;

    calc->base.expr = config->expr;
    calc->base.run = run_ast_calc;
    calc->base.destroy = destroy_ast_calc;

    if (yylex_init_extra (&calc->ast, &scanner)) {
        fprintf (stderr, "error: cannot initialize scanner: %s\n",
                 strerror (errno));
        return (NULL);
    }

    if (yy_scan_string (calc->base.expr, scanner) == NULL) {
        fprintf (stderr, "error: cannot scan given string\n");
        return (NULL);
    }

    if (ast_parse (scanner)) {
        fprintf (stderr, "error: cannot parse string\n");
        return (NULL);
    }

    yylex_destroy (scanner);

    return ((abstract_calc_t *) calc);
}

int
run_ast_calc (abstract_calc_t *calc)
{
    ast_calc_t *ast_calc = (ast_calc_t *) calc;

    ast_calc->base.result = ast_eval (ast_calc->ast);

    return (EXIT_SUCCESS);
}

void
destroy_ast_calc (abstract_calc_t *calc)
{
    ast_calc_t *ast_calc = (ast_calc_t *) calc;

    if (ast_calc->ast) {
        ast_free (ast_calc->ast);
    }

    free (ast_calc);
}
