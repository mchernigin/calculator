#include "ast.h"
#include "lexer.h"

#define ARENA_INIT_CAPACITY 16
#define ARENA_CAPACITY_GROWTH 2

arena_node_t *
arena_init (void)
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

void
arena_destroy (arena_node_t *arena)
{
    if (NULL != arena->ast) {
        free (arena->ast);
    }

    free (arena);
}

int
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

ast_node_t *
node_op_create (node_type_t node_type, ast_node_t *left, ast_node_t *right)
{
    ast_node_t *node = (ast_node_t *) malloc (sizeof (*node));

    if (NULL == node) {
        perror ("error: cannot create a node");
        return (NULL);
    }

    node->node_type = node_type;
    node->left = left;
    node->right = right;

    return (node);
}

ast_node_t *
node_value_create (calc_value_t value)
{
    ast_node_t *node = (ast_node_t *) malloc (sizeof (*node));

    if (NULL == node) {
        perror ("error: cannot create a node");
        return (NULL);
    }

    node->node_type = NT_NUM;
    node->value = value;

    return (node);
}

calc_value_t
ast_eval (ast_node_t *ast)
{
    switch (ast->node_type) {
    case NT_NUM:   return (ast->value);
    case NT_PLUS:  return (ast_eval (ast->left) + ast_eval (ast->right));
    case NT_MINUS: return (ast_eval (ast->left) - ast_eval (ast->right));
    case NT_MUL:   return (ast_eval (ast->left) * ast_eval (ast->right));
    case NT_DIV:   return (ast_eval (ast->left) / ast_eval (ast->right));
    case NT_NEG:   return (-ast_eval (ast->left));
    default:
        __builtin_unreachable ();
    }
}

void
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

