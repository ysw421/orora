#ifndef PARSER_H
#define PARSER_H


#include "../Lexer/lexer.h"

typedef struct parser_t
{
  Lexer* lexer;
  Token* token;
  Token* prev_token;
} Parser;

Parser* init_parser(Lexer* lexer);

#endif
