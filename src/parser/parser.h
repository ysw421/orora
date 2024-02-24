#ifndef PARSER_H
#define PARSER_H

#include "../env.h"
#include "ast/ast.h"
#include "parser_T.h"
#include "../lexer/lexer.h"

typedef struct get_compound_env_t
{
  bool is_allow_linebreak;
  bool is_in_parentheses;
  bool is_in_braces;
  bool is_usefull_comma;
  char* is_usefull_end;
  bool is_size_one;
  bool is_usefull_break;
  bool is_usefull_continue;
  bool is_usefull_return;
} GET_COMPOUND_ENV;

Parser* init_parser(Lexer* lexer);
Parser* parser_set(Parser* parser, size_t pointer);
Parser* parser_advance(Parser* parser, int type);
AST* parser_parse(Parser* parser);

GET_COMPOUND_ENV* init_get_compound_env(GET_COMPOUND_ENV* env);
AST* parser_get_compound(Parser* parser, GET_COMPOUND_ENV* compound_env);

char* parser_is_begin(Parser* parser, int count, ...);
bool parser_is_end(Parser* parser, char* code);

// For test...
AST* parser_get_cases
(Parser* parser, AST* ast, Token* token, 
 Token* s_token, GET_COMPOUND_ENV* compound_env);
AST* parser_get_code
(Parser* parser, AST* ast, Token* token, 
 Token* s_token, GET_COMPOUND_ENV* compound_env, char* code);
// -----------

#endif
