#include <stdbool.h>
#include <stdio.h>
#include <getopt.h>
#include <assert.h>

#include "ast.h"
#include "common.h"
#include "parser.h"

void
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
                fprintf(stderr, "ERROR: n is not an integer\n");
                exit(EXIT_FAILURE);
            }
            break;
        }
        case '?':
        default:
            exit(EXIT_FAILURE);
        }
    }

    config->expr = argv[optind];
    if (!config->expr) {
        fprintf(stderr, "ERROR: no expresion was provided\n");
        exit(EXIT_FAILURE);
    }
}

void
run_parser(config_t *config)
{
    puts("PARSER");
    for (size_t i = 0; i < config->iteration_number; ++i) {
        yy_scan_string(config->expr);
        yyparse();
    }
}

void
run_ast(config_t *config)
{
    puts("AST");
    for (size_t i = 0; i < config->iteration_number; ++i) {
        yy_scan_string(config->expr);
        yyparse();
    }
}

int
main(int argc, char *argv[])
{
    config_t config = {
        .iteration_number = 1,
        .mode = MODE_PARSER,
    };
    parse_args(&config, argc, argv);

    switch (config.mode)
    {
    case MODE_PARSER: 
        run_parser(&config);
        break;
    case MODE_AST:
        run_ast(&config);
        break;
    default:
        fprintf(stderr, "ERROR: Unexpected mode\n");
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}