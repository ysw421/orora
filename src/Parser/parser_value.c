#include "parser_id.h"
#include <stdlib.h>

AST* parser_get_new_int_ast(AST* ast, Token* token)
{
  AST* new_ast_node =
    init_ast(AST_INT, ast, token->col, token->col_first,
              token->row, token->row_char, token->row_char_first);
  new_ast_node->int_v = init_ast_int(atoi(token->value));

  return new_ast_node;
}

AST* parser_get_new_float_ast(AST* ast, Token* token)
{
  AST* new_ast_node =
    init_ast(AST_FLOAT, ast, token->col, token->col_first,
              token->row, token->row_char, token->row_char_first);
  new_ast_node->float_v = init_ast_float(atof(token->value));

  return new_ast_node;
}

AST* parser_get_new_string_ast(AST* ast, Token* token)
{
  AST* new_ast_node =
    init_ast(AST_STRING, ast, token->col, token->col_first,
              token->row, token->row_char, token->row_char_first);
  new_ast_node->string_v = init_ast_string(token->value);

  return new_ast_node;
}

Parser* parser_get_int(Parser* parser, AST* ast, Token* token)
{ 
  ast_compound_add(ast->compound_v, parser_get_new_int_ast(ast, token));

  return parser_advance(parser, TOKEN_INT);
}

Parser* parser_get_float(Parser* parser, AST* ast, Token* token)
{
  ast_compound_add(ast->compound_v, parser_get_new_float_ast(ast, token));

  return parser_advance(parser, TOKEN_FLOAT);
}

Parser* parser_get_string(Parser* parser, AST* ast, Token* token)
{
  ast_compound_add(ast->compound_v, parser_get_new_string_ast(ast, token));

  return parser_advance(parser, TOKEN_STRING);
}
