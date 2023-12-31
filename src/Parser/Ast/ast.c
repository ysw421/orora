#include "ast.h"
#include <string.h>
#include <stdlib.h>

AST* init_ast(int type, AST* parent, size_t col, size_t col_first,
              size_t row, size_t row_char, size_t row_char_first)
{
  AST* ast = (AST*) malloc(sizeof(struct ast_t));

  ast->type = type;

  ast->parent = parent;
  ast->col = col;
  ast->col_first = col_first;
  ast->row = row;
  ast->row_char = row_char;
  ast->row_char_first= row_char_first;

  return ast;
}

AST_compound* init_ast_compound()
{
  AST_compound* ast_compound =
    (AST_compound*) malloc(sizeof(struct ast_compound_t));
  ast_compound->size = 0;
  ast_compound->items = (struct ast_t**) malloc(sizeof(struct ast_t*));

  return ast_compound;
}

AST_variable* init_ast_variable(char* name, size_t length)
{
  AST_variable* ast_variable =
    (AST_variable*) malloc(sizeof(struct ast_variable_t));
  ast_variable->name = name;
  ast_variable->value = (void*) 0;
  ast_variable->name_length = length;

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

AST_float* init_ast_float(double value)
{
  AST_float* ast_float =
    (AST_float*) malloc(sizeof(struct ast_float_t));
  ast_float->value = value;

  return ast_float;
}

AST_compound* ast_compound_add(AST_compound* compound, AST* ast)
{
  compound->size ++;
  compound->items = realloc(compound->items, compound->size * sizeof(struct ast_t));
  compound->items[compound->size - 1] = ast;
  return compound;
}

orora_value_type* init_value_type_list(orora_value_type** head)
{
  orora_value_type* node =
    (orora_value_type*) malloc(sizeof(struct orora_value_type_t));
  node->name = (void*) 0;

  *head = node;
  
  return node;
}

orora_value_type* push_value_type_list
  (
   orora_value_type** head,
   char* name,
   int token_id,
   AST* (*parser_get_new_ast)(AST*, Token*),
   Parser* (*parser_get)(Parser*, AST*, Token*)
  )
{
  static orora_value_type* pointer = NULL;

  orora_value_type* point = *head;
  init_value_type_list(&point);
  point->name = (char*) malloc((strlen(name) + 1) * sizeof(char));
  point->name = name;
  point->token_id = token_id;
  point->parser_get_new_ast = parser_get_new_ast;
  point->parser_get = parser_get;

  point->next = pointer;

  pointer = point;
  *head = point;

  return point;
}
