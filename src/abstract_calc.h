#ifndef ABSTRACT_CALC_H
#define ABSTRACT_CALC_H

#include "ast.h"
#include "config.h"
#include "lexer.h"

typedef struct abstract_calc_t {
    char *expr;
    calc_value_t result;

    int (*run) (struct abstract_calc_t *calc);
    void (*destroy) (struct abstract_calc_t *calc);
} abstract_calc_t;

#endif // ABSTRACT_CALC_H
