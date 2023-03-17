#ifndef BASIC_CALC_H
#define BASIC_CALC_H

#include "abstract_calc.h"
#include "config.h"

int init_basic_calc (config_t *config, abstract_calc_t *calc);
int run_basic_calc (config_t *config, abstract_calc_t *calc);
void destroy_basic_calc (abstract_calc_t *calc);

#endif // BASIC_CALC_H
