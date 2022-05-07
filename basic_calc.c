#include "basic_calc.h"
#include "basic_lexer.h"

int
run_basic (config_t *config)
{
    yyscan_t scanner = NULL;

    if (basiclex_init_extra (&config->result, &scanner)) {
        fprintf (stderr, "ERROR: cannot initialize scanner\n");
        return (EXIT_FAILURE);
    }

    for (size_t i = 0; i < config->iteration_number; ++i) {
        if (basic_scan_string (config->expr, scanner) == NULL) {
            fprintf (stderr, "ERROR: cannot set string to parse\n");
            return (EXIT_FAILURE);
        }

        if (basicparse (scanner)) {
            fprintf (stderr, "ERROR: cannot parse string\n");
            return (EXIT_FAILURE);
        }
        
    }

    return (EXIT_SUCCESS);
}
