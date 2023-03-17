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
init_ast_calc (config_t *config, abstract_calc_t *calc)
{
    if (yylex_init_extra (&calc->ast, &calc->scanner)) {
        fprintf (stderr, "ERROR: cannot initialize scanner\n");
        return (EXIT_FAILURE);
    }

    if (yy_scan_string (config->expr, calc->scanner) == NULL) {
        fprintf (stderr, "ERROR: cannot scan given string\n");
        return (EXIT_FAILURE);
    }

    if (ast_parse (calc->scanner)) {
        fprintf (stderr, "ERROR: cannot parse string\n");
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}

int
run_ast_calc (config_t *config, abstract_calc_t *calc)
{
    config->result = ast_eval (calc->ast);

    return (EXIT_SUCCESS);
}

void
destroy_ast_calc (abstract_calc_t *calc)
{
    if (calc->ast) {
        ast_free (calc->ast);
    }
    yylex_destroy (calc->scanner);
}
