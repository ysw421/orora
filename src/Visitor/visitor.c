#include <string.h>
#include "visitor.h"

#ifdef DEVELOP_MODE
#include <stdio.h>
#endif

void visitor_visit(Env* env, AST* ast)
{
  if (ast->type == AST_FUNCTION)
  {
    AST_function* ast_function = ast->value.function_v;

#ifdef DEVELOP_MODE
    if (!strcmp(ast_function->name, "print"))
    {
      for (int i = 0; i < ast_function->args_size; i ++)
      {
        switch (ast_function->args[i]->type)
        {
          case AST_STRING:
            printf("%s", ast_function->args[i]->value.string_v->real_value);
            break;
        }
      }
    }
#endif
  }
}
