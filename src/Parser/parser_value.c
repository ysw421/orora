#include "parser_id.h"
#include <stdlib.h>

GET_VALUE_ENV* init_get_value_env()
{
  GET_VALUE_ENV* new_env = malloc(sizeof(GET_VALUE_ENV));
  new_env->is_in_parentheses = false;

  return new_env;
}

int parser_get_value_precedence(Token* token)
{
  switch (token->type)
  {
//     case :
  }
}

AST* parser_get_value(Parser* parser, GET_VALUE_ENV* value_env)
{
  orora_value_type* value_type_list;

  AST_value* value = init_ast_value();
  bool is_last_value = false;
  Token* token = parser->token;

  while (token != (void*) 0)
  {

    bool is_single_value = false;
    orora_value_type* p = value_type_list;
    do
    {
      if (token->type == p->token_id)
      {
        is_single_value = true;
        break;
      }
      p = p->next;
    } while (p);

    if (is_single_value)
    {}
  }

  free(value_env);
  return (void*) 0;
}

AST* parser_get_new_int_ast(AST* ast, Token* token)
{
  AST* new_ast_node =
    init_ast(AST_INT, ast, token);
  new_ast_node->int_v = init_ast_int(atoi(token->value));

  return new_ast_node;
}

AST* parser_get_new_float_ast(AST* ast, Token* token)
{
  AST* new_ast_node =
    init_ast(AST_FLOAT, ast, token);
  new_ast_node->float_v = init_ast_float(atof(token->value));

  return new_ast_node;
}

AST* parser_get_new_string_ast(AST* ast, Token* token)
{
  AST* new_ast_node =
    init_ast(AST_STRING, ast, token);
  new_ast_node->string_v = init_ast_string(token->value);

  return new_ast_node;
}
