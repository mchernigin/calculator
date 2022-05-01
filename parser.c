#include "parser.h"

void
run_parser (config_t *config)
{
    yyscan_t scanner = NULL;
    parserlex_init (&scanner);

    for (size_t i = 0; i < config->iteration_number; ++i) {
        parser_scan_string (config->expr, scanner);
        parserparse (scanner);
    }
}
