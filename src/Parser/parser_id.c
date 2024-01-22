#include "parser_id.h"
#include <stdlib.h>
#include "../main.h"

AST* parser_set_variable_value(Parser* parser, AST* ast, Token* last_token);
AST* parser_set_function(Parser* parser, AST* ast, Token* last_token);

AST* parser_parse_variable(Parser* parser, AST* ast, Token* last_token)
{
  Token* token = parser->token;

  if (token == (void*) 0)
    return parser_set_variable_value(parser, ast, last_token);

  // Variable
  switch (token->type)
  {
    case TOKEN_DEFINE:   // Define variable
      if (parser->prev_token->col == token->col_first)
      {
        parser = parser_advance(parser, TOKEN_DEFINE);
        AST* new_ast = parser_value_define(parser, ast, last_token);
      
        return new_ast;
      }
      break;
  }
  // End variable

  return parser_set_variable_value(parser, ast, last_token);
}

AST* parser_parse_function(Parser* parser, AST* ast, Token* last_token,
    AST_function* fa)
{
  Token* token = parser->token;

  AST* new_ast_node =
    init_ast(AST_FUNCTION, ast, last_token);
  new_ast_node->value.function_v = fa;

  if (token == (void*) 0)
    return new_ast_node;

  switch (token->type)
  {
    case TOKEN_DEFINE:
      if (parser->prev_token->col == token->col_first)
      {
        parser = parser_advance(parser, TOKEN_DEFINE);
        for (int i = 0; i < fa->args_size; i ++)
          if (fa->args[i]->type != AST_VARIABLE)
          {
            int required =
              snprintf(NULL, 0, "에러, 함수 %s의 정의를 위해 argument는 변수여야함",
                  fa->name);
            char* error_message = malloc((required + 1) * sizeof(char));
            snprintf(error_message, required + 1,
                "에러, 함수 %s의 정의를 위해 argument는 변수여야함",
                fa->name);
            error(error_message, parser);
          }
        token = parser->token;

        if (!token)
        {
          printf("에러, ':=' 사용에 목적이 없음.");
          exit(1);
        }
        if (parser->prev_token->col != token->col_first)
        {
          printf("에러, ':=' 뒤에는 값이 와야함.");
          exit(1);
        }

        AST* value_node =
            parser_get_value(&parser, ast, token, init_get_value_env());
        token = parser->prev_token;

        if (value_node)
        {
          new_ast_node->value.function_v->codes_size = 1;
          new_ast_node->value.function_v->codes = malloc(sizeof(struct AST*));
          new_ast_node->value.function_v->codes[0] = value_node;
        }
        else
        {
          // ToDo: type2 function....
          // like....
          // f(x) := \begin{code}print(x)\begin{end}
          printf("에러, ':=' 뒤에는 값이 와야함.");
          exit(1);
        }
        return new_ast_node;
      }
      break;
  }

  return new_ast_node;
}

