#include "abstract_calc.h"

#define YYSTYPE calc_value_t
#include "lexer.h"

typedef struct basic_calc_extra_t {
    calc_value_t result;
    x_t x;
} basic_calc_extra_t;

#define EVAL_RESULT(VALUE) {                                                   \
    basic_calc_extra_t *extra = yyget_extra (scanner);                         \
    extra->result = VALUE;                                                     \
}

#define EVAL_X(LHS) {                                                          \
    basic_calc_extra_t *extra = yyget_extra (scanner);                         \
    if (extra->x.specified) {                                                 \
        LHS = extra->x.value;                                                  \
    } else {                                                                   \
        yyerror (scanner, "x was not specified");                              \
    }                                                                          \
}

#define EVAL_NUM(LHS, VALUE)       LHS = VALUE
#define EVAL_ADD(LHS, LEFT, RIGHT) LHS = LEFT + RIGHT
#define EVAL_SUB(LHS, LEFT, RIGHT) LHS = LEFT - RIGHT
#define EVAL_MUL(LHS, LEFT, RIGHT) LHS = LEFT * RIGHT
#define EVAL_DIV(LHS, LEFT, RIGHT) LHS = LEFT / RIGHT
#define EVAL_NEG(LHS, VALUE)       LHS = -VALUE

#define yyparse basic_parse
#include "parser.c"

typedef struct basic_calc_t {
    abstract_calc_t base;
    yyscan_t scanner;
    basic_calc_extra_t extra;
} basic_calc_t;

static int
basic_calc_run (abstract_calc_t *calc, x_t *x, calc_value_t *result)
{
    basic_calc_t *basic_calc = (basic_calc_t *) calc;

    if (NULL == yy_scan_string (basic_calc->base.expr, basic_calc->scanner)) {
        perror ("error: cannot set string to parse");
        return (EXIT_FAILURE);
    }

    basic_calc->extra.x = *x;

    if (0 != basic_parse (basic_calc->scanner)) {
        fprintf (stderr, "error: cannot parse given string\n");
        return (EXIT_FAILURE);
    }

    *result = basic_calc->extra.result;

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

    if (0 != yylex_init_extra (&calc->extra, &calc->scanner)) {
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
