#include <stdio.h>
#include <stdlib.h>
#include "abstract_calc.h"

typedef enum {
    NT_NUM,
    NT_PLUS,
    NT_MINUS,
    NT_MUL,
    NT_DIV,
    NT_NEG,
} node_type_t;

typedef struct ast_node_t {
    union {
        calc_value_t value;
        struct {
            int left;
            int right;
        };
    };
    node_type_t node_type;
} ast_node_t;

typedef struct arena_node_t {
    ast_node_t *ast;
    size_t capacity;
    size_t allocated;
} arena_node_t;

#define ARENA_INIT_CAPACITY 16
#define ARENA_CAPACITY_GROWTH 2

static int
arena_init (arena_node_t *arena)
{
    arena->capacity = ARENA_INIT_CAPACITY;
    arena->allocated = 0;
    arena->ast = malloc (arena->capacity * sizeof (*arena->ast));
    if (NULL == arena->ast) {
        perror ("error: cannot initialize arena");
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}

static void
arena_destroy (arena_node_t *arena)
{
    if (NULL != arena->ast) {
        free (arena->ast);
    }

    arena->ast = NULL;
}

static int
arena_allocate (arena_node_t *arena, size_t alloc_size)
{
    size_t new_alloc_size = arena->allocated + alloc_size;

    if (new_alloc_size > arena->capacity) {
        size_t new_capacity = ARENA_CAPACITY_GROWTH * new_alloc_size;
        ast_node_t *new_ast =
            realloc (arena->ast, new_capacity * sizeof (*arena->ast));

        if (NULL == arena->ast) {
            perror ("error: cannot reallocate arena");
            return (-1);
        }

        arena->ast = new_ast;
        arena->capacity = new_capacity;
    }

    int idx = arena->allocated;
    arena->allocated = new_alloc_size;
    return (idx);
}

static calc_value_t
ast_eval_rec (arena_node_t *arena, int index)
{
    ast_node_t *node = &arena->ast[index];
    switch (node->node_type) {
    case NT_NUM:
        return (node->value);
    case NT_PLUS:
        return (ast_eval_rec (arena, node->left) + ast_eval_rec (arena, node->right));
    case NT_MINUS:
        return (ast_eval_rec (arena, node->left) - ast_eval_rec (arena, node->right));
    case NT_MUL:
        return (ast_eval_rec (arena, node->left) * ast_eval_rec (arena, node->right));
    case NT_DIV:
        return (ast_eval_rec (arena, node->left) / ast_eval_rec (arena, node->right));
    case NT_NEG:
        return (-ast_eval_rec (arena, node->left));
    default:
        __builtin_unreachable ();
    }
}

#define EVAL_RESULT(VALUE) {                                                   \
    arena_node_t *arena = yyget_extra (scanner);                               \
    int index = arena_allocate (arena, 1);                                     \
    if (index >= 0) {                                                          \
        arena->ast[index] = VALUE;                                             \
    }                                                                          \
}

#define EVAL_NUM(LHS, NODE) {                                                  \
    LHS = (ast_node_t) {                                                       \
        .value = NODE.value,                                                   \
        .node_type = NT_NUM                                                    \
    };                                                                         \
}

#define AST_BIN_OP(LHS, NODE_TYPE, LEFT, RIGHT) {                              \
    arena_node_t *arena = yyget_extra (scanner);                               \
    int left = arena_allocate (arena, 2);                                      \
    if (left >= 0) {                                                           \
        arena->ast[left] = LEFT;                                               \
        arena->ast[left + 1] = RIGHT;                                          \
        LHS.left = left;                                                       \
        LHS.right = left + 1;                                                  \
    }                                                                          \
    LHS.node_type = NODE_TYPE;                                                 \
}

#define EVAL_ADD(LHS, LEFT, RIGHT) AST_BIN_OP(LHS, NT_PLUS, LEFT, RIGHT)
#define EVAL_SUB(LHS, LEFT, RIGHT) AST_BIN_OP(LHS, NT_MINUS, LEFT, RIGHT)
#define EVAL_MUL(LHS, LEFT, RIGHT) AST_BIN_OP(LHS, NT_MUL, LEFT, RIGHT)
#define EVAL_DIV(LHS, LEFT, RIGHT) AST_BIN_OP(LHS, NT_DIV, LEFT, RIGHT)
#define EVAL_NEG(LHS, VALUE) AST_BIN_OP(LHS, NT_NEG, VALUE, (ast_node_t) {})

#define YYSTYPE ast_node_t
#define yyparse ast_parse
#include "parser.c"

typedef struct ast_calc_t {
    abstract_calc_t base;
    arena_node_t arena;
} ast_calc_t;

static int
ast_calc_run_rec (abstract_calc_t *calc)
{
    ast_calc_t *ast_calc = (ast_calc_t *) calc;
    ast_calc->base.result =
        ast_eval_rec (&ast_calc->arena, ast_calc->arena.allocated - 1);
    return (EXIT_SUCCESS);
}

static int
ast_calc_run_iter (abstract_calc_t *calc)
{
    ast_calc_t *ast_calc = (ast_calc_t *) calc;
    calc_value_t results[ast_calc->arena.allocated];

    for (size_t i = 0; i < ast_calc->arena.allocated; ++i) {
        ast_node_t *node = &ast_calc->arena.ast[i];
        switch (node->node_type) {
        case NT_NUM:
          results[i] = node->value;
          break;
        case NT_PLUS:
          results[i] = results[node->left] + results[node->right];
          break;
        case NT_MINUS:
          results[i] = results[node->left] - results[node->right];
          break;
        case NT_MUL:
          results[i] = results[node->left] * results[node->right];
          break;
        case NT_DIV:
          results[i] = results[node->left] / results[node->right];
          break;
        case NT_NEG:
          results[i] = -results[node->left];
          break;
        }
    }

    ast_calc->base.result = results[ast_calc->arena.allocated - 1];
    return (EXIT_SUCCESS);
}

static void
ast_calc_destroy (abstract_calc_t *calc)
{
    ast_calc_t *ast_calc = (ast_calc_t *) calc;
    arena_destroy (&ast_calc->arena);
    free (ast_calc);
}

static abstract_calc_t *
ast_calc_init (char *expr)
{
    ast_calc_t *calc = (ast_calc_t *) malloc (sizeof (*calc));
    if (NULL == calc) {
        perror ("error: cannot create calculator");
        return (NULL);
    }

    if (0 != arena_init (&calc->arena)) {
        return (NULL);
    }

    yyscan_t scanner = NULL;

    if (0 != yylex_init_extra (&calc->arena, &scanner)) {
        perror ("error: cannot initialize scanner");
        goto fail_free_calc;
    }

    if (NULL == yy_scan_string (expr, scanner)) {
        perror ("error: cannot scan given string");
        goto fail_free_scanner;
    }

    if (0 != ast_parse (scanner)) {
        fprintf (stderr, "error: cannot parse given string\n");
        goto fail_free_scanner;
    }

    calc->base.expr = expr;

    yylex_destroy (scanner);

    return (&calc->base);

fail_free_scanner:
    yylex_destroy (scanner);

fail_free_calc:
    free (calc);

    return (NULL);
}

calc_funcs_t calc_ast_rec_funcs = {
    .init = ast_calc_init,
    .run = ast_calc_run_rec,
    .destroy = ast_calc_destroy
};

calc_funcs_t calc_ast_iter_funcs = {
    .init = ast_calc_init,
    .run = ast_calc_run_iter,
    .destroy = ast_calc_destroy
};

