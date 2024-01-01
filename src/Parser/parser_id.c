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

AST* parser_set_value(Parser* parser, AST* ast, Token* last_token)
{
  AST* new_ast_node =
    init_ast(AST_VARIABLE, ast, last_token);
  new_ast_node->variable_v =
    init_ast_variable(last_token->value, last_token->length);
  ast_compound_add(ast->compound_v, new_ast_node);

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

  switch (token->type)
  {
    case TOKEN_EQUAL:   // Define variable
    {
      parser = parser_advance(parser, TOKEN_EQUAL);
      AST* new_ast = parser_value_define(parser, ast, last_token);
      
      if (new_ast)
        ast_compound_add(ast->compound_v, new_ast);
      
      return new_ast;
    } break;
    case TOKEN_LPAR:  // function
    {
      if (parser->prev_token->col != token->col_first)
        break;
    } break;
  }

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

  // check value
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
  // -----------

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
    return parser_set_value(parser, ast, token);
  
  AST* new_ast = init_ast(AST_FUNCTION, ast, token);
  new_ast->function_v = init_ast_function(token->value, token->length);

  return new_ast;
}
