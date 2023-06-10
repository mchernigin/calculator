#include <stdio.h>
#include "abstract_calc.h"
#include "basic_calc.h"

#define EVAL_RESULT(VALUE) {                                                   \
    calc_value_t *res = yyget_extra (scanner);                                 \
    *res = VALUE;                                                              \
}

#define EVAL_NUM(LHS, VALUE)       LHS = VALUE
#define EVAL_ADD(LHS, LEFT, RIGHT) LHS = LEFT + RIGHT
#define EVAL_SUB(LHS, LEFT, RIGHT) LHS = LEFT - RIGHT
#define EVAL_MUL(LHS, LEFT, RIGHT) LHS = LEFT * RIGHT
#define EVAL_DIV(LHS, LEFT, RIGHT) LHS = LEFT / RIGHT
#define EVAL_NEG(LHS, VALUE)       LHS = -VALUE

#define YYSTYPE calc_value_t
#define yyparse basic_parse
#include "parser.c"

typedef struct basic_calc_t {
    abstract_calc_t base;
    yyscan_t scanner;
} basic_calc_t ;

static int
basic_calc_run (abstract_calc_t *calc)
{
    basic_calc_t *basic_calc = (basic_calc_t *) calc;

    if (NULL == yy_scan_string (basic_calc->base.expr, basic_calc->scanner)) {
        perror ("error: cannot set string to parse");
        return (EXIT_FAILURE);
    }

    if (0 != basic_parse (basic_calc->scanner)) {
        fprintf (stderr, "error: cannot parse given string\n");
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}

static void
basic_calc_destroy (abstract_calc_t *calc)
{
    basic_calc_t *basic_calc = (basic_calc_t *) calc;
    yylex_destroy (basic_calc->scanner);
    free (basic_calc);
}

static abstract_calc_t *
basic_calc_init (char *expr)
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

    return (&calc->base);
}

calc_funcs_t calc_basic_funcs = {
    .init = basic_calc_init,
    .run = basic_calc_run,
    .destroy = basic_calc_destroy
};
