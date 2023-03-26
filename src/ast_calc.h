#ifndef AST_CALC_H
#define AST_CALC_H

#include "abstract_calc.h"

typedef struct ast_calc_t {
    abstract_calc_t base;
    ast_node_t *ast;
} ast_calc_t;

abstract_calc_t *init_ast_calc (char *expr);

#endif // AST_CALC_H
