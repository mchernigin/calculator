#ifndef AST_H
#define AST_H

#include "config.h"

typedef struct ast_t {
    int nodetype;
    struct ast_t *left;
    struct ast_t *right;
} ast_t;

typedef struct numval_t {
    int nodetype;
    calc_value_t number;
} numval_t;

#define YYSTYPE ASTSTYPE
#include "ast.tab.h"

ast_t *ast_create (int nodetype, ast_t *left, ast_t *right);
ast_t *numval_create (calc_value_t value);
calc_value_t ast_eval (ast_t *ast);
void ast_free (ast_t *ast);
int run_ast (config_t *config);

#endif // AST_H