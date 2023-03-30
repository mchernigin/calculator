#ifndef AST_H
#define AST_H

#include <stdlib.h>
#include "config.h"

#define YYSTYPE ast_node_t *

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

typedef struct arena_node_t {
    ast_node_t *ast;
    size_t capacity;
    size_t allocated;
} arena_node_t;

ast_node_t *node_value_create (calc_value_t value);
ast_node_t *node_op_create (node_type_t node_type, ast_node_t *left,
                            ast_node_t *right);
ast_node_t *node_op_create (node_type_t node_type, ast_node_t *left,
                            ast_node_t *right);
calc_value_t ast_eval (ast_node_t *ast);
void ast_free (ast_node_t *ast);

#endif // AST_H
