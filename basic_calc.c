#include "config.h"
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
run_basic (config_t *config)
{
    yyscan_t scanner = NULL;
    if (yylex_init_extra (&config->result, &scanner)) {
        fprintf (stderr, "ERROR: cannot initialize scanner\n");
        return (EXIT_FAILURE);
    }

    int return_value = EXIT_SUCCESS;

    for (size_t i = 0; i < config->iteration_number; ++i) {
        if (yy_scan_string (config->expr, scanner) == NULL) {
            fprintf (stderr, "ERROR: cannot set string to parse\n");
            return_value = EXIT_FAILURE;
            goto free_scanner;
        }

        if (basic_parse (scanner)) {
            fprintf (stderr, "ERROR: cannot parse string\n");
            return_value = EXIT_FAILURE;
            goto free_scanner;
        }
    }

free_scanner:
    yylex_destroy (scanner);

    return (return_value);
}
