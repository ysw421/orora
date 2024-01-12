#include <string.h>
#include "visitor.h"

void visitor_visit(Env* env, AST* ast)
{
  if (ast->type == AST_FUNCTION)
  {
    AST_function* ast_function = ast->function_v;

    // For develop...
#include <stdio.h>
    if (!strcmp(ast_function->name, "print"))
    {
      for (int i = 0; i < ast_function->args_size; i ++)
      {
        switch (ast_function->args[i]->type)
        {
          case AST_STRING:
            printf("%s", ast_function->args[i]->string_v->real_value);
            break;
        }
      }
    }
    // end
  }
}
