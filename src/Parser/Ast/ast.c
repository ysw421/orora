#include "ast.h"
#include <string.h>
#include <stdlib.h>

AST* init_ast(int type, AST* parent, Token* token)
{
  AST* ast = (AST*) malloc(sizeof(struct ast_t));

  ast->type = type;

  ast->parent = parent;
  if (token)
  {
    ast->col = token->col;
    ast->col_first = token->col_first;
    ast->row = token->row;
    ast->row_char = token->row_char;
    ast->row_char_first = token->row_char_first;
  }
  else
  {
    ast->col = 0;
    ast->col_first = 0;
    ast->row = 0;
    ast->row_char = 0;
    ast->row_char_first = 0;
  }

  return ast;
}

AST_PARSER* init_ast_parser(AST* ast, Parser* parser)
{
  AST_PARSER* new_ast_parser = malloc(sizeof(AST_PARSER));
  new_ast_parser->ast = ast;
  new_ast_parser->parser = parser;

  return new_ast_parser;
}

AST_code* init_ast_code()
{
  AST_code* ast_code =
    (AST_code*) malloc(sizeof(struct ast_code_t));
  ast_code->code = (void*) 0;

  return ast_code;
}

AST_while* init_ast_while()
{
  AST_while* ast_while =
    (AST_while*) malloc(sizeof(struct ast_while_t));
  ast_while->condition = (void*) 0;
  ast_while->code = (void*) 0;

  return ast_while;
}

AST_if* init_ast_if()
{
  AST_if* ast_if =
    (AST_if*) malloc(sizeof(struct ast_if_t));
  ast_if->condition = (void*) 0;
  ast_if->code = (void*) 0;

  return ast_if;
}

AST_cases* init_ast_cases()
{
  AST_cases* ast_cases = 
    (AST_cases*) malloc(sizeof(struct ast_cases_t));
  ast_cases->is_have_otherwise = false;
  ast_cases->size = 0;
  ast_cases->codes = (void*) 0;
  ast_cases->conditions = (void*) 0;

  return ast_cases;
}

AST_return* init_ast_return()
{
  AST_return* ast_return = 
    (AST_return*) malloc(sizeof(struct ast_return_t));
  ast_return->value = (void*) 0;

  return ast_return;
}

AST_value* init_ast_value()
{
  AST_value* ast_value =
    (AST_value*) malloc(sizeof(struct ast_value_t));
  ast_value->size = 0;
  ast_value->stack =  (void*) 0;

  return ast_value;
}

AST_value_stack* init_ast_value_stack(int type, Token* token)
{
  AST_value_stack* ast =
    (AST_value_stack*) malloc(sizeof(struct ast_value_stack_t));

  ast->type = type;

  if (token)
  {
    ast->col = token->col;
    ast->col_first = token->col_first;
    ast->row = token->row;
    ast->row_char = token->row_char;
    ast->row_char_first = token->row_char_first;
  }

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
  ast_variable->satisfy_size = 0;
  ast_variable->satisfy = (void*) 0;
  ast_variable->ast_type = AST_VARIABLE_NULL;

  return ast_variable;
}

AST_function* init_ast_function(char* name, size_t length)
{
  AST_function* ast_function =
    (AST_function*) malloc(sizeof(struct ast_function_t));
  ast_function->name = name;
  ast_function->name_length = length;
  ast_function->args_size = 0;
  ast_function->args = (void*) 0;
  ast_function->code = (void*) 0;
  ast_function->ast_type = AST_FUNCTION_NULL;

  return ast_function;
}

AST_string* init_ast_string(Token* token)
{
  AST_string* ast_string =
    (AST_string*) malloc(sizeof(struct ast_string_t));
  ast_string->value = token->value;
  char* real_value = malloc((strlen(token->value) - 1) * sizeof(char));
  for (int i = 1; i < strlen(token->value) - 1; i ++)
    real_value[i - 1] = token->value[i];
  real_value[strlen(token->value) - 2] = '\0';
  ast_string->real_value = real_value;
  ast_string->value_length = strlen(token->value);

  return ast_string;
}

AST_int* init_ast_int(Token* token)
{
  AST_int* ast_int =
    (AST_int*) malloc(sizeof(struct ast_int_t));
  ast_int->value = atoi(token->value);

  return ast_int;
}

AST_float* init_ast_float(Token* token)
{
  AST_float* ast_float =
    (AST_float*) malloc(sizeof(struct ast_float_t));
  ast_float->value = atof(token->value);

  return ast_float;
}

AST_compound* ast_compound_add(AST_compound* compound, AST* ast)
{
  if (!ast)
    return (void*) 0;
  compound->size ++;
  compound->items =
    realloc(compound->items, compound->size * sizeof(struct ast_t));
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
   AST_value_stack* (*parser_get_new_ast_value_stack)(Token*, bool),
   bool (*is_check)(Token*),
   int ast_type_id,
   int env_variable_type_id,
   int ast_value_type_id,
   struct env_variable_t* (*visitor_set_value_Env_variable_from_AST_value_stack)
   (struct env_variable_t*, AST_value_stack*),
   AST_value_stack* visitor_set_value_AST_value_stack_from_Env_variable
   (AST_value_stack* new_value_stack, struct env_variable_t* env_variable)
  )
{
  static orora_value_type* pointer = NULL;

  orora_value_type* point = *head;
  init_value_type_list(&point);
//   point->name = (char*) malloc((strlen(name) + 1) * sizeof(char));
  point->name = name;
  point->token_id = token_id;
  point->parser_get_new_ast = parser_get_new_ast;
  point->parser_get_new_ast_value_stack = parser_get_new_ast_value_stack;
  point->is_check_type = is_check;
  point->ast_type_id = ast_type_id;
  point->env_variable_type_id = env_variable_type_id;
  point->ast_value_type_id = ast_value_type_id;
  point->visitor_set_value_Env_variable_from_AST_value_stack =
    visitor_set_value_Env_variable_from_AST_value_stack;
  point->visitor_set_value_AST_value_stack_from_Env_variable =
    visitor_set_value_AST_value_stack_from_Env_variable;

  point->next = pointer;

  pointer = point;
  *head = point;

  return point;
}
