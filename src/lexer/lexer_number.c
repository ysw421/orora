#include "lexer.h"
#include <string.h>
#include <stdlib.h>

Token* lexer_get_number(Lexer* lexer)
{
  bool is_first_dot = true;
  char* number = malloc(2 * sizeof(char));
  number[0] = '\0';

  while (!IS_LEXER_CONTENTS_END(lexer)
        && (IS_NUMBER_CHAR(lexer->c)
           || (lexer->c == '.' && is_first_dot == true)))
  {
    if (is_first_dot == true && lexer->c == '.')
      is_first_dot = false;
    char* s = malloc(2 * sizeof(char));
    s[0] = lexer->c;
    s[1] = '\0';
    number = realloc(
                number,
                (strlen(number) + strlen(s) + 1) * sizeof(char)
             );
    strcat(number, s);
    free(s);

    lexer_advance(lexer);
  }

  Token* token = init_token(lexer, is_first_dot ? TOKEN_INT : TOKEN_FLOAT, number);

  return token;
}

