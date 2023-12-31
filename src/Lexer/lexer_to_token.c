#include "lexer.h"
#include <stdlib.h>
#include <string.h>

int get_one_char_type(char c)
{
  switch (c)
  {
    case '=': return TOKEN_EQUAL; break;
    case ',': return TOKEN_COMMA; break;
    case '+': return TOKEN_PLUS; break;
    case '-': return TOKEN_MINUS; break;
    case '*': return TOKEN_STAR; break;
    case '/': return TOKEN_SLASH; break;
    case '(': return TOKEN_LPAR; break;
    case ')': return TOKEN_RPAR; break;
    case '{': return TOKEN_LBRACE; break;
    case '}': return TOKEN_RBRACE; break;
    case '[': return TOKEN_LSQB; break;
    case ']': return TOKEN_RSQB; break;
    case '$': return TOKEN_DOLLAR; break;
    default: return -1; break;
  }

  return -1;
}

Token* lexer_to_token_one_char(Lexer* lexer)
{
  char* value = (char*) malloc(2 * sizeof(char));
  value[0] = lexer->c;
  value[1] = '\0';

  int token_type = get_one_char_type(lexer->c);
  if (token_type != -1)
  {
    lexer_advance(lexer);
//     lexer_advance_by_token(lexer, (unsigned int) strlen(value));
    return init_token(lexer, token_type, value);
  }

  free(value);
  return (void*) 0;
}

// Starting with '\' like '\times'
int get_special_string_type(int length, char* string)
{
  switch (length)
  {
    case 7:
      if (!strcmp(string, "satisfy")) return TOKEN_SATISFY; break;
  }

  if (string[0] != '\\')
    return -1;

  switch (length)
  {
    case 6:
      if (!strcmp(string, "\\begin")) return TOKEN_BEGIN;
      if (!strcmp(string, "\\times")) return TOKEN_STAR;
      if (!strcmp(string, "\\right")) return TOKEN_RIGHT;
      break;
    case 5:
      if (!strcmp(string, "\\left")) return TOKEN_LEFT;
      break;
    case 4:
      if (!strcmp(string, "\\end")) return TOKEN_END;
      if (!strcmp(string, "\\div")) return TOKEN_SLASH;
      break;
    default: return -1; break;
  }

  return -1;
}
