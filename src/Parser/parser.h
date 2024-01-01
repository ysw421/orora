#ifndef PARSER_H
#define PARSER_H

#include "../env.h"
#include "Ast/ast.h"
#include "parser_T.h"
#include "../Lexer/lexer.h"

typedef struct get_compound_env_t
{
  bool is_in_parentheses;
  bool is_usefull_comma;
} GET_COMPOUND_ENV;

Parser* init_parser(Lexer* lexer);
Parser* parser_advance(Parser* parser, int type);
AST* parser_parse(Parser* parser);

GET_COMPOUND_ENV* init_get_compound_env();
AST* parser_get_compound(Parser* parser, GET_COMPOUND_ENV* compound_env);

#endif
