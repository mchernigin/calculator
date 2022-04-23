#include <stdbool.h>
#include <stdio.h>
#include <getopt.h>
#include <assert.h>
#include <unistd.h>

#include "ast.h"
#include "calc.tab.h"
#include "calc.lex.h"

typedef enum {
    MODE_PARSER,
    MODE_AST,
} calc_mode_t;

typedef struct config_t {
    char *expr;
    calc_mode_t mode;
    size_t iteration_number;
} config_t;

bool
parse_args (config_t *config, int argc, char *argv[])
{
    opterr = 0;     // Silence getopt error printing
    int opt;
    while ((opt = getopt(argc, argv, "n:pa")) != -1) {
        switch (opt) {
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
                return (false);
            }
            break;
        }
        case '?':
        default:
            fprintf (stderr, "ERROR: Unexpected flag\n");
            return (false);
        }
    }

    config->expr = argv[optind];
    if (!config->expr) {
        fprintf (stderr, "ERROR: no expression was provided\n");
        return (false);
    }

    return (true);
}

void
run_parser (config_t *config)
{
    puts("PARSER");
    for (size_t i = 0; i < config->iteration_number; ++i) {
        yy_scan_string (config->expr);
        yyparse ();
    }
}

void
run_ast (config_t *config)
{
    puts("AST");
    for (size_t i = 0; i < config->iteration_number; ++i) {
        yy_scan_string (config->expr);
        yyparse ();
    }
}

int
main (int argc, char *argv[])
{
    config_t config = {
        .iteration_number = 1,
        .mode = MODE_PARSER,
    };
    if (!parse_args (&config, argc, argv)) {
        return (EXIT_FAILURE);
    }

    switch (config.mode) {
    case MODE_PARSER:
        run_parser (&config);
        break;
    case MODE_AST:
        run_ast (&config);
        break;
    default:
        fprintf (stderr, "ERROR: Unexpected calculator mode\n");
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}