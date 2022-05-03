#ifndef AST_H
#define AST_H

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

#define YYSTYPE ASTSTYPE
#include "ast_parser.tab.h"

ast_node_t *node_op_create (node_type_t node_type, ast_node_t *left, ast_node_t *right);
ast_node_t *node_value_create (calc_value_t value);
calc_value_t ast_eval (ast_node_t *ast);
void ast_free (ast_node_t *ast);
int run_ast (config_t *config);

#endif // AST_H