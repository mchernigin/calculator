#include <stdbool.h>
#include <stdio.h>
#include <getopt.h>

#include "common.h"
#include "parser.h"

bool
parse_args(config_t *config, int argc, char *argv[])
{
    for (;;) {
        int opt = getopt(argc, argv, "n:pa");
        if (opt == -1) {
            break;
        }

        switch (opt)
        {
        case 'p':
            config->mode = MODE_PARSER;
            break;
        case 'a':
            config->mode = MODE_AST;
            break;
        case 'n':
        {
            char *end;
            config->iteration_number = strtoul(optarg, &end, 10);
            if (*end != '\0') {
                fprintf(stderr, "n must be an integer number\n");
                return (false);
            }
            break;
        }
        case '?':
            return (false);
        }
    }

    config->expr = argv[optind];
    return (true);
}

int
main(int argc, char *argv[])
{
    config_t config = {
        .iteration_number = 1,
        .expr = NULL,
    };

    if (!parse_args(&config, argc, argv)) {
        return (EXIT_FAILURE);
    }

    yy_scan_string(config.expr);
    yyparse();

    return (EXIT_SUCCESS);
}