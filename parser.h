#include "config.h"
#define PARSERSTYPE long double
#define YYSTYPE PARSERSTYPE
#include "parser.tab.h"
#include "parser.lex.h"

void run_parser (config_t *config);