AST* parser_value_define(Parser* parser, AST* ast, Token* last_token)
{
  Token* token = parser->token;

  if (!token)
  {
    printf("에러, ':=' 사용에 목적이 없음.");
    exit(1);
  }

  // Check value
  if (parser->prev_token->col != token->col_first)
  {
    printf("에러, ':=' 뒤에는 값이 와야함.");
    exit(1);
  }
  AST* value_node =
      parser_get_value(&parser, ast, token, init_get_value_env());
  token = parser->prev_token;
  if (value_node)
  {
    if (value_node->value.value_v->size == 1
        && value_node->value.value_v->stack->type == AST_VALUE_VARIABLE)
    {
      AST* new_ast_node =
        init_ast(AST_VARIABLE, ast, last_token);
      new_ast_node->value.variable_v =
        init_ast_variable(last_token->value, last_token->length);

      if (parser->token && parser->token->type == TOKEN_DEFINE
          && parser->prev_token->col == parser->token->col)
      {
        Token* stoken = parser->prev_token;
        parser = parser_advance(parser, TOKEN_DEFINE);

        new_ast_node->value.variable_v->value =
          parser_value_define(parser, new_ast_node, stoken);
        
        token = parser->token;
      }
      else
      {
        AST* new_ast_node2 =
          init_ast(AST_VARIABLE, ast, token);
        new_ast_node2->value.variable_v =
          init_ast_variable(token->value, token->length);
        new_ast_node->value.variable_v->value = new_ast_node2;
        new_ast_node2->value.variable_v->ast_type = AST_VARIABLE_VALUE;

        token = parser->token;
      }
      new_ast_node->value.variable_v->ast_type = AST_VARIABLE_DEFINE;

      return new_ast_node;
    }
    else
    {
      AST* new_ast_node =
        init_ast(AST_VARIABLE, ast, last_token);
      new_ast_node->value.variable_v =
        init_ast_variable(last_token->value, last_token->length);
      new_ast_node->value.variable_v->value = value_node;
      new_ast_node->value.variable_v->ast_type = AST_VARIABLE_DEFINE;

      return new_ast_node;
    }
  }
  else
  {
    printf("에러, ':=' 뒤에는 값이 와야함.");
    exit(1);
  }

  return (void*) 0;
}

AST* parser_get_function(Parser* parser, AST* ast)
{
//   parser = parser_advance(parser, TOKEN_ID);
  Token* token = parser->token;

  if (!parser->next_token
      || token->type != TOKEN_ID
      || parser->next_token->type != TOKEN_LPAR)
    return (void*) 0;

  if (token->col != parser->next_token->col_first)
    return (void*) 0;
//     return parser_set_value(parser, ast, parser->prev_token);

  AST* new_ast = init_ast(AST_FUNCTION, ast, token);
  new_ast->value.function_v =
    init_ast_function(token->value, token->length);
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

      if (new_arg_ast->value.compound_v->size == 0)
      {
        int required =
          snprintf(NULL, 0, "에러, 함수 %s의 ',' 사이 argument가 비어있음",
            new_ast->value.function_v->name);
        char* error_message = malloc((required + 1) * sizeof(char));
        snprintf(error_message, required + 1,
            "에러, 함수 %s의 ',' 사이 argument가 비어있음",
            new_ast->value.function_v->name);
        error(error_message, parser);
      }

      if (new_arg_ast->value.compound_v->size > 1)
      {
        int required =
          snprintf(NULL, 0, "에러, 함수 %s의 각 argument는 ','로 구분되어야 함",
            new_ast->value.function_v->name);
        char* error_message = malloc((required + 1) * sizeof(char));
        snprintf(error_message, required + 1,
            "에러, 함수 %s의 각 argument는 ','로 구분되어야 함",
            new_ast->value.function_v->name);

        error_prev_token(error_message, parser);
      }
      if (token->type == TOKEN_COMMA)
        parser = parser_advance(parser, TOKEN_COMMA);

      int args_num = ++ new_ast->value.function_v->args_size;
      new_ast->value.function_v->args = realloc(new_ast->value.function_v->args,
          args_num * sizeof(struct ast_t*));
      new_ast->value.function_v->args[args_num - 1] =
        new_arg_ast->value.compound_v->items[0];
    }
  }
  parser = parser_advance(parser, TOKEN_RPAR);

  return new_ast;
}

AST* parser_set_variable_value(Parser* parser, AST* ast, Token* last_token)
{
  AST* new_ast_node =
    init_ast(AST_VARIABLE, ast, last_token);
  new_ast_node->value.variable_v =
    init_ast_variable(last_token->value, last_token->length);
  new_ast_node->value.variable_v->ast_type = AST_VARIABLE_VALUE;

  return new_ast_node;
}

AST* parser_set_function(Parser* parser, AST* ast, Token* last_token)
{
  AST* new_ast_node =
    init_ast(AST_FUNCTION, ast, last_token);
  new_ast_node->value.function_v =
    init_ast_function(last_token->value, last_token->length);

  return new_ast_node;
}
