#include "ast.h"
#include <stdlib.h>

AST* init_ast(int type)
{
  AST* ast = (AST*) malloc(sizeof(struct ast_t));

  ast->type = type;

  ast->parent = 0;
  ast->col = 0;
  ast->col_first = 0;
  ast->row = 0;
  ast->row_char = 0;
  ast->row_char_first= 0;

  return ast;
}

AST_compound* init_ast_compound()
{
  AST_compound* ast_compound =
    (AST_compound*) malloc(sizeof(struct ast_compound_t));
  ast_compound->size = 0;

  return ast_compound;
}

AST_variable* init_ast_variable()
{
  AST_variable* ast_variable =
    (AST_variable*) malloc(sizeof(struct ast_variable_t));
  ast_variable->name_length = 0;

  return ast_variable;
}

AST_function* init_ast_function()
{
  AST_function* ast_function =
    (AST_function*) malloc(sizeof(struct ast_function_t));
  ast_function->name_length = 0;
  ast_function->args_size = 0;
  ast_function->codes_length = 0;

  return ast_function;
}

AST_string* init_ast_string(char* value)
{
  AST_string* ast_string =
    (AST_string*) malloc(sizeof(struct ast_string_t));
  ast_string->value = value;
  ast_string->value_length = 0;

  return ast_string;
}

AST_int* init_ast_int(int value)
{
  AST_int* ast_int =
    (AST_int*) malloc(sizeof(struct ast_int_t));
  ast_int->value = value;

  return ast_int;
}

AST_float* init_ast_float(float value)
{
  AST_float* ast_float =
    (AST_float*) malloc(sizeof(struct ast_float_t));
  ast_float->value = value;

  return ast_float;
}
