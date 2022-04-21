#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>

typedef struct ast_t {
    int nodetype;
    struct ast_t *left;
    struct ast_t *right;
} ast_t;

typedef struct numval_t {
    int nodetype;
    long double number;
} numval_t;

ast_t *ast_create(int nodetype, ast_t *left, ast_t *right);
ast_t *numval_create(long double value);
long double ast_eval(ast_t *ast);
void ast_free(ast_t *ast);

#define NUM(lvalue, a)    lvalue = numval_create(a);
#define NEG(lvalue, a)    lvalue = ast_create('N', a, NULL);
#define ADD(lvalue, a, b) lvalue = ast_create('+', a, b);
#define SUB(lvalue, a, b) lvalue = ast_create('-', a, b);
#define MUL(lvalue, a, b) lvalue = ast_create('*', a, b);
#define DIV(lvalue, a, b) lvalue = ast_create('/', a, b);
#define EVAL(ast)         { ast_eval(ast); ast_free(ast); }

#endif // AST_H