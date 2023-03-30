#ifndef ABSTRACT_CALC_H
#define ABSTRACT_CALC_H

#include <stdlib.h>
#include "config.h"

typedef struct abstract_calc_t {
    char *expr;
    calc_value_t result;

    int (*run) (struct abstract_calc_t *);
    void (*destroy) (struct abstract_calc_t *);
} abstract_calc_t;

static inline int
run_calc (abstract_calc_t *calc)
{
    if (NULL == calc) {
        return (EXIT_FAILURE);
    }

    return (calc->run (calc));
}

static inline int
destroy_calc (abstract_calc_t *calc)
{
    if (NULL == calc) {
        return (EXIT_FAILURE);
    }

    calc->destroy (calc);
    return (EXIT_SUCCESS);
}

#endif // ABSTRACT_CALC_H
