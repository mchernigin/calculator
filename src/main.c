#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "abstract_calc.h"
#include "basic_calc.h"
#include "ast_calc.h"

#define USAGE()                                                                \
    printf ("usage: %s [-h] [-p PARSER] [-t] [-n NUM] expression\n\n"          \
            "optional arguments:\n"                                            \
            "  -h,        show this help message and exit\n"                   \
            "  -p PARSER, basic (default), ast_rec, ast_iter\n"                \
            "  -t,        print calculation time to stderr\n"                  \
            "  -n NUM,    number of calculations\n", argv[0]);

typedef struct config_t {
    char *expr;
    unsigned long iteration_number;
    calc_funcs_t *calc_funcs;
    bool print_time;
} config_t;

int
parse_args (config_t *config, int argc, char *argv[])
{
    opterr = 0; // Suppress getopt error printing
    int opt;
    while (-1 != (opt = getopt (argc, argv, "hp:n:t"))) {
        switch (opt) {
        case 'h':
            USAGE ();
            return (EXIT_FAILURE);
        case 'p':
            if (strcmp (optarg, "basic") == 0) {
                config->calc_funcs = &calc_basic_funcs;
            } else if (strcmp (optarg, "ast_rec") == 0) {
                config->calc_funcs = &calc_ast_rec_funcs;
            } else if (strcmp (optarg, "ast_iter") == 0) {
                config->calc_funcs = &calc_ast_iter_funcs;
            } else {
                fprintf (stderr, "error: unexpected parser\n");
                USAGE ();
                return (EXIT_FAILURE);
            }
            break;
        case 'n':
        {
            char *end;
            config->iteration_number = strtoul (optarg, &end, 10);
            if ('\0' != *end) {
                fprintf (stderr, "error: n is not an integer\n");
                USAGE ();
                return (EXIT_FAILURE);
            }
            break;
        }
        case 't':
            config->print_time = true;
            break;
        case '?':
        default:
            fprintf (stderr, "error: unexpected flag\n");
            USAGE ();
            return (EXIT_FAILURE);
        }
    }

    if (NULL == config->calc_funcs) {
        config->calc_funcs = &calc_basic_funcs;
    }

    config->expr = argv[optind];
    if (NULL == config->expr) {
        fprintf (stderr, "error: no expression was provided\n");
        USAGE ();
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}

int
main (int argc, char *argv[])
{
    config_t config = {
        .iteration_number = 1,
        .calc_funcs = &calc_basic_funcs,
        .print_time = false,
    };
    if (0 != parse_args (&config, argc, argv)) {
        return (EXIT_FAILURE);
    }

    abstract_calc_t *calc = config.calc_funcs->init (config.expr);
    calc->funcs = config.calc_funcs;

    int exit_code = EXIT_SUCCESS;

    clock_t begin = clock ();

    for (size_t i = 0; i < config.iteration_number; ++i) {
        if (0 != run_calc (calc)) {
            exit_code = EXIT_FAILURE;
            goto destroy_calc;
        }
    }

    clock_t end = clock ();
    double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;

    printf ("%g\n", calc->result);
    if (config.print_time) {
        fprintf (stderr, "%lf", time_spent);
    }

destroy_calc:
    destroy_calc (calc);

    return (exit_code);
}

