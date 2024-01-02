#include "parser_id.h"
#include <stdlib.h>
#include "../main.h"

struct value_t
{
  char* name;
  enum
  {
    value_int,
    value_float,
    value_string,
  } type;
} Value;

AST* parser_get_function(Parser* parser, AST* ast);

AST* parser_set_value(Parser* parser, AST* ast, Token* last_token)
{
  AST* new_ast_node =
    init_ast(AST_VARIABLE, ast, last_token);
  new_ast_node->variable_v =
    init_ast_variable(last_token->value, last_token->length);

  return new_ast_node;
}

AST* parser_get_id(Parser* parser, AST* ast, Token* last_token)
{
  struct value_t* value = malloc(sizeof(struct value_t));
  value->name = last_token->value;
  value->type = value_int;
  
  parser = parser_advance(parser, TOKEN_ID);
  Token* token = parser->token;

  if (token == (void*) 0)
  {
    return parser_set_value(parser, ast, last_token);
  }

  // Function
  AST* function_ast = parser_get_function(parser, ast);
  if (function_ast)
    return function_ast;
  // End function

  // Variable
  switch (token->type)
  {
    case TOKEN_EQUAL:   // Define variable
    {
      parser = parser_advance(parser, TOKEN_EQUAL);
      AST* new_ast = parser_value_define(parser, ast, last_token);
      
      return new_ast;
    } break;
  }
  // End variable

  return parser_set_value(parser, ast, last_token);
}

AST* parser_value_define(Parser* parser, AST* ast, Token* last_token)
{
  Token* token = parser->token;

  if (token == (void*) 0)
  {
    printf("에러, '=' 사용에 목적이 없음.");
    exit(1);
  }

  // Check value
  // ToDo: use get_value function
  orora_value_type* p = value_type_list;
  do
  {
    if (token->type == p->token_id)
    {
      AST* new_ast_node =
        init_ast(AST_VARIABLE, ast, last_token);
      new_ast_node->variable_v =
        init_ast_variable(last_token->value, last_token->length);
      new_ast_node->variable_v->value = p->parser_get_new_ast(ast, token);

      parser = parser_advance(parser, p->token_id);
      Token* token = parser->token;

      return new_ast_node;
    }

    p = p->next;
  } while (p);
  // End check value
  
  switch (token->type)
  {
    case TOKEN_ID:
    {
      AST* new_ast_node =
        init_ast(AST_VARIABLE, ast, last_token);
      new_ast_node->variable_v =
        init_ast_variable(last_token->value, last_token->length);

      if (parser->next_token && parser->next_token->type == TOKEN_EQUAL)
      {
        parser = parser_advance(parser, TOKEN_ID);
        parser = parser_advance(parser, TOKEN_EQUAL);

        new_ast_node->variable_v->value =
          parser_value_define(parser, new_ast_node, token);
        
        token = parser->token;
      }
      else
      {
        AST* new_ast_node2 =
          init_ast(AST_VARIABLE, ast, token);
        new_ast_node2->variable_v =
          init_ast_variable(token->value, token->length);
        new_ast_node->variable_v->value = new_ast_node2;

        parser = parser_advance(parser, TOKEN_ID);
        token = parser->token;
      }

      return new_ast_node;
    } break;

    default:
    {
      printf("에러, '=' 뒤에는 값이 와야함.");
      exit(1);
    } break;
  }

  return (void*) 0;
}

AST* parser_get_function(Parser* parser, AST* ast)
{
  Token* token = parser->token;

  if (token->type != TOKEN_ID || parser->next_token->type != TOKEN_LPAR)
    return (void*) 0;

  if (token->col != parser->next_token->col_first)
    return parser_set_value(parser, ast, parser->prev_token);

  AST* new_ast = init_ast(AST_FUNCTION, ast, token);
  new_ast->function_v = init_ast_function(token->value, token->length);
  parser = parser_advance(parser, TOKEN_ID);
  parser = parser_advance(parser, TOKEN_LPAR);
  token = parser->token;

  if (token->type != TOKEN_RPAR)    // if the argument is not empty
  {
    int num_of_lpar = 0;
    while (!(num_of_lpar == 0 && token->type == TOKEN_RPAR))
    {
      GET_COMPOUND_ENV* new_env = init_get_compound_env();
      new_env->is_in_parentheses = true;
      new_env->is_usefull_comma = true;
      AST* new_arg_ast = parser_get_compound(parser, new_env);
      token = parser->token;

      if (new_arg_ast->compound_v->size == 0)
      {
        int required =
          snprintf(NULL, 0, "에러, 함수 %s의 ',' 사이 argument가 비어있음",
            new_ast->function_v->name);
        char* error_message = malloc((required + 1) * sizeof(char));
        snprintf(error_message, required + 1,
            "에러, 함수 %s의 ',' 사이 argument가 비어있음",
            new_ast->function_v->name);
        error(error_message, parser, token);
      }

      if (new_arg_ast->compound_v->size > 1)
      {
        printf("에러, 함수 %s의 각 argument는 ','로 구분되어야 함\n",
            new_ast->function_v->name);
        exit(1);
      }
      if (token->type == TOKEN_COMMA)
        parser = parser_advance(parser, TOKEN_COMMA);

      int args_num = ++ new_ast->function_v->args_size;
      new_ast->function_v->args = realloc(new_ast->function_v->args,
          args_num * sizeof(struct ast_t*));
      new_ast->function_v->args[args_num - 1] = new_arg_ast->compound_v->items[0];
    }
  }
  parser = parser_advance(parser, TOKEN_RPAR);

  return new_ast;
}
