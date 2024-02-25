#ifndef PARSER_ID_H
#define PARSER_ID_H

#include "parser/parser_value.h"

AST* parser_parse_variable
(Parser* parser, AST* ast, 
 Token* last_token, GET_COMPOUND_ENV* compound_env);
AST* parser_parse_function(Parser* parser, AST* ast, Token* last_token,
    AST_function* fa, GET_COMPOUND_ENV* compound_env);
AST* parser_get_id(Parser* parser, AST* ast, Token* last_token);
AST* parser_value_define
(Parser* parser, AST* ast, 
 Token* last_token, GET_COMPOUND_ENV* compound_env);
AST* parser_get_function
(Parser* parser, AST* ast, GET_COMPOUND_ENV* compound_env);

#endif
