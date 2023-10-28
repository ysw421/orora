#include "lexer.h"
#include <stdlib.h>
#include <string.h>

char* lexer_get_id_add_char(char* id, char* s)
{
  id = realloc(
          id,
          (strlen(id) + strlen(s) + 1) * sizeof(char)
       );
  strcat(id, s);

  return id;
}

Token* lexer_get_id(Lexer* lexer)
{
  char* id = (char*) malloc(2 * sizeof(char));
  id[0] = lexer->c;
  id[1] = '\0';

  lexer_advance(lexer);

  bool is_next_backslash = false;

  while (!IS_LEXER_CONTENTS_END(lexer)
        && !IS_WHITESPACE(lexer->c)
        && !(lexer->c == '\\' && is_next_backslash == false)
        && lexer->c != '\n'
        && get_one_char_type(lexer->c) == -1)
  {
    if (is_next_backslash) // Special literal, e.g.) \n, \t, \r
    {
      is_next_backslash = false;
      char* s = (char*) malloc(2 * sizeof(char));
      s[1] = '\0';
      bool is_special_literal = true;
      switch (lexer->c)
      {
        case 'n': s[0] = '\n'; break;
        case 't': s[0] = '\t'; break;
        case 'r': s[0] = '\r'; break;
        case '\\': s[0] = '\\'; break;
        case '\'': s[0] = '\''; break;
        case '"': s[0] = '"'; break;
        default: is_special_literal = false; break;
      }
      if (is_special_literal)
      {
        id = lexer_get_id_add_char(id, s);
        lexer_advance(lexer);
        free(s);
        continue;
      }
      free(s);
    }

    if (lexer->c == '\\') is_next_backslash = true;

    char* s = (char*) malloc(2 * sizeof(char));
    s[0] = lexer->c;
    s[1] = '\0';

    id = lexer_get_id_add_char(id, s);
    free(s);

    lexer_advance(lexer);
  }

  Token* token = init_token(lexer, ID, id);
  
  return token;
}
