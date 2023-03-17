#ifndef ABSTRACT_CALC_H
#define ABSTRACT_CALC_H

#include "ast.h"
#include "lexer.h"

typedef struct abstract_calc_t {
    yyscan_t scanner;
    ast_node_t *ast;

    int (*init) (config_t *, struct abstract_calc_t *calc);
    int (*run) (config_t *, struct abstract_calc_t *calc);
    void (*destroy) (struct abstract_calc_t *calc);
} abstract_calc_t;

#endif // ABSTRACT_CALC_H
