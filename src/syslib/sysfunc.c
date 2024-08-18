#include "sysfunc.h"

AST_value_stack* check_sysfunc(Envs* envs, AST_function* ast_function)
{
  if (strcmp(ast_function->name, "print") == 0)
    return visitor_print_function(envs, ast_function);
  else if (strcmp(ast_function->name, "print") == 0)
    return (void*) 0;

  return (void*) 0;
}
