#include "sysfunc.h"

AST_value_stack* check_sysfunc(Envs* envs, AST_function* ast_function)
{
  if (strcmp(ast_function->name, "...print") == 0)
    return visitor_print_function(envs, ast_function);
  if (strcmp(ast_function->name, "...type") == 0)
    return type_function(envs, ast_function);
  if (strcmp(ast_function->name, "...random") == 0)
    return random_function(envs, ast_function);
  if (strcmp(ast_function->name, "...input") == 0)
    return input_function(envs, ast_function);
  if (strcmp(ast_function->name, "...get.space") == 0)
    return get_space_function(envs, ast_function);
  if (strcmp(ast_function->name, "...row") == 0)
    return row_function(envs, ast_function);
  if (strcmp(ast_function->name, "...col") == 0)
    return col_function(envs, ast_function);

  return (void*) 0;
}
