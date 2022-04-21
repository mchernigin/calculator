#include "ast.h"

ast_t *
ast_create(int nodetype, ast_t *left, ast_t *right)
{
    ast_t *ast = malloc(sizeof(ast_t));

    if (!ast) {
        fprintf(stderr, "ERROR: Can not create ast: not enough memory\n");
        exit(EXIT_FAILURE);
    }

    ast->nodetype = nodetype;
    ast->left = left;
    ast->right = right;

    return (ast);    
}

ast_t *
numval_create(long double value)
{
    numval_t *numval = malloc(sizeof(numval_t));

    if (!numval) {
        fprintf(stderr, "ERROR: Can not create numval: not enough memory\n");
        exit(EXIT_FAILURE);
    }

    numval->nodetype = 'K';
    numval->number = value;

    return ((ast_t *) numval);    
}

long double
ast_eval(ast_t *ast)
{
    switch (ast->nodetype)
    {
    case 'K': return (((numval_t *) ast)->number);
    case '+': return (ast_eval(ast->left) + ast_eval(ast->right));
    case '-': return (ast_eval(ast->left) - ast_eval(ast->right));
    case '*': return (ast_eval(ast->left) * ast_eval(ast->right));
    case '/': return (ast_eval(ast->left) / ast_eval(ast->right));
    case 'N': return (-ast_eval(ast->left));
    default: fprintf(stderr, "ERROR: Unknown ast nodetype\n");
    }

    return (0);
}

void
ast_free(ast_t *ast)
{
    switch (ast->nodetype)
    {
    case '+':
    case '-':
    case '*':
    case '/':
        ast_free(ast->right);
    case 'N':
        ast_free(ast->left);
    case 'K':
        free(ast);
        break;
    default:
        fprintf(stderr, "ERROR: Unknown ast nodetype\n");
    }
}