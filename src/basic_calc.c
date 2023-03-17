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
init_basic_calc (config_t *config, abstract_calc_t *calc)
{
    if (yylex_init_extra (&config->result, &calc->scanner)) {
        fprintf (stderr, "error: cannot initialize scanner\n");
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}

int
run_basic_calc (config_t *config, abstract_calc_t *calc)
{
    if (yy_scan_string (config->expr, calc->scanner) == NULL) {
        fprintf (stderr, "error: cannot set string to parse\n");
        return (EXIT_FAILURE);
    }

    if (basic_parse (calc->scanner)) {
        fprintf (stderr, "error: cannot parse string\n");
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}

void
destroy_basic_calc (abstract_calc_t *calc)
{
    if (calc->scanner) {
        yylex_destroy (calc->scanner);
    }
}
