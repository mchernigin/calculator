#ifndef ABSTRACT_CALC_H
#define ABSTRACT_CALC_H

#include <stdlib.h>

#ifndef calc_value_t
#   define calc_value_t double
#endif

struct calc_funcs_t;

typedef struct abstract_calc_t {
    char *expr;
    calc_value_t result;

    struct calc_funcs_t *funcs;
} abstract_calc_t;

typedef struct calc_funcs_t {
    abstract_calc_t *(*init) (char *);
    int (*run) (abstract_calc_t *);
    void (*destroy) (abstract_calc_t *);
} calc_funcs_t;

static inline int
run_calc (abstract_calc_t *calc)
{
    if (NULL == calc) {
        return (EXIT_FAILURE);
    }

    return (calc->funcs->run (calc));
}

static inline void
destroy_calc (abstract_calc_t *calc)
{
    if (NULL != calc) {
        calc->funcs->destroy (calc);
    }
}

#endif // ABSTRACT_CALC_H
