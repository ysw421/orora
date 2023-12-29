#ifndef PARSER_H
#define PARSER_H

#include "../Lexer/lexer.h"
#include "../env.h"
#include "Ast/ast.h"

typedef struct parser_t
{
  Lexer* lexer;
  Token* token;
  Token* prev_token;
} Parser;

Parser* init_parser(Lexer* lexer);

AST* parser_parse(Parser* parser, Envs* envs);

#endif
