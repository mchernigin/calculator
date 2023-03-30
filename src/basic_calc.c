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

int
run_basic_calc (abstract_calc_t *calc)
{
    basic_calc_t *basic_calc = (basic_calc_t *) calc;

    if (NULL == yy_scan_string (basic_calc->base.expr, basic_calc->scanner)) {
        perror ("error: cannot set string to parse");
        return (EXIT_FAILURE);
    }

    if (0 != basic_parse (basic_calc->scanner)) {
        perror ("error: cannot parse string");
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}

void
destroy_basic_calc (abstract_calc_t *calc)
{
    basic_calc_t *basic_calc = (basic_calc_t *) calc;
    yylex_destroy (basic_calc->scanner);
    free (basic_calc);
}

abstract_calc_t *
init_basic_calc (char *expr)
{
    basic_calc_t *calc = (basic_calc_t *) malloc (sizeof (*calc));

    if (NULL == calc) {
        perror ("error: cannot create calculator");
        return (NULL);
    }

    if (0 != yylex_init_extra (&calc->base.result, &calc->scanner)) {
        perror ("error: cannot initialize scanner");
        free (calc);
        return (NULL);
    }

    calc->base.expr = expr;
    calc->base.run = run_basic_calc;
    calc->base.destroy = destroy_basic_calc;

    return (&calc->base);
}

