#include "parser.h"
#include <stdlib.h>
#include <stdio.h>

Parser* init_parser(Lexer* lexer)
{
  Parser* parser = (Parser*) malloc(sizeof(struct parser_t));
  parser->lexer = lexer;
//   parser->prev_token = parser->token;
  parser->prev_token = (void*) 0;
  parser->token = lexer_get_token(lexer);

  return parser;
}

Parser* parser_advance(Parser* parser, int type)
{
  if (parser->token->type != type)
  {
    printf(
        "에러, %s의 타입은 %d가 아닌 %d여야함",
        parser->token->value,
        parser->token->type,
        type
    );
    exit(1);
  }
  parser->prev_token = parser->token;
  parser->token = lexer_get_token(parser->lexer);

  return parser;
}

AST* parser_parse(Parser* parser, Envs* envs)
{
  AST* ast = init_ast(AST_COMPOUND);

  while (parser->token != (void*) 0)
  {}

  return ast;
}
