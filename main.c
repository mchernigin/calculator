#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>

#include "config.h"
#include "basic_calc.h"
#include "ast_calc.h"

#define USAGE() \
    printf ("usage: %s [-h] [-ba] [-t] [-n NUM] expression\n\n" \
            "optional arguments:\n" \
            "  -h,     show this help message and exit\n" \
            "  -b,     use basic parser mode (default)\n" \
            "  -a,     use AST parser mode\n" \
            "  -t,     print calculation time to stderr\n" \
            "  -n NUM, number of calculations\n", argv[0]);

int
parse_args (config_t *config, int argc, char *argv[])
{
    opterr = 0;     // Silence getopt error printing
    int opt;
    while ((opt = getopt (argc, argv, "hban:t")) != -1) {
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
            if (*end != '\0') {
                fprintf (stderr, "ERROR: n is not an integer\n");
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
            fprintf (stderr, "ERROR: unexpected flag\n");
            USAGE ();
            return (EXIT_FAILURE);
        }
    }

    config->expr = argv[optind];
    if (!config->expr) {
        fprintf (stderr, "ERROR: no expression was provided\n");
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
    if (parse_args (&config, argc, argv) != 0) {
        return (EXIT_FAILURE);
    }

    clock_t begin = clock ();

    int returned;
    switch (config.mode) {
    case MODE_BASIC:
        returned = run_basic (&config);
        break;
    case MODE_AST:
        returned = run_ast (&config);
        break;
    default: // MODE_BASIC by default and can only be switched to MODE_AST
        __builtin_unreachable ();
    }

    if (returned != 0) {
        return (EXIT_FAILURE);
    }

    clock_t end = clock ();
    double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;

    printf ("%Lg\n", config.result);
    if (config.print_time) {
        fprintf (stderr, "%g", time_spent);
    }

    return (EXIT_SUCCESS);
}
