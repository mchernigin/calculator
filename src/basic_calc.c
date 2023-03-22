#include "basic_calc.h"
#undef YYSTYPE
#define YYSTYPE calc_value_t
#include "parser.h"

#define EVAL(value) YYSTYPE *res = yyget_extra (scanner); *res = value;
#define EVAL_NUM(value)       (value)
#define EVAL_ADD(left, right) (left + right)
#define EVAL_SUB(left, right) (left - right)
#define EVAL_MUL(left, right) (left * right)
#define EVAL_DIV(left, right) (left / right)
#define EVAL_NEG(value)       (-value)

#define yyparse basic_parse
#include "parser.c"

abstract_calc_t *
init_basic_calc (config_t *config)
{
    basic_calc_t *calc = (basic_calc_t *) malloc (sizeof (*calc));

    calc->base.expr = config->expr;
    calc->base.run = run_basic_calc;
    calc->base.destroy = destroy_basic_calc;

    if (yylex_init_extra (&calc->base.result, &calc->scanner)) {
        fprintf (stderr, "error: cannot initialize scanner\n");
        return (NULL);
    }

    return ((abstract_calc_t *) calc);
}

int
run_basic_calc (abstract_calc_t *calc)
{
    basic_calc_t *basic_calc = (basic_calc_t *) calc;

    if (yy_scan_string (basic_calc->base.expr, basic_calc->scanner) == NULL) {
        fprintf (stderr, "error: cannot set string to parse\n");
        return (EXIT_FAILURE);
    }

    if (basic_parse (basic_calc->scanner)) {
        fprintf (stderr, "error: cannot parse string\n");
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}

void
destroy_basic_calc (abstract_calc_t *calc)
{
    basic_calc_t *basic_calc = (basic_calc_t *) calc;

    if (basic_calc->scanner) {
        yylex_destroy (basic_calc->scanner);
    }

    free (basic_calc);
}
