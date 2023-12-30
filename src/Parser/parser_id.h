#ifndef PARSER_ID_H
#define PARSER_ID_H

#include "parser_value.h"

Parser* parser_get_id(Parser* parser, AST* ast, Token* last_token);
Parser* parser_value_define(Parser* parser, AST* ast, Token* last_token);

#endif
