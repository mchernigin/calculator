#ifndef CONFIG_H
#define CONFIG_H

typedef long double calc_value_t;

typedef enum {
    MODE_PARSER,
    MODE_AST,
} calc_mode_t;

typedef struct config_t {
    char *expr;
    calc_value_t result;
    calc_mode_t mode;
    unsigned long iteration_number;
} config_t;

#endif // CONFIG_H