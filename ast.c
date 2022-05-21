#include "ast.h"
#include "lexer.h"

ast_node_t *
node_op_create (node_type_t node_type, ast_node_t *left, ast_node_t *right)
{
    ast_node_t *node = malloc (sizeof (*node));

    if (!node) {
        fprintf (stderr, "ERROR: cannot create a node: not enough memory\n");
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
    ast_node_t *node = malloc (sizeof (*node));

    if (!node) {
        fprintf (stderr, "ERROR: cannot create a node: not enough memory\n");
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
        fprintf (stderr, "ERROR: unknown ast node type %d\n", ast->node_type);
    }

    return (0);
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
        __attribute__ ((fallthrough)); // Intend to fall through
    case NT_NEG:
        ast_free (ast->left);
        __attribute__ ((fallthrough)); // Intend to fall through
    case NT_NUM:
        free (ast);
        break;
    default:
        fprintf (stderr, "ERROR: unknown ast node type\n");
    }
}
