#ifndef PARSER_VALUE_H
#define PARSER_VALUE_H

#include "parser.h"
#include <stdio.h>
#include "../Lexer/token.h"

typedef struct get_value_env_t
{
  bool is_in_parentheses;
} GET_VALUE_ENV;

GET_VALUE_ENV* init_get_value_env();
AST* parser_get_value(Parser** parser_, AST* ast,
    Token* last_token, GET_VALUE_ENV* value_env);
AST* parser_get_new_int_ast(AST* ast, Token* token);
AST* parser_get_new_float_ast(AST* ast, Token* token);
AST* parser_get_new_string_ast(AST* ast, Token* token);
AST_value_stack* parser_get_new_int_ast_value_stack(Token* token);
AST_value_stack* parser_get_new_float_ast_value_stack(Token* token);
AST_value_stack* parser_get_new_string_ast_value_stack(Token* token);
bool is_string_ast(Token* token);
bool is_float_ast(Token* token);
bool is_int_ast(Token* token);


#endif
