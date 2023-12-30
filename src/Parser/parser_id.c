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
  
  parser = parser_advance(parser, ID);
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
    case EQEQUAL:
    {
      parser = parser_advance(parser, EQEQUAL);
      Token* token = parser->token;

      if (token == (void*) 0)
      {
        printf("에러, '=' 사용에 목적이 없음.");
        exit(1);
      }
      switch (token->type)
      {
        case INT:
        {
          AST* new_ast_node =
            init_ast(AST_VARIABLE, ast, token->col, token->col_first,
                     token->row, token->row_char, token->row_char_first);
          new_ast_node->variable_v =
            init_ast_variable(last_token->value, last_token->length);
          new_ast_node->variable_v->value = parser_get_new_int_ast(ast, token);
          ast_compound_add(ast->compound_v, new_ast_node);

          parser = parser_advance(parser, INT);
          Token* token = parser->token;

          return parser;
        } break;
      }
    } break;
  }

  return parser;
}
