#include "ast.h"

#define ASTSTYPE ast_node_t *
#define YYSTYPE ASTSTYPE

#include "memory.h"
ASTSTYPE
debug(ASTSTYPE node)
{ 
    calc_value_t val;
    memcpy (&val, &node, sizeof (val));
    return node_value_create (val);
}

#define EVAL(value) ASTSTYPE *res = yyget_extra (scanner); *res = value;
#define EVAL_NUM(node)        debug (node);
#define EVAL_ADD(left, right) node_op_create (NT_PLUS, left, right)
#define EVAL_SUB(left, right) node_op_create (NT_MINUS, left, right)
#define EVAL_MUL(left, right) node_op_create (NT_MUL, left, right)
#define EVAL_DIV(left, right) node_op_create (NT_DIV, left, right)
#define EVAL_NEG(value)       node_op_create (NT_NEG, value, NULL)

#define yyparse ast_parse
#include "parser.c"

int
run_ast (config_t *config)
{
    yyscan_t scanner = NULL;
    ast_node_t *ast = NULL;

    if (yylex_init_extra (&ast, &scanner)) {
        fprintf (stderr, "ERROR: cannot initialize scanner\n");
        return (EXIT_FAILURE);
    }

    if (yy_scan_string (config->expr, scanner) == NULL) {
        fprintf (stderr, "ERROR: cannot scan given string\n");
        return (EXIT_FAILURE);
    }

    if (ast_parse (scanner)) {
        fprintf (stderr, "ERROR: cannot parse string\n");
        return (EXIT_FAILURE);
    }

    for (size_t i = 0; i < config->iteration_number; ++i) {
        config->result = ast_eval (ast);
    }

    ast_free (ast);

    return (EXIT_SUCCESS);
}