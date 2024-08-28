#include "syslib/get_space.h"

AST_value_stack* get_space_function(Envs* envs, AST_function* ast_function)
{
  if (ast_function->args_size > 1)
  {
    orora_error("에러, ...get.space 함수는 인자를 1개만 받습니다.", (void*) 0);
  }

  AST_value_stack* result = init_ast_value_stack(AST_VALUE_STRING, (void*) 0);
  result->value.string_v = malloc(sizeof(struct ast_string_t));

  AST_value_stack* input = visitor_get_value_from_ast(envs, ast_function->args[0]);
  if (input->type != AST_VALUE_STRING)
  {
    orora_error("에러, ...get.space의 잘못된 사용", (void*) 0);
  }
  else if (strcmp(input->value.string_v->real_value, "real number") == 0
      || strcmp(input->value.string_v->real_value, "integer") == 0
      || strcmp(input->value.string_v->real_value, "natural number") == 0)
  {
    result->value.string_v->value = input->value.string_v->real_value;
    result->value.string_v->real_value = input->value.string_v->real_value;
    result->value.string_v->value_length = input->value.string_v->value_length;
  }
  else
  {
    orora_error("에러, 정의되지 않은 공간", (void*) 0);
  }
  return result;
}
