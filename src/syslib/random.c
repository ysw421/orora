#include "syslib/random.h"

AST_value_stack* random_function(Envs* envs, AST_function* ast_function)
{
  if (ast_function->args_size != 0)
  {
    orora_error("에러, random 함수는 인자를 받지 않습니다.", (void*) 0);
  }

  AST_value_stack* result = init_ast_value_stack(AST_VALUE_FLOAT, (void*) 0);
  result->value.float_v = malloc(sizeof(struct ast_float_t));
  result->value.float_v->value = (float)rand() / (float)RAND_MAX;

  return result;
}
