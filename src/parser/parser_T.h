#ifndef PARSER_T_H
#define PARSER_T_H

#include "../lexer/lexer_T.h"

typedef struct parser_t
{
  Lexer* lexer;
  Token* token;
  Token* prev_token;
  Token* next_token;
  Token** row_tokens;
  size_t size;
  size_t pointer;
  Token** tokens;
  size_t row_size;
} Parser;

#endif
