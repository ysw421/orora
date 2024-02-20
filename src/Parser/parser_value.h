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
    Token* last_token, GET_VALUE_ENV* value_env,
    GET_COMPOUND_ENV* compound_env);

AST* parser_get_new_int_ast(AST* ast, Parser* parser);
AST* parser_get_new_float_ast(AST* ast, Parser* parser);
AST* parser_get_new_string_ast(AST* ast, Parser* parser);
AST* parser_get_new_null_ast(AST* ast, Parser* parser);
AST* parser_get_new_bool_ast(AST* ast, Parser* parser);
AST* parser_get_new_matrix_ast(AST* ast, Parser* parser);

AST_value_stack* parser_get_new_int_ast_value_stack
  (Parser* parser, bool is_minus);
AST_value_stack* parser_get_new_float_ast_value_stack
  (Parser* parser, bool is_minus);
AST_value_stack* parser_get_new_string_ast_value_stack
  (Parser* parser, bool is_minus);
AST_value_stack* parser_get_new_null_ast_value_stack
  (Parser* parser, bool is_minus);
AST_value_stack* parser_get_new_bool_ast_value_stack
  (Parser* parser, bool is_minus);
AST_value_stack* parser_get_new_matrix_ast_value_stack
  (Parser* parser, bool is_minus);

bool is_string_ast(Parser* parser);
bool is_float_ast(Parser* parser);
bool is_int_ast(Parser* parser);
bool is_null_ast(Parser* parser);
bool is_bool_ast(Parser* parser);
bool is_matrix_ast(Parser* parser);

int parser_precedence(int ast_stack_id);
AST_value* parser_push_value(AST_value* value, AST_value_stack* node);
AST_value_stack* parser_pop_value(AST_value* value);
int get_ast_value_type(int token_id);
int get_token_type(int ast_value_id);
bool is_operator_use_one_value(int token_id);


#endif

