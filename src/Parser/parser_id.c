#include "parser_id.h"
#include <stdlib.h>

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

Parser* parser_get_id(Parser* parser, AST* ast, Token* last_token)
{
  struct value_t* value = malloc(sizeof(struct value_t));
  value->name = last_token->value;
  value->type = value_int;
  
  parser = parser_advance(parser, TOKEN_ID);
  Token* token = parser->token;

  if (token == (void*) 0)
  {
    AST* new_ast_node =
      init_ast(AST_VARIABLE, ast, last_token->col, last_token->col_first,
               last_token->row, last_token->row_char, last_token->row_char_first);
    new_ast_node->variable_v =
      init_ast_variable(last_token->value, last_token->length);
    ast_compound_add(ast->compound_v, new_ast_node);

    return parser;
  }

  switch (token->type)
  {
    case TOKEN_EQEQUAL:   // Define variable
    {
      parser = parser_value_define(parser, ast, last_token);
      token = parser->token;
    } break;
  }

  return parser;
}

Parser* parser_value_define(Parser* parser, AST* ast, Token* last_token)
{
  parser = parser_advance(parser, TOKEN_EQEQUAL);
  Token* token = parser->token;

  if (token == (void*) 0)
  {
    printf("에러, '=' 사용에 목적이 없음.");
    exit(1);
  }
  bool is_checked_type = false;


  switch (token->type)
  {
    case TOKEN_INT:
    {
      AST* new_ast_node =
        init_ast(AST_VARIABLE, ast, token->col, token->col_first,
                  token->row, token->row_char, token->row_char_first);
      new_ast_node->variable_v =
        init_ast_variable(last_token->value, last_token->length);
      new_ast_node->variable_v->value = parser_get_new_int_ast(ast, token);
      ast_compound_add(ast->compound_v, new_ast_node);

      parser = parser_advance(parser, TOKEN_INT);
      Token* token = parser->token;

      return parser;
    } break;
    case TOKEN_FLOAT:
    {
      AST* new_ast_node =
        init_ast(AST_VARIABLE, ast, token->col, token->col_first,
                  token->row, token->row_char, token->row_char_first);
      new_ast_node->variable_v =
        init_ast_variable(last_token->value, last_token->length);
      new_ast_node->variable_v->value = parser_get_new_float_ast(ast, token);
      ast_compound_add(ast->compound_v, new_ast_node);

      parser = parser_advance(parser, TOKEN_FLOAT);
      Token* token = parser->token;

      return parser;
    } break;
    case TOKEN_STRING:
    {
      AST* new_ast_node =
        init_ast(AST_VARIABLE, ast, token->col, token->col_first,
                  token->row, token->row_char, token->row_char_first);
      new_ast_node->variable_v =
        init_ast_variable(last_token->value, last_token->length);
      new_ast_node->variable_v->value = parser_get_new_string_ast(ast, token);
      ast_compound_add(ast->compound_v, new_ast_node);

      parser = parser_advance(parser, TOKEN_STRING);
      Token* token = parser->token;

      return parser;
    } break;
    default:
    {
      printf("에러, '=' 뒤에는 값이 와야함.");
      exit(1);
    } break;
  }
  return parser;
}
