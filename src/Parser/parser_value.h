#ifndef PARSER_VALUE_H
#define PARSER_VALUE_H

#include "parser.h"
#include <stdio.h>
#include "../Lexer/token.h"

typedef struct get_value_env_t
{
  bool is_in_parentheses;
} GET_VALUE_ENV;

AST* parser_get_new_int_ast(AST* ast, Token* token);
AST* parser_get_new_float_ast(AST* ast, Token* token);
AST* parser_get_new_string_ast(AST* ast, Token* token);

#endif
