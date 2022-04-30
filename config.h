#ifndef CONFIG_H
#define CONFIG_H

#include <stdlib.h>

typedef enum {
    MODE_PARSER,
    MODE_AST,
} calc_mode_t;

typedef struct config_t {
    char *expr;
    calc_mode_t mode;
    size_t iteration_number;
} config_t;

#endif // CONFIG_H