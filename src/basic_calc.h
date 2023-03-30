#ifndef BASIC_CALC_H
#define BASIC_CALC_H

#include "abstract_calc.h"

typedef struct basic_calc_t {
    abstract_calc_t base;
    yyscan_t scanner;
} basic_calc_t ;

abstract_calc_t *init_basic_calc (char *);

#endif // BASIC_CALC_H
