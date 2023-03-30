#include "basic_calc.h"
#include "parser.h"

#define EVAL(value) YYSTYPE *res = yyget_extra (scanner); *res = value;
#define EVAL_NUM(value)       (value)
#define EVAL_ADD(left, right) (left + right)
#define EVAL_SUB(left, right) (left - right)
#define EVAL_MUL(left, right) (left * right)
#define EVAL_DIV(left, right) (left / right)
#define EVAL_NEG(value)       (-value)

#define YYSTYPE calc_value_t
#define yyparse basic_parse
#include "parser.c"

typedef struct basic_calc_t {
    abstract_calc_t base;
    yyscan_t scanner;
} basic_calc_t ;

int
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

void
basic_calc_destroy (abstract_calc_t *calc)
{
    basic_calc_t *basic_calc = (basic_calc_t *) calc;
    yylex_destroy (basic_calc->scanner);
    free (basic_calc);
}

abstract_calc_t *
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
    calc->base.run = basic_calc_run;
    calc->base.destroy = basic_calc_destroy;

    return (&calc->base);
}

