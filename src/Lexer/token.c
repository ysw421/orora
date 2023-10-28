#include "token.h"
#include <stdlib.h>
#include <string.h>

Token* init_token
        (Lexer* lexer, int type, char* value)
{
  Token* token = (Token*) malloc(sizeof(Token));
  token->value = value;
  token->col = lexer->col;
  token->col_first = lexer->col_first;
  lexer->col_first = lexer->col;
  token->row = lexer->row;
  token->row_char = lexer->row_char;
  token->row_char_first = lexer->row_char_first;
  lexer->row_char_first = lexer->row_char;
  token->length = (size_t) strlen(value);
  lexer->row ++;

  return token;
}
