#ifndef AST_H
#define AST_H

#include <stdlib.h>
#include "config.h"

typedef enum {
    NT_NUM,
    NT_PLUS,
    NT_MINUS,
    NT_MUL,
    NT_DIV,
    NT_NEG,
} node_type_t;

typedef struct ast_node_t {
    node_type_t node_type;
    union {
        calc_value_t value;
        struct {
            struct ast_node_t *left;
            struct ast_node_t *right;
        };
    };
} ast_node_t;

ast_node_t *node_value_create (calc_value_t);
ast_node_t *node_op_create (node_type_t, ast_node_t *, ast_node_t *);
calc_value_t ast_eval (ast_node_t *);
void ast_free (ast_node_t *);

#endif // AST_H
