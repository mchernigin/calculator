#ifndef AST_CALC_H
#define AST_CALC_H

#include "abstract_calc.h"
#include "config.h"

int init_ast_calc (config_t *config, abstract_calc_t *calc);
int run_ast_calc (config_t *config, abstract_calc_t *calc);
void destroy_ast_calc (abstract_calc_t *calc);

#endif // AST_CALC_H
