#include "type.h"

AST_value_stack* type_function(Envs* envs, AST_function* ast_function)
{
  if (ast_function->args_size != 1)
  {
    orora_error("에러, type 함수는 인자를 하나만 받아야 합니다.", (void*) 0);
  }

  AST* ast_function_arg = ast_function->args[0];

  bool is_value = false;
  switch (ast_function_arg->type)
  {
    case AST_VARIABLE:
      if (ast_function_arg->value.variable_v->ast_type == AST_VARIABLE_VALUE)
      {
        is_value = true;
      }
      break;
    case AST_FUNCTION:
      if (ast_function_arg->value.function_v->ast_type == AST_FUNCTION_VALUE)
      {
        is_value = true;
      }
      break;
    case AST_VALUE:
    case AST_CODE:
    case AST_CASES:
      is_value = true;
      break;

    default:
      break;
  }
  if (!is_value)
  {
    orora_error("에러, 값에만 타입이 부여됨", (void*) 0);
  }
  AST_value_stack* new_value = visitor_get_value_from_ast(envs, ast_function_arg);
  AST_value_stack* result = init_ast_value_stack(AST_VALUE_INT, (void*) 0);
  result->value.int_v = malloc(sizeof(struct ast_int_t));
  result->value.int_v->value = new_value->type - 2;

  return result;
}
