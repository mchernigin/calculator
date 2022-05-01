#ifndef PARSER_H
#define PARSER_H

#include "config.h"
#include "parser.tab.h"

#define PARSERSTYPE calc_value_t
#define YYSTYPE PARSERSTYPE

int run_parser (config_t *config);

#endif // PARSER_H