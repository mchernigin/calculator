#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>

#include "config.h"
#include "abstract_calc.h"
#include "basic_calc.h"
#include "ast_calc.h"

#define USAGE()                                                                \
    printf ("usage: %s [-h] [-ba] [-t] [-n NUM] expression\n\n"                \
            "optional arguments:\n"                                            \
            "  -h,     show this help message and exit\n"                      \
            "  -b,     use basic parser mode (default)\n"                      \
            "  -a,     use AST parser mode\n"                                  \
            "  -t,     print calculation time to stderr\n"                     \
            "  -n NUM, number of calculations\n", argv[0]);

int
parse_args (config_t *config, int argc, char *argv[])
{
    opterr = 0; // Suppress getopt error printing
    int opt;
    while (-1 != (opt = getopt (argc, argv, "hban:t"))) {
        switch (opt) {
        case 'h':
            USAGE ();
            return (EXIT_FAILURE);
        case 'b':
            config->mode = MODE_BASIC;
            break;
        case 'a':
            config->mode = MODE_AST;
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
        .mode = MODE_BASIC,
        .print_time = false,
    };
    if (0 != parse_args (&config, argc, argv)) {
        return (EXIT_FAILURE);
    }

    abstract_calc_t *calc;

    switch (config.mode) {
    case MODE_BASIC:
        calc = basic_calc_init (config.expr);
        break;
    case MODE_AST:
        calc = ast_calc_init (config.expr);
        break;
    default: // MODE_BASIC by default and can only be switched to MODE_AST
        __builtin_unreachable ();
    }

    if (NULL == calc) {
        return (EXIT_FAILURE);
    }

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

