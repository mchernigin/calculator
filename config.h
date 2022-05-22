#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

#ifndef calc_value_t
#   define calc_value_t long double
#endif

typedef enum {
    MODE_BASIC,
    MODE_AST,
} calc_mode_t;

typedef struct config_t {
    char *expr;
    calc_value_t result;
    calc_mode_t mode;
    unsigned long iteration_number;
    bool print_time;
} config_t;

#endif // CONFIG_H
