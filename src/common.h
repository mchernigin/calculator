#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>

typedef enum {
    MODE_PARSER,
    MODE_AST,
} calc_mode_t;

typedef struct config_t {
    calc_mode_t mode;
    size_t iteration_number;
    char *expr;
} config_t;

#endif // COMMON_H