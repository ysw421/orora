#ifndef PARSER_T_H
#define PARSER_T_H

#include "../Lexer/lexer_T.h"

typedef struct parser_t
{
  Lexer* lexer;
  Token* token;
  Token* prev_token;
  Token* next_token;
} Parser;

#endif
