#include "parser.h"

void
run_parser (config_t *config)
{
    puts("PARSER");
    yyscan_t scanner = NULL;

    for (size_t i = 0; i < config->iteration_number; ++i) {
        yy_scan_string (config->expr, scanner);
        parserparse (scanner);
    }
}
