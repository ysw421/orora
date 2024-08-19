#include "syslib/input.h"

AST_value_stack* input_function(Envs* envs, AST_function* ast_function)
{
  if (ast_function->args_size > 1)
  {
    orora_error("에러, input 함수 인자는 최대 1개입니다.", (void*) 0);
  }
  else if (ast_function->args_size == 1)
    visitor_print_function(envs, ast_function);

  char* value = malloc(500 * sizeof(char));;

  scanf("%s", value);
  // ToDo... Connet with web client

  AST_value_stack* result = init_ast_value_stack(AST_VALUE_STRING, (void*) 0);
  result->value.string_v = malloc(sizeof(struct ast_string_t));
  result->value.string_v->value = value;
  result->value.string_v->real_value = value;
  result->value.string_v->value_length = strlen(value);

  return result;
}
