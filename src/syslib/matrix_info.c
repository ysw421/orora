#include "syslib/matrix_info.h"

AST_value_stack* col_function(Envs* envs, AST_function* ast_function)
{
  if (ast_function->args_size > 1)
  {
    orora_error("에러, ...col 함수는 인자를 1개만 받습니다.", (void*) 0);
  }

  AST_value_stack* result = init_ast_value_stack(AST_VALUE_INT, (void*) 0);
  result->value.int_v = malloc(sizeof(struct ast_int_t));

  AST_value_stack* input = visitor_get_value_from_ast(envs, ast_function->args[0]);
  if (input->type != AST_VALUE_MATRIX)
  {
    orora_error("에러, ...col함수는 matrix만을 인자로 받음", (void*) 0);
  }
  result->value.int_v->value = input->value.matrix_v->row_size;
  return result;
}

AST_value_stack* row_function(Envs* envs, AST_function* ast_function)
{
  if (ast_function->args_size > 1)
  {
    orora_error("에러, ...row 함수는 인자를 1개만 받습니다.", (void*) 0);
  }

  AST_value_stack* result = init_ast_value_stack(AST_VALUE_INT, (void*) 0);
  result->value.int_v = malloc(sizeof(struct ast_int_t));

  AST_value_stack* input = visitor_get_value_from_ast(envs, ast_function->args[0]);
  if (input->type != AST_VALUE_MATRIX)
  {
    orora_error("에러, ...row함수는 matrix만을 인자로 받음", (void*) 0);
  }
  result->value.int_v->value = input->value.matrix_v->col_size;
  return result;
}
