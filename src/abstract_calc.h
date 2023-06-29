#ifndef ABSTRACT_CALC_H
#define ABSTRACT_CALC_H

#include <stdlib.h>
#include <stdbool.h>

#ifndef calc_value_t
#   define calc_value_t double
#endif

typedef struct x_t {
    bool specified;
    calc_value_t value;
} x_t;

struct calc_funcs_t;

typedef struct abstract_calc_t {
    char *expr;

    struct calc_funcs_t *funcs;
} abstract_calc_t;

typedef struct calc_funcs_t {
    abstract_calc_t *(*init) (char *);
    int (*run) (abstract_calc_t *, x_t *x, calc_value_t *);
    void (*destroy) (abstract_calc_t *);
} calc_funcs_t;

static inline int
run_calc (abstract_calc_t *calc, x_t *x, calc_value_t *result)
{
    if (NULL == calc) {
        return (EXIT_FAILURE);
    }

    return (calc->funcs->run (calc, x, result));
}

static inline void
destroy_calc (abstract_calc_t *calc)
{
    if (NULL != calc) {
        calc->funcs->destroy (calc);
    }
}

#endif // ABSTRACT_CALC_H
