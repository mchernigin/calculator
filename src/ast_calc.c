#include <stdio.h>
#include "ast_calc.h"

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
            struct ast_node_t *left;
            struct ast_node_t *right;
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

static arena_node_t *
arena_create (void)
{
    arena_node_t *arena = malloc (sizeof (*arena));
    if (NULL == arena) {
        perror ("error: cannot initialize arena");
        return (NULL);
    }

    arena->capacity = ARENA_INIT_CAPACITY;
    arena->allocated = 0;
    arena->ast = malloc (arena->capacity * sizeof (*arena->ast));
    if (NULL == arena->ast) {
        perror ("error: cannot initialize arena");
        free (arena);
        return (NULL);
    }

    return (arena);
}

static void
arena_destroy (arena_node_t *arena)
{
    if (NULL != arena->ast) {
        free (arena->ast);
    }

    free (arena);
}

static int
arena_allocate (arena_node_t *arena, size_t alloc_size)
{
    size_t new_alloc_size = arena->allocated + alloc_size;

    if (new_alloc_size > arena->capacity) {
        size_t new_capacity = ARENA_CAPACITY_GROWTH * arena->capacity;
        ast_node_t *new_asts =
            realloc (arena->ast, new_capacity * sizeof (*arena->ast));

        if (NULL == arena->ast) {
            perror ("error: cannot reallocate arena");
            return (-1);
        }

        arena->ast = new_asts;
        arena->capacity = new_capacity;
    }

    int idx = arena->allocated;
    arena->allocated = new_alloc_size;
    return (idx);
}

static ast_node_t *
node_op_create (arena_node_t *arena, node_type_t node_type, ast_node_t *left,
                ast_node_t *right)
{
    int index = arena_allocate (arena, 1);
    if (index < 0) {
        return (NULL);
    }
    ast_node_t *node = arena->ast + index;

    node->node_type = node_type;
    node->left = left;
    node->right = right;

    return (node);
}

static ast_node_t *
node_num_create (arena_node_t *arena, calc_value_t value)
{
    int index = arena_allocate (arena, 1);
    if (index < 0) {
        return (NULL);
    }
    ast_node_t *node = arena->ast + index;

    node->node_type = NT_NUM;
    node->value = value;

    return (node);
}

static calc_value_t
ast_eval_rec (ast_node_t *ast)
{
    switch (ast->node_type) {
    case NT_NUM:   return (ast->value);
    case NT_PLUS:  return (ast_eval_rec (ast->left) + ast_eval_rec (ast->right));
    case NT_MINUS: return (ast_eval_rec (ast->left) - ast_eval_rec (ast->right));
    case NT_MUL:   return (ast_eval_rec (ast->left) * ast_eval_rec (ast->right));
    case NT_DIV:   return (ast_eval_rec (ast->left) / ast_eval_rec (ast->right));
    case NT_NEG:   return (-ast_eval_rec (ast->left));
    default:
        __builtin_unreachable ();
    }
}

#if 0
static void
ast_free (ast_node_t *ast)
{
    switch (ast->node_type) {
    case NT_PLUS:
    case NT_MINUS:
    case NT_MUL:
    case NT_DIV:
        ast_free (ast->right);
        __attribute__ ((fallthrough));
    case NT_NEG:
        ast_free (ast->left);
        __attribute__ ((fallthrough));
    case NT_NUM:
        free (ast);
        break;
    default:
        __builtin_unreachable ();
    }
}
#endif

#define EVAL_RESULT(VALUE) {                                                   \
    arena_node_t *arena = yyget_extra (scanner);                               \
    int index = arena_allocate (arena, 1);                                     \
    if (index >= 0) {                                                          \
        arena->ast[index] = *VALUE;                                            \
    }                                                                          \
}

#define EVAL_NUM(LHS, NODE) {                                                  \
    arena_node_t *arena = yyget_extra (scanner);                               \
    LHS = node_num_create (arena, *(calc_value_t *) &NODE);                    \
}

#define EVAL_ADD(LHS, LEFT, RIGHT) {                                           \
    arena_node_t *arena = yyget_extra (scanner);                               \
    LHS = node_op_create (arena, NT_PLUS, LEFT, RIGHT);                        \
}

#define EVAL_SUB(LHS, LEFT, RIGHT) {                                           \
    arena_node_t *arena = yyget_extra (scanner);                               \
    LHS = node_op_create (arena, NT_MINUS, LEFT, RIGHT);                       \
}

#define EVAL_MUL(LHS, LEFT, RIGHT) {                                           \
    arena_node_t *arena = yyget_extra (scanner);                               \
    LHS = node_op_create (arena, NT_MUL, LEFT, RIGHT);                         \
}

#define EVAL_DIV(LHS, LEFT, RIGHT) {                                           \
    arena_node_t *arena = yyget_extra (scanner);                               \
    LHS = node_op_create (arena, NT_DIV, LEFT, RIGHT);                         \
}

#define EVAL_NEG(LHS, VALUE) {                                                 \
    arena_node_t *arena = yyget_extra (scanner);                               \
    LHS = node_op_create (arena, NT_NEG, VALUE, NULL);                         \
}

#define YYSTYPE ast_node_t *
#define yyparse ast_parse
#include "parser.c"

typedef struct ast_calc_t {
    abstract_calc_t base;
    arena_node_t *arena;
} ast_calc_t;

int
ast_calc_run (abstract_calc_t *calc)
{
    ast_calc_t *ast_calc = (ast_calc_t *) calc;
    ast_calc->base.result =
        ast_eval_rec (&ast_calc->arena->ast[ast_calc->arena->allocated - 1]);
    return (EXIT_SUCCESS);
}

void
ast_calc_destroy (abstract_calc_t *calc)
{
    ast_calc_t *ast_calc = (ast_calc_t *) calc;
    arena_destroy (ast_calc->arena);
    free (ast_calc);
}

abstract_calc_t *
ast_calc_init (char *expr)
{
    ast_calc_t *calc = (ast_calc_t *) malloc (sizeof (*calc));
    if (NULL == calc) {
        perror ("error: cannot create calculator");
        return (NULL);
    }

    calc->arena = arena_create();
    if (NULL == calc->arena) {
        return (NULL);
    }

    yyscan_t scanner = NULL;

    if (0 != yylex_init_extra (calc->arena, &scanner)) {
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
    calc->base.run = ast_calc_run;
    calc->base.destroy = ast_calc_destroy;

    yylex_destroy (scanner);

    return (&calc->base);

fail_free_scanner:
    yylex_destroy (scanner);

fail_free_calc:
    free (calc);

    return (NULL);
}

