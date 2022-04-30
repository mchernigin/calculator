#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include "config.h"

typedef struct ast_t {
    int nodetype;
    struct ast_t *left;
    struct ast_t *right;
} ast_t;

typedef struct numval_t {
    int nodetype;
    long double number;
} numval_t;

#define YYSTYPE ASTSTYPE
#include "ast.tab.h"

ast_t *ast_create (int nodetype, ast_t *left, ast_t *right);
ast_t *numval_create (long double value);
long double ast_eval (ast_t *ast);
void ast_free (ast_t *ast);
void run_ast (config_t *config);

#endif // AST_H