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
    ast_free (ast_calc->ast);
    free (ast_calc);
}

abstract_calc_t *
init_ast_calc (char *expr)
{
    ast_calc_t *calc = (ast_calc_t *) malloc (sizeof (*calc));

    if (NULL == calc) {
        perror ("error: cannot create calculator");
        return (NULL);
    }

    yyscan_t scanner = NULL;

    if (0 != yylex_init_extra (&calc->ast, &scanner)) {
        perror ("error: cannot initialize scanner");
        goto fail_free_calc;
    }

    if (NULL == yy_scan_string (expr, scanner)) {
        perror ("error: cannot scan given string");
        goto fail_free_scanner;
    }

    if (0 != ast_parse (scanner)) {
        perror ("error: cannot parse given string");
        goto fail_free_scanner;
    }

    calc->base.expr = expr;
    calc->base.run = run_ast_calc;
    calc->base.destroy = destroy_ast_calc;

    yylex_destroy (scanner);

    return (&calc->base);

fail_free_scanner:
    yylex_destroy (scanner);

fail_free_calc:
    free (calc);

    return (NULL);
}

