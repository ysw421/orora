#ifndef PARSER_VALUE_H
#define PARSER_VALUE_H

#include "parser.h"
#include <stdio.h>
#include "../Lexer/token.h"

typedef struct get_value_setting_t
{
  bool is_in_parentheses;
} GET_VALUE_SETTING;

AST* parser_get_new_int_ast(AST* ast, Token* token);
AST* parser_get_new_float_ast(AST* ast, Token* token);
AST* parser_get_new_string_ast(AST* ast, Token* token);

Parser* parser_get_int(Parser* parser, AST* ast, Token* token);
Parser* parser_get_float(Parser* parser, AST* ast, Token* token);
Parser* parser_get_string(Parser* parser, AST* ast, Token* token);

#endif
