#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>

#include "config.h"
#include "basic_calc.h"
#undef YYSTYPE
#include "ast_calc.h"

#define USAGE {                                                                \
    printf ("usage: %s [-h] [-pa] [-n NUM] expression\n\n", argv[0]);          \
    printf ("optional arguments:\n");                                          \
    printf ("  -h,     show this help message and exit\n");                    \
    printf ("  -p,     use basic parser mode (default)\n");                    \
    printf ("  -a,     use AST mode\n");                                       \
    printf ("  -n NUM, number of calculations\n");                             \
}

int
parse_args (config_t *config, int argc, char *argv[])
{
    opterr = 0;     // Silence getopt error printing
    int opt;
    while ((opt = getopt (argc, argv, "hpan:")) != -1) {
        switch (opt) {
        case 'h':
            USAGE;
            return (EXIT_FAILURE);
        case 'p':
            config->mode = MODE_PARSER;
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
                USAGE;
                return (EXIT_FAILURE);
            }
            break;
        }
        case '?':
        default:
            fprintf (stderr, "ERROR: unexpected flag\n");
            USAGE;
            return (EXIT_FAILURE);
        }
    }

    config->expr = argv[optind];
    if (!config->expr) {
        fprintf (stderr, "ERROR: no expression was provided\n");
        USAGE;
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}

int
main (int argc, char *argv[])
{
    config_t config = {
        .iteration_number = 1,
        .mode = MODE_PARSER,
    };
    if (parse_args (&config, argc, argv) != 0) {
        return (EXIT_FAILURE);
    }

    clock_t begin = clock ();
    int returned;
    switch (config.mode) {
    case MODE_PARSER:
        returned = run_parser (&config);
        break;
    case MODE_AST:
        returned = run_ast (&config);
        break;
    default:
        fprintf (stderr, "ERROR: Unexpected calculator mode\n");
        return (EXIT_FAILURE);
    }
    clock_t end = clock ();
    double time_spent = (double) (end - begin) / CLOCKS_PER_SEC;

    if (returned != 0) {
        return (EXIT_FAILURE);
    }

    printf ("%Lg\n", config.result);
    fprintf (stderr, "%g\n", time_spent);

    return (EXIT_SUCCESS);
}