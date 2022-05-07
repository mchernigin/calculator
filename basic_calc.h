#ifndef BASIC_CALC_H
#define BASIC_CALC_H

#include "config.h"
#include "basic_parser.h"

#define BASICSTYPE calc_value_t
#define YYSTYPE BASICSTYPE

int run_basic (config_t *config);

#endif // BASIC_CALC_H